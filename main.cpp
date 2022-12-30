#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Customer.h"
#include "Log.h"
#include "Car.h"
#include "ParkLoc.h"
#include "CostCalculator.h"

using namespace std;

typedef unsigned int Type;
typedef string CarName;
typedef unsigned int CarEngine;
typedef string Plate;
typedef unsigned int customerNumber;
typedef unsigned int Action;
typedef enum { Park = 1, Remove, DataLog, PrintAll, SpecificInfo, CSVFile, EXIT };
typedef bool Check;
typedef vector<CarLog> TotalLog;
typedef vector<CarLog> CurrentLog;

#define SMALL_SIZE 1600
#define MIDDLE_SIZE 2000

bool searchLog(CurrentLog& currentLog, string num);
void customerCost(CarLog& currentLog, const Data& data, DataBase& DB);
void Cost(CarLog& currentLog, double& cost);

int main() {
    DataBase DB;
    DB.read_csv();

    ParkLoc PL(ROW, COL); // 주차 공간 생성
    PL.printParkLoc();

    Type type;
    CarName car_name;
    Plate number_plate;
    CarEngine engine;

    TotalLog totalLog; // 전체 차량의 로그
    CurrentLog currentLog; // 현재 주차된 차량의 로그

    while (true)
    {

        cout << " +------------------------------------------+" << "\n";
        cout << " |             주차 관리 시스템             |" << "\n";
        cout << " +------------------------------------------+" << "\n";
        cout << " |        (1) 자동차 주차                   |" << "\n";
        cout << " |        (2) 자동차 빼기                   |" << "\n";
        cout << " |        (3) 데이터 로그 출력              |" << "\n";
        cout << " |        (4) 주차장 현재 상황 출력         |" << "\n";
        cout << " |        (5) 현재 주차된 차량 정보 표시    |" << "\n";
        cout << " |        (6) 로그 csv 파일 만들기          |" << "\n";
        cout << " |        (7) 프로그램 종료                 |" << "\n";
        cout << " +------------------------------------------+" << "\n";
        cout << "번호를 고르세요. ";

        bool custYes;
        bool ParkYes;
        bool find = false;
        customerNumber customerNum;
        Action action;

        cin >> action;
        switch (action) {
        case Park: {
            Car* entry_car;
            cout << "회원이신가요? (맞다면 1을 아니라면 0을 입력해주세요.) : ";
            cin >> custYes;
            if (custYes) {
                cout << "회원 번호를 입력해주세요 : ";
                cin >> customerNum;
                Data cust;
                bool custERROR = DB.searchDB(customerNum, cust);
                if (!custERROR)
                    cout << "회원이 아닙니다. 다시 확인해주세요. \n";
                else {
                    entry_car = new Car(cust);
                    bool carnumERROR = searchLog(currentLog, cust.num);
                    if (carnumERROR)
                        cout << "이미 등록되어 있는 차량입니다. 다시 확인해주세요.\n";
                    else {
                        ParkYes = PL.Park(entry_car);
                        if (ParkYes) {
                            time_t now = time(NULL);
                            currentLog.push_back(CarLog(now, entry_car));
                        }
                        else
                            cout << "현재 가능한 공간이 없습니다. 다음에 이용해주세요.\n";
                    }
                }
            }
            else {
                cout << "차 종류를 입력하세요 : ";
                cin >> car_name;
                cout << "좌석 수를 입력하세요 : ";
                cin >> type;
                cout << "배기량을 입력하세요 : ";
                cin >> engine;
                cout << "번호판을 입력하세요 : ";
                cin >> number_plate;
                bool carnumERROR = searchLog(currentLog, number_plate);
                if (carnumERROR)
                    cout << "이미 등록되어 있는 차량입니다. 다시 확인해주세요.\n";
                else {
                    entry_car = new Car(type, engine, car_name, number_plate, false);
                    // 입력한 차량 주차
                    ParkYes = PL.Park(entry_car);
                    if (ParkYes) {
                        time_t now = time(NULL);
                        currentLog.push_back(CarLog(now, entry_car));
                    }
                    else
                        cout << "현재 가능한 공간이 없습니다. 다음에 이용해주세요.\n";
                }
            }
            break;
        }

        case Remove: {
            Car* out_car;
            cout << "회원이신가요? (맞다면 1을 아니라면 0을 입력해주세요.) : ";
            cin >> custYes;
            if (custYes) {
                cout << "회원 번호를 입력해주세요 : ";
                cin >> customerNum;
                Data cust;
                custYes = DB.searchDB(customerNum, cust);
                if (!custYes)
                    cout << "회원이 아닙니다. 다시 확인해주세요.\n";
                else {
                    // 주차장에서 차량 빼기
                    out_car = new Car(cust);
                    ParkYes = PL.deletePark(out_car->getNumber());
                    if (!ParkYes)
                        cout << "입력하신 " << out_car->getNumber() << "는 조회되지 않는 차량입니다.\n";
                    else
                    {
                        // 요금 계산
                        for (int i = 0; i < currentLog.size(); i++)
                        {
                            if (currentLog[i].getNumber() == out_car->getNumber())
                            {
                                currentLog[i].setEnd();
                                totalLog.push_back(currentLog[i]);
                                customerCost(currentLog[i], cust, DB);
                                currentLog.erase(currentLog.begin() + i);
                                break;
                            }
                        }
                    }
                }
            }
            else {
                cout << "번호판을 입력해주세요 : ";
                cin >> number_plate;
                // 주차장에서 차량 빼기
                ParkYes = PL.deletePark(number_plate);
                if (!ParkYes) {
                    cout << "입력하신 " << number_plate << "는 조회되지 않는 차량입니다.\n";
                }
                else {
                    // 요금 계산
                    CostCalculator* parent;
                    for (int i = 0; i < currentLog.size(); i++)
                    {
                        if (currentLog[i].getNumber() == number_plate)
                        {
                            double cost;
                            currentLog[i].setEnd();
                            Cost(currentLog[i], cost);
                            totalLog.push_back(currentLog[i]);
                            currentLog.erase(currentLog.begin() + i);
                            break;
                        }
                    }
                }
            }
            break;

        }
        case DataLog: {
            cout << "모든 차량 로그를 조회합니다.\n";
            for (CarLog& log : currentLog)
                log.describe();
            for (CarLog& log : totalLog)
                log.describe();
            break;
        }
        case PrintAll: {
            PL.printParkLoc();
            break;
        }
        case SpecificInfo: {
            for (CarLog& log : currentLog) {
                if (log.getEngine() <= SMALL_SIZE) {
                    Small small = Small(log.getSize(), log.getEngine(), log.getName(), log.getNumber(), log.getcustYesNo());
                    small.describe();
                }
                else if (log.getEngine() <= MIDDLE_SIZE) {
                    Middle middle = Middle(log.getSize(), log.getEngine(), log.getName(), log.getNumber(), log.getcustYesNo());
                    middle.describe();
                }
                else {
                    Big big = Big(log.getSize(), log.getEngine(), log.getName(), log.getNumber(), log.getcustYesNo());
                    big.describe();
                }
            }
            break;
        }
        case CSVFile: {
            ofstream out_file("Log.csv");
            out_file << "CarNumber,StartTime,EndTime\n";
            for (CarLog& log : totalLog)
                out_file << log.getNumber() << "," << log.getStart() << "," << log.getEnd() << "\n";
            cout << "Log.csv 파일이 생성되었습니다.\n";
            out_file.close();
            break;
        }
        case EXIT: {
            cout << "프로그램을 종료합니다.";
            exit(0);
            break;
        }
        }
    }
}

