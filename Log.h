#ifndef LOG_H
#define LOG_H
#define _CRT_SECURE_NO_WARNINGS

#include "Car.h"
#include <iostream>
#include <ctime>
#include <chrono>
using namespace std;

typedef time_t StartTime;
typedef time_t EndTime;
typedef time_t SpentTime;

class CarLog
{
private:
    StartTime start_time;
    EndTime end_time;
    Car* car;

public:
    // Constructor 
    CarLog(StartTime i_start_time, Car*& c) : start_time{ i_start_time }, end_time{ -1 }, car{ c } {
    }
    CarLog(StartTime i_start_time, EndTime i_end_time, Car*& c) : start_time{ i_start_time }, end_time{ i_end_time }, car{ c } {
    }

    void setStart() {
        start_time = time(NULL);
    }
    void setEnd() {
        end_time = time(NULL);
    }

    CarName getName() const {
        return car->getName();
    }
    CarEngine getEngine() const {
        return car->getEngine();
    }
    CarNumber getNumber() const {
        return car->getNumber();
    }
    Seat getSize() const {
        return car->getSize();
    }
    CustomerYesNo getcustYesNo() const {
        return car->getcustYesNo();
    }
    StartTime getStart() const {
        return start_time;
    }
    EndTime getEnd() const {
        return end_time;
    }
    // 주차장에 머문 시간을 계산
    SpentTime time_spent() const {
        if (end_time == -1) {
            return -1;
        }
        else {
            return end_time - start_time;
        }
    }

    void describe() {
        if (end_time == -1) {
            struct tm* start = localtime(&start_time);
            cout << "번호판 : " << car->getNumber() << "\n";
            cout << "들어온 시간 : " << start_time << "\n";
        }
        else {
            struct tm* start = localtime(&start_time);
            struct tm* end = localtime(&end_time);
            cout << "번호판 : " << car->getNumber() << "\n";
            cout << "들어온 시간 : " << start_time << "\n";
            cout << "나간 시간 : " << end_time << "\n";
        }
    }
};

#endif
