#include "../include/Action.h"
#include "../include/Studio.h"

extern Studio* backup;

//BaseAction
BaseAction:: BaseAction():logMsg(),errorMsg(),status(){}

BaseAction:: BaseAction(const BaseAction& other){
    logMsg = other.logMsg;
    errorMsg = other.errorMsg;
    status = other.status;
}
BaseAction:: ~BaseAction(){}

void BaseAction:: complete(){
    this->status = COMPLETED;
}

void BaseAction:: error(std::string errorMsg){
    this->status = ERROR;
    std::cout<< "Error: " + errorMsg <<std::endl;
    this->errorMsg = errorMsg;
}


//Getters
std::string BaseAction:: getLogMsg() const{
    return logMsg;
}
std::string BaseAction:: getErrorMsg() const{
    return errorMsg;
}
ActionStatus BaseAction:: getStatus() const{
    return status;
}


//Setters
void BaseAction:: setLogMsg(const std::string& otherLogMsg){
    logMsg = otherLogMsg;
}
void BaseAction:: setErrorMsg(const std::string& otherErrorMsg){
    errorMsg = otherErrorMsg;
}
void BaseAction:: setStatus(const ActionStatus& otherStatus){
    status = otherStatus;
}


//OpenTrainer
OpenTrainer:: OpenTrainer(int id, std::vector<Customer *> &customersList):
trainerId(id),customers(customersList){}

void OpenTrainer:: act(Studio &studio) {
    int numOfTrainers = studio.getNumOfTrainers();
    Trainer *trainer = studio.getTrainer(trainerId);
    if (numOfTrainers <= trainerId || trainer->isOpen() || customers.empty()) {
        error("Workout session does not exist or is already open");
    }else{
        trainer -> openTrainer();
        for(unsigned int i = 0; i < customers.size(); i++){
            trainer->addCustomer(customers[i]);
        }
        if (!trainer->isCanShowStatus()){
            trainer->openCanShowStatus();
        }
        complete();
    }
    this->logMsg = this->toString();
}

std::string OpenTrainer:: toString() const{
    std::string actionArgs = "";
    for (Customer* customer : customers) {
        if(actionArgs == ""){
            actionArgs = customer->toString();
        }else{
            actionArgs = actionArgs + " " + customer->toString();
        }
    }
    std::string outputString = "open " + std::to_string(this->trainerId) + " " + actionArgs ;
    if(this->getStatus() == COMPLETED){
        outputString = outputString + " Completed";
    } else {
        outputString = outputString + " Error: " + getErrorMsg();
    }
    return outputString;
}

BaseAction* OpenTrainer:: createAction(){
    OpenTrainer* openTrainerPtr = new OpenTrainer(*this);
    return openTrainerPtr;
}

OpenTrainer:: OpenTrainer(const OpenTrainer& other): trainerId(other.trainerId){
    for (auto &customer : other.customers){
        Customer *customerPtr = customer;
        customers.push_back(customerPtr);
    }
    setStatus(other.getStatus());
    setErrorMsg(other.getErrorMsg());
    setLogMsg(other.getLogMsg());
}

OpenTrainer:: OpenTrainer(OpenTrainer&& other): trainerId(std::move(other.trainerId)),
customers(std::move(other.customers)) {}

OpenTrainer:: ~OpenTrainer(){
    customers.clear();
}


//Order
Order:: Order(int id): trainerId(id){}

Order:: Order(const Order& other): BaseAction(other),trainerId(other.trainerId){}

