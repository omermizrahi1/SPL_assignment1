#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual ~Customer();
    virtual std::vector<int> order(const std::vector<Workout> &workout_options) = 0;
    virtual std::string toString() const = 0; // if not virtual, it used by the super's function.
    std::string getName() const;
    int getId() const;
    virtual Customer* createCustomerType() = 0;

private:
    const std::string name;
    const int id;
};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* createCustomerType();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* createCustomerType();
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* createCustomerType();
private:
    void sortByPrice(const std::vector<Workout> &workout_options, std::vector<int> &order);
    void quickSort(const std::vector<Workout> &workout_options, std::vector<int> &order, int low, int high);
    int partition (const std::vector<Workout> &workout_options, std::vector<int> &order, int low, int high);
    void swap(int* a, int* b);
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* createCustomerType();
private:
    int findCheapestOrExpensiveByType(const std::vector<Workout> &workout_options, WorkoutType type, bool cheapest);
};

#endif
