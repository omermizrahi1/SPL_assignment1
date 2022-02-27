#include "../include/Studio.h"
#include <fstream>

using namespace std;

// Constructors
Studio:: Studio():customerIdCounter(0), open(false), trainers(), workout_options(), actionsLog(){}

Studio::Studio(const std::string &configFilePath) {
    fstream fileSource;
    string newLine;
    int notEmptyLineCounter = 0;
    int workoutIdCounter = 0;
    string delimiter = ",";
    fileSource.open(configFilePath, ifstream::in);
    while (getline(fileSource, newLine)) {
        if (isNotEmptyLine(newLine)) {
            if (notEmptyLineCounter == 0) {
                notEmptyLineCounter++;
            } else if (notEmptyLineCounter == 1) {
                vector<string> trainersCapacityVector = split(newLine, delimiter);
                for (auto trainerCapacity : trainersCapacityVector) {
                    trainers.push_back(new Trainer(stoi(trainerCapacity)));
                }
                notEmptyLineCounter++;
            } else {
                vector<string> workoutOptions = split(newLine, delimiter);
                removeSpace(workoutOptions[0]);
                removeSpace(workoutOptions[1]);
                string type = workoutOptions[1];
                WorkoutType workoutType = getWorkoutType(type);
                string workoutName = workoutOptions[0];
                int workoutPrice = stoi(workoutOptions[2]);
                Workout workout = Workout(workoutIdCounter, workoutName, workoutPrice, workoutType);
                workoutIdCounter++;
                workout_options.push_back(workout);
            }
        }
    }
    fileSource.close();
}

bool Studio:: isNotEmptyLine(std::string newLine) {
    return newLine != " " &&
    newLine != "\t" &&
    newLine != "\n" &&
    newLine != "\r" &&
    newLine != "\0" &&
    newLine.at(0) != '#';
}

