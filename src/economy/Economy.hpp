#pragma once
#include "core/Types.hpp"
namespace fg { class Economy { public: int balance()const{return balance_;} bool canAfford(int n)const{return balance_>=n;} bool spend(int amount); void earn(int amount); void sellFood(DailyReport& report); int closeDay(DailyReport& report); private:int balance_{2500}; }; }
