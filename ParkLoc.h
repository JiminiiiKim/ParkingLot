#ifndef PARKLOC_H
#define PARKLOC_H

#include <map>
#include <string>
#include <vector>
#include "Car.h"

using namespace std;

typedef char** Parking;
typedef unsigned int Row;
typedef unsigned int Col;
typedef unsigned int Occupancy;
typedef map<string, vector<pair<int, int>>> coordMap;

#define ROW 6
#define COL 9

#define SMALL_SIZE 5
#define MIDDLE_SIZE 7

class ParkLoc
{
private:
    Parking parking;
    Row row;
    Col col;
    Occupancy occupancy;
    coordMap coord_map;

public:
    // ������ ���� �����
    ParkLoc(Row row = ROW, Col col = COL) : row{ ROW }, col{ COL }, occupancy{ 0 } {
        parking = new char* [row];
        for (int i = 0; i < row; i++) {
            parking[i] = new char[col];
        }
        int each_row = col / 3;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (j < each_row)
                    parking[i][j] = 'S';
                else if (j < 2 * each_row)
                    parking[i][j] = 'M';
                else
                    parking[i][j] = 'L';
            }
        }
    }
    // Destructor
    ~ParkLoc() {
        for (int i = 0; i < row; i++)
            delete[] parking[i];
        delete[] parking;
    }

    // �������� ȭ�鿡 ���
    void printParkLoc() {
        cout << "\n";
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                cout << parking[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    // ������ �����忡 ����
    bool Park(Car*& park_car) {
        bool availYes = false;
        Seat type = park_car->getSize();
        CarNumber car_num = park_car->getNumber();

        for (int i = 0; (i < row) && (!availYes); i++) {

            for (int j = 0; (j < col) && (!availYes); j++) {
                // �������� ���
                if (type <= SMALL_SIZE) {
                    if (parking[i][j] == 'S')
                    {
                        parking[i][j] = 'O';
                        coord_map[car_num] = { {i, j} };
                        availYes = true;
                    }
                }
                // �������� ���
                else if (type <= MIDDLE_SIZE) {
                    if (parking[i][j] == 'M')
                    {
                        parking[i][j] = 'O';
                        coord_map[car_num] = { {i, j} };
                        availYes = true;
                    }
                }
                // �������� ���
                else {
                    if (parking[i][j] == 'L')
                    {
                        parking[i][j] = 'O';
                        coord_map[car_num] = { {i, j} };
                        availYes = true;
                    }
                }
            }
        }
        if (availYes)
            occupancy++;
        cout << "������ �����Ǿ����ϴ�.\n";
        describeLoc(*this);
        return availYes;
    }

    // �����忡�� ���� ����
    bool deletePark(const string& car_num) {

        if (coord_map.find(car_num) == coord_map.end())
            return false;

        occupancy--;
        vector<pair<int, int>> xy = coord_map[car_num];
        int each_row = col / 3;
        for (auto& each : xy) {
            if (each.second < each_row) // �������� ���
                parking[each.first][each.second] = 'S';
            else if (each.second < 2 * each_row) // �������� ���
                parking[each.first][each.second] = 'M';
            else // �������� ���
                parking[each.first][each.second] = 'L';
        }
        coord_map.erase(car_num);
        return true;
    }

    // ���� ������ ������ �� ���
    void describeLoc(ParkLoc& loc) {
        cout << "���� ������ ������ ���� " << occupancy << "�� �Դϴ�.\n";
    }
};

#endif
