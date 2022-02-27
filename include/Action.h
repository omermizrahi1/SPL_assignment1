#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;


class BaseAction{
public:
    BaseAction(const BaseAction& other);
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio) = 0;
    virtual std::string toString() const = 0;
    virtual ~BaseAction();
    std::string getLogMsg() const;
    virtual BaseAction* createAction() =0;
    void setStatus(const ActionStatus &otherStatus);
    void setErrorMsg(const std::string &_errorMsg);
    void setLogMsg(const std::string &otherLogMsg);

protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
    std::string logMsg;

private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer*> &customersList);
    void act(Studio &studio);
    std::string toString() const;
    OpenTrainer(const OpenTrainer& other);
    OpenTrainer(OpenTrainer&& other);
    ~OpenTrainer();
    BaseAction* createAction();
private:
    const int trainerId;
    std::vector<Customer*> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    Order(const Order& other);
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* createAction();
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    MoveCustomer(const MoveCustomer& other);
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* createAction();
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    Close(const Close& other);
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* createAction();
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* createAction();
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* createAction();
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    PrintTrainerStatus(const PrintTrainerStatus& other);
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* createAction();
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* createAction();
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* createAction();
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* createAction();
private:
};


#endif