vector<string> Studio:: split(std::string line, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;
    if (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n' || line[line.size() - 1] == '\t' || line[line.size() - 1] == '\0') ) {
        line.erase(line.size() - 1);
    }
    while ((pos_end = line.find (delimiter, pos_start)) != string::npos) {
        token = line.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    res.push_back (line.substr (pos_start));
    return res;
}

WorkoutType Studio:: getWorkoutType(std::string workoutType) {
    std::string tempStr = "";
    for (unsigned int i = 0; i < workoutType.length(); ++i){
        tempStr += toupper(workoutType[i]);
    }
    if (tempStr.compare("ANAEROBIC") == 0) {
        return ANAEROBIC;
    } else if (tempStr.compare("MIXED") == 0) {
        return MIXED;
    }else{
        return CARDIO;
    }
}

std::string Studio:: lowerConversion(std::string& inStr) {
    std::string outStr = "";
    for (unsigned int i = 0; i < inStr.length(); ++i){
        outStr += tolower(inStr[i]);
    }
    return outStr;
}

void Studio:: removeSpace(std::string& tempString){
    if (tempString[0] == ' '){
        tempString = tempString.substr(1,tempString.length()-1);
    }
    if (tempString[tempString.length()-1] == ' '){
        tempString = tempString.substr(0,tempString.length()-2);
    }
}


Studio::Studio(const Studio& other): customerIdCounter(other.customerIdCounter), open(other.open){
    workout_options = copyWorkouts(other.workout_options);
    trainers = copyTrainers(other.trainers);
    actionsLog = copyActions(other.actionsLog);
}

Studio::Studio(Studio &&other) : customerIdCounter(std::move(other.customerIdCounter)), open(std::move(other.open)),
trainers(std::move(other.trainers)),
workout_options(std::move(other.workout_options)),
actionsLog(std::move(other.actionsLog)) {}

Studio:: ~Studio() {
    customerIdCounter = 0;
    open = NULL;
    for (auto actionPtr : actionsLog) {
        delete actionPtr;
    }
    actionsLog.clear();
    workout_options.clear();
    for (auto trainerPtr : trainers) {
        delete trainerPtr;
    }
    trainers.clear();

}

Studio& Studio:: operator=(const Studio &other) {
    if (this == &other){
        return *this;
    }
    customerIdCounter = other.customerIdCounter;
    open = other.open;
    for (auto trainerPtr : trainers) {
        delete trainerPtr;
    }
    trainers.clear();
    trainers = copyTrainers(other.trainers);
    workout_options.clear();
    workout_options = copyWorkouts(other.workout_options);
    for (auto actionPtr : actionsLog) {
        delete actionPtr;
    }
    actionsLog.clear();
    actionsLog = copyActions(other.actionsLog);
    return *this;
}

Studio& Studio:: operator=(Studio &&other){
    if (this != &other){
        customerIdCounter = std::move(other.customerIdCounter);
        open = std::move(other.open);
        trainers = std::move(other.trainers);
        workout_options = std::move(other.workout_options);
        actionsLog = std::move(other.actionsLog);
    }
    return *this;
}

std::vector<Workout> Studio:: copyWorkouts(const std::vector<Workout>& other){
    std::vector<Workout> workouts;
    for (Workout workout : other){
        workouts.push_back(workout);
    }
    return workouts;
}

std::vector<Trainer*> Studio:: copyTrainers(const vector<Trainer *> &other_trainers) {
    std::vector<Trainer *> trainers;
    for (auto trainerPtr : other_trainers) {
        trainers.push_back(new Trainer(*trainerPtr));
    }
    return trainers;
}

std::vector<BaseAction*> Studio:: copyActions(const std::vector<BaseAction*> &otherActions) {
    std::vector<BaseAction*> actions;
    for (auto actionPtr : otherActions) {
        actions.push_back(actionPtr->createAction());
    }
    return actions;
}


void Studio:: start() {

    std::cout << "Studio is now open!" << std::endl;
    open = true;
    string actionToExecute;
    getline(cin, actionToExecute);

    customerIdCounter = 0;
    while (actionToExecute != "closeall") {
        vector<std::string> actionDetails = split(actionToExecute, " ");
        if (isOpenTrainerAction(actionDetails[0]) && actionDetails.size() > 1 ) { // OpenTrainer
            int trainerId = stoi(actionDetails[1]);
            Trainer* trainer = getTrainer(trainerId);
            vector<Customer*> customers;
            if (actionDetails.size() >= 3 ){ // Check that the trainer has customers
                if (!trainer->isOpen()){
                    for (unsigned int i = 2; i < (unsigned int)trainer->getCapacity()  + 2  && i<actionDetails.size()  ; i++) {
                        vector<std::string> customerDetails = split(actionDetails[i], ",");
                        string customerType = customerDetails[1];
                        if (customerType == "swt") {
                            customers.push_back(new SweatyCustomer(customerDetails[0], customerIdCounter));
                            customerIdCounter++;
                        } else if (customerType == "chp") {
                            customers.push_back(new CheapCustomer(customerDetails[0], customerIdCounter));
                            customerIdCounter++;
                        } else if (customerType == "mcl") {
                            customers.push_back(new HeavyMuscleCustomer(customerDetails[0], customerIdCounter));
                            customerIdCounter++;
                        } else {
                            customers.push_back(new FullBodyCustomer(customerDetails[0], customerIdCounter));
                            customerIdCounter++;
                        }
                    }
                }
                BaseAction* openTrainerPtr = new OpenTrainer(trainerId, customers);
                openTrainerPtr->act(*this);
                actionsLog.push_back(openTrainerPtr);
            }

        } else if (isOrderAction(actionDetails[0]) && actionDetails.size() > 1) { // Order
            int trainerId = stoi(actionDetails[1]);
            BaseAction* orderPtr = new Order(trainerId);
            orderPtr->act(*this);
            actionsLog.push_back(orderPtr);

        } else if (isMoveAction(actionDetails[0]) && actionDetails.size() > 1) { // MoveAction
            BaseAction*  moveCustomerPtr = new MoveCustomer(stoi(actionDetails[1]), stoi(actionDetails[2]), stoi(actionDetails[3]));
            moveCustomerPtr->act(*this);
            actionsLog.push_back(moveCustomerPtr);

        }else if (isCloseAction(actionDetails[0]) && actionDetails.size() > 1) { // Close
            int trainerId = stoi(actionDetails[1]);
            BaseAction* closePtr = new Close(trainerId);
            closePtr->act(*this);
            actionsLog.push_back(closePtr);

        }else if (isPrintWorkoutOptionsAction(actionDetails[0])) { // PrintWorkoutOptions
            BaseAction* printWorkoutOptionsPtr = new PrintWorkoutOptions;
            printWorkoutOptionsPtr->act(*this);
            actionsLog.push_back(printWorkoutOptionsPtr);

        }else if (isPrintTrainerStatusAction(actionDetails[0])) { // PrintTrainerStatus
            int trainerId = stoi(actionDetails[1]);
            if ((unsigned int)trainerId<trainers.size()) {
                BaseAction *printTrainerStatusPtr = new PrintTrainerStatus(trainerId);
                printTrainerStatusPtr->act(*this);
                actionsLog.push_back(printTrainerStatusPtr);
            }

        }else if (isPrintActionsLogAction(actionDetails[0])) { // PrintActionsLog
            BaseAction* printActionsLogPtr = new PrintActionsLog();
            printActionsLogPtr->act(*this);
            actionsLog.push_back(printActionsLogPtr);

        }else if (isBackupStudioAction(actionDetails[0])) { // BackupStudio
            BaseAction* backupStudioPtr = new BackupStudio();
            backupStudioPtr->act(*this);
            actionsLog.push_back(backupStudioPtr);

        }else if(isRestoreStudioAction(actionDetails[0])) { // RestoreStudio
            BaseAction* restoreStudioPtr = new RestoreStudio();
            restoreStudioPtr->act(*this);
            actionsLog.push_back(restoreStudioPtr);
        }
        getline(cin, actionToExecute);
    }

    CloseAll closeAll;
    closeAll.act(*this);
}


void Studio:: addAction(BaseAction& action){
    actionsLog.push_back(&action);
}

//Getters
int Studio:: getNumOfTrainers() const {
    return trainers.size();;
}

Trainer* Studio:: getTrainer(int tid) {
    if (tid >= getNumOfTrainers()){
        return nullptr;
    }else{
        return trainers[tid];
    }
}

const std::vector<BaseAction*>& Studio:: getActionsLog() const {
    return actionsLog;
}

std::vector<Workout>& Studio:: getWorkoutOptions() {
    return workout_options;
}




//
bool Studio:: isOpenTrainerAction(std::string actionType) {
    if(lowerConversion(actionType).compare("open") == 0){
        return true;
    }
    return false;
}

bool Studio:: isOrderAction(std::string actionType) {
    if(lowerConversion(actionType).compare("order") == 0){
        return true;
    }
    return false;
}

bool Studio:: isMoveAction(std::string actionType) {
    if(lowerConversion(actionType).compare("move") == 0){
        return true;
    }
    return false;
}

bool Studio:: isCloseAction(std::string actionType){
    if(lowerConversion(actionType).compare("close") == 0){
        return true;
    }
    return false;
}

bool Studio:: isPrintWorkoutOptionsAction(std::string actionType){
    if(lowerConversion(actionType).compare("workout_options") == 0){
        return true;
    }
    return false;
}

bool Studio:: isPrintTrainerStatusAction(std::string actionType){
    if(lowerConversion(actionType).compare("status") == 0){
        return true;
    }
    return false;
}

bool Studio:: isPrintActionsLogAction(std::string actionType){
    if(lowerConversion(actionType).compare("log") == 0){
        return true;
    }
    return false;
}

bool Studio:: isBackupStudioAction(std::string actionType){
    if(lowerConversion(actionType).compare("backup") == 0){
        return true;
    }
    return false;
}

bool Studio:: isRestoreStudioAction(std::string actionType){
    if(lowerConversion(actionType).compare("restore") == 0){
        return true;
    }
    return false;
}