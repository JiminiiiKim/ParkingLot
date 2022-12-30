#ifndef CAR_H
#define CAR_H

#include <string>
#include <iostream>
using namespace std;

typedef unsigned int Size;
typedef unsigned int CarEngine;
typedef string CarName;
typedef string CarNumber;
typedef bool CustomerYesNo;
typedef int SmallType;
typedef int MiddleType;
typedef int BigType;
typedef enum { Bike, CompactCar, SportsCar };
typedef enum { PassengerCar, SUV };
typedef enum { Van, Bus };

// Parent Class
class Car
{
protected:
    Size size;
    CarEngine car_engine;
    CarName car_name;
    CarNumber car_num;
    CustomerYesNo custYesNo;

public:
    // Constructor
    Car(Size i_size, CarEngine i_car_engine, CarName i_car_name, CarNumber i_car_num, CustomerYesNo i_custYesNo) {
        size = i_size;
        car_engine = i_car_engine;
        car_name = i_car_name;
        car_num = i_car_num;
        custYesNo = i_custYesNo;
    }
    Car(const Car& p) {
        size = p.size;
        car_engine = p.car_engine;
        car_name = p.car_name;
        car_num = p.car_num;
        custYesNo = p.custYesNo;
    }
    Car(const Data& p) {
        size = p.seat;
        car_engine = p.engine;
        car_name = p.name;
        car_num = p.num;
        custYesNo = true;
    }

    // getter
    Size getSize() const {
        return size;
    }
    CarEngine getEngine() const {
        return car_engine;
    }
    CarName getName() const {
        return car_name;
    }
    CarNumber getNumber() const {
        return car_num;
    }
    CustomerYesNo getcustYesNo() const {
        return custYesNo;
    }

    // 차량 정보 표시
    void describe() const {
        cout << "차 종류 : " << car_name << ", 번호판 : " << car_num << ".\n";
    }
};

// Child Class
class Small : public Car
{
private:
    SmallType small_type = CompactCar;
public:
    // Constructor
    Small(Size size, CarEngine car_engine, CarName car_name, CarNumber car_num, CustomerYesNo custYesNo) : Car(size, car_engine, car_name, car_num, custYesNo) {
    }
    Small(const Car& p) : Car(p) {
    }
    // getter
    SmallType getType() const {
        return small_type;
    }
    // setter
    void setType(SmallType a) {
        small_type = a;
    }
    // 차량 정보 표시
    void describe() const {
        cout << car_name << " 소형차의 번호판 : " << car_num << "\n";
    }
};

class Middle : public Car
{
private:
    MiddleType middle_type = SUV;
public:
    // Constructor
    Middle(Size size, CarEngine car_engine, CarName car_name, CarNumber car_num, CustomerYesNo custYesNo) : Car(size, car_engine, car_name, car_num, custYesNo) {
    }
    Middle(const Car& p) : Car(p) {
    }
    // getter
    MiddleType getType() const {
        return middle_type;
    }
    // setter
    void setType(MiddleType a) {
        middle_type = a;
    }
    // 차량 정보 표시
    void describe() const {
        cout << car_name << " 중형차의 번호판 : " << car_num << "\n";
    }
};

class Big : public Car
{
private:
    BigType big_type = Bus;
public:
    // Constructor
    Big(Size size, CarEngine car_engine, CarName car_name, CarNumber car_num, CustomerYesNo custYesNo) : Car(size, car_engine, car_name, car_num, custYesNo) {
    }
    Big(const Car& p) : Car(p) {
    }
    // getter
    BigType getType() const {
        return big_type;
    }
    // setter
    void setType(BigType a) {
        big_type = a;
    }
    // 차량 정보 표시
    void describe() const {
        cout << car_name << " 대형차의 번호판 : " << car_num << "\n";
    }
};

#endif
