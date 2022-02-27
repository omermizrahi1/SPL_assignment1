#include <iostream>
#include "../include/Trainer.h"

Trainer:: Trainer(int t_capacity): capacity(t_capacity) {
    open = false;
    customersList = std::vector<Customer*>();
    orderList = std::vector<OrderPair>();
    sumSalary = 0;
    sessionIsOrdered = false;
    canShowStatus = false;
}

Trainer::Trainer(const Trainer &other) : capacity(other.capacity), open(other.open), sumSalary(other.sumSalary),
sessionIsOrdered(other.sessionIsOrdered),
canShowStatus(other.canShowStatus) {
    customersList = copyCustomers(other.customersList);
    orderList = copyOrderPair(other.orderList);
}

Trainer::Trainer(Trainer &&other) : capacity(std::move(other.capacity)), open(std::move(other.open)),
customersList(std::move(other.customersList)),
orderList(std::move(other.orderList)),
sumSalary(std::move(other.sumSalary)),
sessionIsOrdered(std::move(other.sessionIsOrdered)),
canShowStatus(std::move(other.canShowStatus)) {}

Trainer:: ~Trainer() {
    capacity = 0;
    open = NULL;
    for (auto customer: customersList) {
        delete customer;
    }
    customersList.clear();
    orderList.clear();
    sumSalary = 0;
    sessionIsOrdered = NULL;
    canShowStatus = NULL;
}

Trainer &Trainer:: operator=(const Trainer &other) {
    if (this == &other) {
        capacity = other.capacity;
        open = other.open;
        for (auto customer: customersList) {
            delete customer;
        }
        customersList.clear();
        customersList = copyCustomers(other.customersList);
        orderList.clear();
        orderList = copyOrderPair(other.orderList);
        sumSalary = other.sumSalary;
        sessionIsOrdered = other.sessionIsOrdered;
        canShowStatus = other.canShowStatus;
    }
    return *this;
}

Trainer &Trainer:: operator=(Trainer &&other) {
    if (this != &other) {
        capacity = std::move(other.capacity);
        open = std::move(other.open);
        customersList = std::move(other.customersList);
        orderList = std::move(other.orderList);
        sumSalary = std::move(other.sumSalary);
        sessionIsOrdered = std::move(other.sessionIsOrdered);
        canShowStatus = std::move(other.canShowStatus);
    }
    return *this;
}

int Trainer:: getCapacity() const {
    return capacity;
}

void Trainer:: addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Trainer:: removeCustomer(int id) {
    for (unsigned int i = 0; i < customersList.size(); i++) {
        if (customersList[i]->getId() == id) {
            customersList.erase(customersList.begin() + i);
        }
    }
}

void Trainer:: deleteCustomer(int id) {
    for (unsigned int i = 0; i < customersList.size(); i++) {
        if (customersList[i]->getId() == id) {
            delete customersList[i];
            customersList.erase(customersList.begin() + i);
        }
    }
}

Customer *Trainer:: getCustomer(int id) {
    for (Customer *customer : customersList) {
        if (customer->getId() == id) {
            return customer;
        }
    }
    return nullptr;
}

std::vector<Customer*> &Trainer:: getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Trainer:: getOrders() {
    return orderList;
}

void Trainer:: order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    for (int workoutId : workout_ids) {
        Workout workout = workout_options[workoutId];
        std::pair<int, Workout> order(customer_id, workout);
        orderList.push_back(order);
    }
}

void Trainer:: openTrainer() {
    this->open = true;
}

void Trainer:: closeTrainer() {
    this->open = false;
}

int Trainer:: getSalary() {
    // Summarized the pay from the customers we did not change their id (removeOrder method)
    int sumPrice = 0;
    std::vector<OrderPair> &orders = this->getOrders();
    for (OrderPair order : orders) {
        if (order.first >= 0) {
            int price = order.second.getPrice();
            sumPrice = sumPrice + price;
        }
    }
    return sumPrice;
}

bool Trainer::isOpen() {
    return open;
}


// Auxiliary functions
std::vector<Customer*> Trainer:: copyCustomers(const std::vector<Customer*> &otherCustomersList) {
    std::vector<Customer*> customers;
    for (auto customerPtr : otherCustomersList) {
        customers.push_back(customerPtr->createCustomerType());
    }
    return customers;
}

std::vector<OrderPair> Trainer:: copyOrderPair(const std::vector<OrderPair> &otherOrderPair) {
    std::vector<OrderPair> orders;
    for (auto order : otherOrderPair) {
        orders.push_back(OrderPair(order.first, order.second));
    }
    return orders;
}
void Trainer:: deleteOrders() {
    orderList.clear();
}

void Trainer:: removeOrder(int customerId) {
    // Changes the costumerId to (-1)
    for (unsigned int i = 0; i < orderList.size(); i++) {
        if (orderList[i].first == customerId) {
            orderList[i].first = -1;
        }
    }
}

void Trainer:: setSumSalary(int addedSalary) {
    this->sumSalary = this->sumSalary + addedSalary;
}

int Trainer:: sumUpSalary() {
    return this->sumSalary;
}

bool Trainer:: isSessionIsOrdered() {
    return this->sessionIsOrdered;
}

void Trainer:: openSessionIsOrdered() {
    this->sessionIsOrdered = true;
}

void Trainer:: closeSessionIsOrdered() {
    this->sessionIsOrdered = false;
}

bool Trainer:: isCanShowStatus() {
    return this->canShowStatus;
}

void Trainer:: openCanShowStatus() {
    this->canShowStatus = true;
}
