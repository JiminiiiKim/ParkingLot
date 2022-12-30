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

    ParkLoc PL(ROW, COL); // ���� ���� ����
    PL.printParkLoc();

    Type type;
    CarName car_name;
    Plate number_plate;
    CarEngine engine;

    TotalLog totalLog; // ��ü ������ �α�
    CurrentLog currentLog; // ���� ������ ������ �α�

    while (true)
    {

        cout << " +------------------------------------------+" << "\n";
        cout << " |             ���� ���� �ý���             |" << "\n";
        cout << " +------------------------------------------+" << "\n";
        cout << " |        (1) �ڵ��� ����                   |" << "\n";
        cout << " |        (2) �ڵ��� ����                   |" << "\n";
        cout << " |        (3) ������ �α� ���              |" << "\n";
        cout << " |        (4) ������ ���� ��Ȳ ���         |" << "\n";
        cout << " |        (5) ���� ������ ���� ���� ǥ��    |" << "\n";
        cout << " |        (6) �α� csv ���� �����          |" << "\n";
        cout << " |        (7) ���α׷� ����                 |" << "\n";
        cout << " +------------------------------------------+" << "\n";
        cout << "��ȣ�� ������. ";

        bool custYes;
        bool ParkYes;
        bool find = false;
        customerNumber customerNum;
        Action action;

        cin >> action;
        switch (action) {
        case Park: {
            Car* entry_car;
            cout << "ȸ���̽Ű���? (�´ٸ� 1�� �ƴ϶�� 0�� �Է����ּ���.) : ";
            cin >> custYes;
            if (custYes) {
                cout << "ȸ�� ��ȣ�� �Է����ּ��� : ";
                cin >> customerNum;
                Data cust;
                bool custERROR = DB.searchDB(customerNum, cust);
                if (!custERROR)
                    cout << "ȸ���� �ƴմϴ�. �ٽ� Ȯ�����ּ���. \n";
                else {
                    entry_car = new Car(cust);
                    bool carnumERROR = searchLog(currentLog, cust.num);
                    if (carnumERROR)
                        cout << "�̹� ��ϵǾ� �ִ� �����Դϴ�. �ٽ� Ȯ�����ּ���.\n";
                    else {
                        ParkYes = PL.Park(entry_car);
                        if (ParkYes) {
                            time_t now = time(NULL);
                            currentLog.push_back(CarLog(now, entry_car));
                        }
                        else
                            cout << "���� ������ ������ �����ϴ�. ������ �̿����ּ���.\n";
                    }
                }
            }
            else {
                cout << "�� ������ �Է��ϼ��� : ";
                cin >> car_name;
                cout << "�¼� ���� �Է��ϼ��� : ";
                cin >> type;
                cout << "��ⷮ�� �Է��ϼ��� : ";
                cin >> engine;
                cout << "��ȣ���� �Է��ϼ��� : ";
                cin >> number_plate;
                bool carnumERROR = searchLog(currentLog, number_plate);
                if (carnumERROR)
                    cout << "�̹� ��ϵǾ� �ִ� �����Դϴ�. �ٽ� Ȯ�����ּ���.\n";
                else {
                    entry_car = new Car(type, engine, car_name, number_plate, false);
                    // �Է��� ���� ����
                    ParkYes = PL.Park(entry_car);
                    if (ParkYes) {
                        time_t now = time(NULL);
                        currentLog.push_back(CarLog(now, entry_car));
                    }
                    else
                        cout << "���� ������ ������ �����ϴ�. ������ �̿����ּ���.\n";
                }
            }
            break;
        }

        case Remove: {
            Car* out_car;
            cout << "ȸ���̽Ű���? (�´ٸ� 1�� �ƴ϶�� 0�� �Է����ּ���.) : ";
            cin >> custYes;
            if (custYes) {
                cout << "ȸ�� ��ȣ�� �Է����ּ��� : ";
                cin >> customerNum;
                Data cust;
                custYes = DB.searchDB(customerNum, cust);
                if (!custYes)
                    cout << "ȸ���� �ƴմϴ�. �ٽ� Ȯ�����ּ���.\n";
                else {
                    // �����忡�� ���� ����
                    out_car = new Car(cust);
                    ParkYes = PL.deletePark(out_car->getNumber());
                    if (!ParkYes)
                        cout << "�Է��Ͻ� " << out_car->getNumber() << "�� ��ȸ���� �ʴ� �����Դϴ�.\n";
                    else
                    {
                        // ��� ���
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
                cout << "��ȣ���� �Է����ּ��� : ";
                cin >> number_plate;
                // �����忡�� ���� ����
                ParkYes = PL.deletePark(number_plate);
                if (!ParkYes) {
                    cout << "�Է��Ͻ� " << number_plate << "�� ��ȸ���� �ʴ� �����Դϴ�.\n";
                }
                else {
                    // ��� ���
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
            cout << "��� ���� �α׸� ��ȸ�մϴ�.\n";
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
            cout << "Log.csv ������ �����Ǿ����ϴ�.\n";
            out_file.close();
            break;
        }
        case EXIT: {
            cout << "���α׷��� �����մϴ�.";
            exit(0);
            break;
        }
        }
    }
}

// ȸ���� ���� ��� ���
void customerCost(CarLog& currentLog, const Data& cust, DataBase& DB) {
    double cost;
    CostCalculator* calculator;
    cout << currentLog.getNumber() << " ������ ���� ����� ";
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
    cout << cost << "���Դϴ�.\n";

    Mileage mileage;
    cout << "���ϴ� ���� " << cust.mileage << " ���ϸ����� �ֽ��ϴ�. (����ϽŴٸ� ���ϴ� ���ϸ�����, �ƴϸ� 0�� �Է����ּ���.) : ";
    cin >> mileage;
    while (mileage < 0 || mileage > cust.mileage) {
        cout << "��ȿ�� ������ �Է����ּ���.\n";
        cin >> mileage;
    }
    if (mileage == 0)
        DB.upMileage(cust.id, 2.0);
    else
        DB.upMileage(cust.id, -mileage);
    cout << mileage << "���ϸ����� �����Ͽ� " << cost - mileage << "���Դϴ�.\n";
}

// ��ȸ���� ���� ��� ���
void Cost(CarLog& currentLog, double& cost) {
    CostCalculator* calculator;
    cout << currentLog.getNumber() << " ������ ���� ����� ";
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
    cout << cost << "���Դϴ�.\n";
}

// �����忡 �ִ� �������� Ȯ��
bool searchLog(CurrentLog& currentLog, string num) {
    for (CarLog& log : currentLog) {
        if (log.getNumber() == num) {
            return true;
        }
    }
    return false;
}