// 회원의 주차 요금 계산
void customerCost(CarLog& currentLog, const Data& cust, DataBase& DB) {
    double cost;
    CostCalculator* calculator;
    cout << currentLog.getNumber() << " 차량의 주차 요금은 ";
    if (currentLog.getEngine() <= SMALL_SIZE) {
        SmallCostCalculator small;
        calculator = &small;
    }
    else if (currentLog.getEngine() <= MIDDLE_SIZE) {
        MiddleCostCalculator middle;
        calculator = &middle;
    }
    else {
        BigCostCalculator big;
        calculator = &big;
    }
    cost = calculator->CustcalCost(currentLog);
    cout << cost << "원입니다.\n";

    Mileage mileage;
    cout << "귀하는 현재 " << cust.mileage << " 마일리지가 있습니다. (사용하신다면 원하는 마일리지를, 아니면 0을 입력해주세요.) : ";
    cin >> mileage;
    while (mileage < 0 || mileage > cust.mileage) {
        cout << "유효한 값으로 입력해주세요.\n";
        cin >> mileage;
    }
    if (mileage == 0)
        DB.upMileage(cust.id, 2.0);
    else
        DB.upMileage(cust.id, -mileage);
    cout << mileage << "마일리지를 차감하여 " << cost - mileage << "원입니다.\n";
}

// 비회원의 주차 요금 계산
void Cost(CarLog& currentLog, double& cost) {
    CostCalculator* calculator;
    cout << currentLog.getNumber() << " 차량의 주차 요금은 ";
    if (currentLog.getEngine() <= SMALL_SIZE) {
        SmallCostCalculator small;
        calculator = &small;
    }
    else if (currentLog.getEngine() <= MIDDLE_SIZE) {
        MiddleCostCalculator middle;
        calculator = &middle;
    }
    else {
        BigCostCalculator big;
        calculator = &big;
    }
    cost = calculator->calCost(currentLog);
    cout << cost << "원입니다.\n";
}

// 주차장에 있는 차량인지 확인
bool searchLog(CurrentLog& currentLog, string num) {
    for (CarLog& log : currentLog) {
        if (log.getNumber() == num) {
            return true;
        }
    }
    return false;
}