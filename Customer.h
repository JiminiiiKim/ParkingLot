#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;

typedef unsigned int CustomerID;
typedef double Mileage;
typedef string Name;
typedef unsigned int Seat;
typedef string CarNumber;
typedef unsigned int Engine;

typedef struct Data {
    CustomerID id;
    Mileage mileage;
    Name name;
    Seat seat;
    CarNumber num;
    Engine engine;
} Data;

typedef vector<Data> CustInfo;
typedef int InputMileage;

class DataBase {
private:
    CustInfo customer;

public:
    // getter
    CustInfo getDB() const {
        return customer;
    }
    // ȸ�� ID�� ������ ã��
    bool searchDB(CustomerID id, Data& data) {
        for (int i = 0; i < customer.size(); i++)
        {
            if (customer[i].id == id)
            {
                data = customer[i];
                return true;
            }
        }
        return false;
    }
    // ���� ��ȣ������ ������ ã��
    bool searchCarNum(CarNumber num) {
        for (int i = 0; i < customer.size(); i++)
        {
            if (customer[i].num.compare(num) == 0) {
                cout << i;
                return true;
            }
        }
        return false;
    }
    // ȸ���� ���ϸ��� ������Ʈ
    bool upMileage(CustomerID id, InputMileage num) {
        for (int i = 0; i < customer.size(); i++)
        {
            if (customer[i].id == id)
            {
                this->customer[i].mileage = this->customer[i].mileage + num;
                return true;
            }
        }
        return false;
    }
    // ������ ����(csv) �а� ��� ������ ����
    void read_csv() {
        string filename("UserCarData.csv");
        vector<string> lines;
        string line;

        ifstream input_file(filename);
        if (!input_file.is_open()) {
            cerr << "Could not open the file : " << filename << "'" << endl;
            exit(1);
        }

        getline(input_file, line); // 1�� ����
        while (getline(input_file, line)) {
            stringstream ss(line);
            string temp;
            lines.clear();

            while (getline(ss, temp, ',')) {
                lines.push_back(temp);
            }
            Data each;
            each.id = stoi(lines[0]);
            each.name = lines[1];
            each.mileage = stof(lines[2]);
            each.seat = stoi(lines[3]);
            each.num = lines[4];
            each.engine = stoi(lines[5]);
            customer.push_back(each);
        }
        input_file.close();
    }
};

#endif
