#ifndef COST_CALC_H
#define COST_CALC_H

#include "Log.h"

typedef int Fee;
typedef int BasicFee;
typedef double Rate;

// Abstract Base Class
class CostCalculator
{
private:
    Fee cost;
public:
    // Constructor
    CostCalculator() {
        cost = 0;
    }
    // getter
    Fee getCost() const {
        return cost;
    }
    // setter
    void setCost(Fee num) {
        cost = num;
    }
    // Pure virtual function
    virtual double calCost(CarLog& log) = 0;
    virtual double CustcalCost(CarLog& log) = 0;
};

// Child Class
class SmallCostCalculator : public CostCalculator
{
private:
    Rate small_rate;
    BasicFee small_fee;
public:
    // Constructor
    SmallCostCalculator() {
        small_rate = 3.5;
        small_fee = 1000;
    }
    // getter
    Rate get_small_rate() const {
        return small_rate;
    }
    BasicFee get_small_fee() const {
        return small_fee;
    }
    // setter
    void set_small_rate(double num) {
        small_rate = num;
    }
    void set_small_fee(double num) {
        small_fee = num;
    }
    // 요금을 계산하는 함수
    double calCost(CarLog& log) {
        if (log.time_spent() == -1)
            return 0;
        return small_fee + (small_rate * log.time_spent());
    }
    double CustcalCost(CarLog& log) {
        if (log.time_spent() == -1)
            return 0;
        return small_fee + ((small_rate - 0.5) * log.time_spent());
    }
};

class MiddleCostCalculator : public CostCalculator
{
private:
    Rate middle_rate;
    BasicFee middle_fee;
public:
    // Constructor
    MiddleCostCalculator() {
        middle_rate = 4.5;
        middle_fee = 2000;
    }
    // getter
    Rate get_middle_rate() const {
        return middle_rate;
    }
    BasicFee get_middle_fee() const {
        return middle_fee;
    }
    // setter
    void set_middle_rate(double num) {
        middle_rate = num;
    }
    void set_middle_fee(double num) {
        middle_fee = num;
    }
    // 요금을 계산하는 함수
    double calCost(CarLog& log) {
        if (log.time_spent() == -1)
            return 0;
        return middle_fee + (middle_rate * log.time_spent());
    }
    double CustcalCost(CarLog& log) {
        if (log.time_spent() == -1)
            return 0;
        return middle_fee + ((middle_rate - 0.5) * log.time_spent());
    }
};

class BigCostCalculator : public CostCalculator
{
private:
    Rate big_rate;
    BasicFee big_fee;
public:
    // Constructor
    BigCostCalculator() {
        big_rate = 5.0;
        big_fee = 3000;
    }
    // getter
    Rate get_big_rate() const {
        return big_rate;
    }
    BasicFee get_big_fee() const {
        return big_fee;
    }
    // setter
    void set_big_rate(double num) {
        big_rate = num;
    }
    void set_big_fee(double num) {
        big_fee = num;
    }
    // 요금을 계산하는 함수
    double calCost(CarLog& log) {
        if (log.time_spent() == -1)
            return 0;
        return big_fee + (big_rate * log.time_spent());
    }
    double CustcalCost(CarLog& log) {
        if (log.time_spent() == -1)
            return 0;
        return big_fee + ((big_rate - 0.5) * log.time_spent());
    }
};

#endif