void Order:: act(Studio &studio){
    int numOfTrainers = studio.getNumOfTrainers();
    Trainer *trainer = studio.getTrainer(trainerId);
    if (numOfTrainers <= trainerId) {
        error("Trainer does not exist or is not open");
    }else if (!trainer -> isOpen()) {
        error("Trainer does not exist or is not open");
    }else{
        if(!trainer->isSessionIsOrdered()){
            trainer->openSessionIsOrdered();
            std::vector<Customer*> customers = trainer->getCustomers();
            for (Customer* customer : customers) {
                trainer->order(customer->getId(), customer->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
            }
        }
        std::vector<OrderPair> &orders = trainer->getOrders();
        for(OrderPair order : orders) {
            Workout &workout = order.second;
            Customer *pCustomer = trainer->getCustomer(order.first);
            std::cout<< pCustomer->getName() + " Is Doing " + workout.getName() <<std::endl;
        }
        complete();
    }
    this->logMsg = this->toString();
}

std::string Order:: toString() const{
    std::string outputString = "order " + std::to_string(this->trainerId);
    if(this->getStatus() == COMPLETED){
        outputString = outputString + " Completed";
    } else {
        outputString = outputString + " Error: " + getErrorMsg();
    }
    return outputString;
}

BaseAction* Order:: createAction(){
    Order* orderPtr = new Order (*this);
    return orderPtr;
}

//MoveCustomer
MoveCustomer:: MoveCustomer(int src, int dst, int customerId) : srcTrainer(src),dstTrainer(dst),id(customerId){}
MoveCustomer:: MoveCustomer(const MoveCustomer& other):BaseAction(other),srcTrainer(other.srcTrainer),dstTrainer(other.dstTrainer),id(other.id) {}
void MoveCustomer:: act(Studio &studio){
    int numOfTrainers = studio.getNumOfTrainers();
    Trainer *trainerSrc = studio.getTrainer(srcTrainer);
    Trainer *trainerDst = studio.getTrainer(dstTrainer);
    if (numOfTrainers <= srcTrainer || numOfTrainers <= dstTrainer) {
        error("Cannot move customer");
    }else if (!trainerSrc -> isOpen() || !trainerDst -> isOpen()) {
        error("Cannot move customer");
    }else if (trainerDst -> getCapacity() < (int)trainerDst->getCustomers().size() + 1) {
        error("Cannot move customer");
    }else if (trainerSrc->getCustomer(id) == nullptr){
        error("Cannot move customer");
    }else if(!trainerSrc->isSessionIsOrdered() || !trainerDst->isSessionIsOrdered()){
        error("Cannot move customer");
    }
    else{
        trainerDst->addCustomer(trainerSrc->getCustomer(id));
        trainerSrc->removeCustomer(id);

        std::vector<OrderPair> &orders = trainerSrc->getOrders();
        std::vector<int> workout_ids;
        std::vector<Workout> workout_options;
        std::vector<Workout> &workoutOptions = studio.getWorkoutOptions();
        for (unsigned int i = 0 ; i < orders.size() ; i++) {
            if (orders[i].first == id) {
                for (Workout workout : workoutOptions) {
                    if (workout.getName() == orders[i].second.getName()) {
                        workout_ids.push_back(workout.getId());
                    }
                }
            }
        }
        trainerDst->order(id, workout_ids, workoutOptions);
        trainerSrc->removeOrder(id);

        if(trainerSrc->getCustomers().size() == 0){
            Close* close = new Close (srcTrainer);
            close->act(studio);
            studio.addAction(*close);
        }
        complete();
    }
    this->logMsg = this->toString();
}

std::string MoveCustomer:: toString() const{
    std::string outputString = "move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id);
    if(this->getStatus() == COMPLETED){
        outputString = outputString + " Completed";
    } else {
        outputString = outputString + " Error: " + getErrorMsg();
    }
    return outputString;
}

BaseAction* MoveCustomer:: createAction(){
    MoveCustomer* moveCustomerPtr = new MoveCustomer (*this);
    return moveCustomerPtr;
}

//Close
Close:: Close(int id): trainerId(id) {}

Close:: Close(const Close& other): BaseAction(other), trainerId(other.trainerId){}

void Close:: act(Studio &studio){
    int numOfTrainers = studio.getNumOfTrainers();
    Trainer *trainer = studio.getTrainer(trainerId);
    if (numOfTrainers <= trainerId) {
        error("Workout session does not exist");
    }else if (!trainer -> isOpen()) {
        error("Workout session is not open");
    }else{
        if(!trainer->isSessionIsOrdered()){ //to check
            trainer->closeSessionIsOrdered();
        }
        std::vector<Customer*> customersList = trainer->getCustomers();
        int salarySession = trainer->getSalary(); // Current salary
        trainer->setSumSalary(salarySession); // Sum salary

        for(Customer* customer: customersList) {
            int id = customer->getId();
            trainer->deleteCustomer(id);
        }
        trainer->getCustomers().clear();
        trainer->deleteOrders();
        std::string outputString;
        outputString = "Trainer " + std::to_string(this->trainerId) + " closed. Salary " + std::to_string(trainer->sumUpSalary()) + "NIS";
        std::cout<< outputString <<std::endl;
        trainer->closeTrainer();
        trainer->closeSessionIsOrdered();
        complete();
    }
    this->logMsg = this->toString();
}

std::string Close:: toString() const{

    std::string outputString = "close " + std::to_string(trainerId);
    if(this->getStatus() == COMPLETED){
        outputString = outputString + " Completed";
    } else {
        outputString = outputString + " Error: " + getErrorMsg();
    }
    return outputString;
}

BaseAction* Close:: createAction(){
    Close* closePtr = new Close (*this);
    return closePtr;
}

//CloseAll
CloseAll:: CloseAll() {}

void CloseAll:: act(Studio &studio) {
    for (int i = 0; i < studio.getNumOfTrainers(); i++) {
        Trainer *trainer = studio.getTrainer(i);
        if (trainer->isOpen()){
            Close close(i);
            close.act(studio);
        }
    }
    complete();
    this->logMsg = this->toString();
}

std::string CloseAll:: toString() const{
    return "closeall Completed";
}

BaseAction* CloseAll:: createAction(){
    CloseAll* closeAllPtr = new CloseAll(*this);
    return closeAllPtr;
}


//PrintWorkoutOptions
PrintWorkoutOptions:: PrintWorkoutOptions() {}

void PrintWorkoutOptions:: act(Studio &studio) {
    for(Workout workout : studio.getWorkoutOptions()){
        std::string workoutType;
        if(workout.getType() == ANAEROBIC){
            workoutType = "Anaerobic";
        }else if(workout.getType() == MIXED){
            workoutType = "Mixed";
        }else{
            workoutType = "Cardio";
        }
        std::cout<< workout.getName() + ", " + workoutType + ", " + std::to_string(workout.getPrice()) <<std::endl;
    }
    complete();
    this->logMsg = this->toString();
}

std::string PrintWorkoutOptions:: toString() const{
    return "workout_options Completed";
}

BaseAction* PrintWorkoutOptions:: createAction(){
    PrintWorkoutOptions* printWorkoutOptionsPtr = new PrintWorkoutOptions(*this);
    return printWorkoutOptionsPtr;
}


//PrintTrainerStatus
PrintTrainerStatus:: PrintTrainerStatus(int id): trainerId(id){}

PrintTrainerStatus::PrintTrainerStatus(const PrintTrainerStatus &other) : BaseAction(other),
trainerId(other.trainerId) {}

void PrintTrainerStatus:: act(Studio &studio){
    if(studio.getTrainer(trainerId)->isCanShowStatus()){
        if(studio.getTrainer(trainerId)->isOpen()){
            std::cout<< "Trainer " + std::to_string(trainerId) + " status: open" <<std::endl;
            std::cout<< "Customers:" <<std::endl;
            for (Customer* customer : studio.getTrainer(trainerId)->getCustomers()) {
                std::cout<< std::to_string(customer->getId()) + " " + customer->getName() <<std::endl;
            }
            std::cout<< "Orders:" <<std::endl;
            for(OrderPair order : studio.getTrainer(trainerId)->getOrders()) {
                if (order.first >= 0){
                    std::cout<< order.second.getName() + " " + std::to_string(order.second.getPrice()) + "NIS " +   std::to_string(order.first) <<std::endl;
                }
            }
            std::cout<< "Current Trainer’s Salary: " + std::to_string(studio.getTrainer(trainerId)->getSalary()) + "NIS" <<std::endl;
        } else {
            std::cout<< "Trainer " + std::to_string(trainerId) + " status: closed" <<std::endl;
        }
    }
    complete();
    this->logMsg = this->toString();
}

std::string PrintTrainerStatus:: toString() const{
    return  "status  " + std::to_string(trainerId) + " Completed";
}

BaseAction* PrintTrainerStatus:: createAction(){
    PrintTrainerStatus* printTrainerStatusPtr = new PrintTrainerStatus(*this);
    return printTrainerStatusPtr;
}

//PrintActionsLog
PrintActionsLog:: PrintActionsLog(){}

void PrintActionsLog:: act(Studio &studio){
    for(unsigned int i = 0; i < studio.getActionsLog().size(); i++) {
        std::cout<< studio.getActionsLog()[i]->getLogMsg() <<std::endl;
    }
    complete();
    this->logMsg = this->toString();
}

std::string PrintActionsLog:: toString() const{
    return "log Completed";
}

BaseAction* PrintActionsLog:: createAction(){
    PrintActionsLog* printActionsLogPtr = new PrintActionsLog(*this);
    return printActionsLogPtr;
}


//BackupStudio
BackupStudio:: BackupStudio(){}

void BackupStudio:: act(Studio &studio){

    if(backup!= nullptr){
        delete backup;
    }
    this->logMsg = this->toString();
    complete();
    backup = new Studio(studio);


}

std::string BackupStudio:: toString() const{
    return "backup Completed";
}

BaseAction* BackupStudio:: createAction(){
    BackupStudio* backupStudioPtr = new BackupStudio(*this);
    return backupStudioPtr;
}


//RestoreStudio
RestoreStudio:: RestoreStudio(){}

void RestoreStudio:: act(Studio &studio){
    if(backup == nullptr){
        error("no backup available");
    } else{
        studio = *backup;
        complete();
    }
    this->logMsg = this->toString();
}

std::string RestoreStudio:: toString() const{
    return "restore Completed";
}

BaseAction* RestoreStudio:: createAction(){
    RestoreStudio* restoreStudio = new RestoreStudio(*this);
    return restoreStudio;
}