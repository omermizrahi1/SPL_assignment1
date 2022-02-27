#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    Trainer(const Trainer& other);
    Trainer(Trainer&& other);
    ~Trainer();
    Trainer& operator=(const Trainer& other);
    Trainer& operator=(Trainer&& other);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();

    //Auxiliary functions
    void setSumSalary(int addedSalary);
    int  sumUpSalary();
    void removeOrder(int customerId);
    bool isSessionIsOrdered();
    void openSessionIsOrdered();
    void closeSessionIsOrdered();
    bool isCanShowStatus();
    void openCanShowStatus();
    void deleteOrders();
    void deleteCustomer(int id);

private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; // A list of pairs for each order for the trainer - (customer_id, Workout)
    int sumSalary;
    bool sessionIsOrdered;
    bool canShowStatus;

    std::vector<Customer*> copyCustomers(const std::vector<Customer*> &otherCustomersList);
    std::vector<OrderPair> copyOrderPair(const std::vector<OrderPair>& otherOrderPair);
};


#endif
