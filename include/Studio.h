#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

class Studio{
public:

    Studio();
    Studio(const std::string &configFilePath);
    Studio(const Studio& other);
    Studio(Studio&& other);
    ~Studio();
    Studio& operator=(const Studio& other);
    Studio& operator=(Studio&& other);

    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();

    void addAction(BaseAction & action);

private:
    int customerIdCounter;
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;


    //Auxiliary functions
    bool isNotEmptyLine(std::string basicString);
    std::vector<std::string> split(std::string line, std::string delimiter);
    WorkoutType getWorkoutType(std::string workoutType);

    bool isOpenTrainerAction(std::string actionType);
    bool isOrderAction(std::string action);
    bool isMoveAction(std::string actionType);
    bool isCloseAction(std::string actionType);
    bool isPrintWorkoutOptionsAction(std::string actionType);
    bool isPrintTrainerStatusAction(std::string actionType);
    bool isPrintActionsLogAction(std::string actionType);
    bool isBackupStudioAction(std::string actionType);
    bool isRestoreStudioAction(std::string actionType);

    std::string lowerConversion(std::string& inStr);
    void removeSpace(std::string& tempString);

    std::vector<Workout> copyWorkouts(const std::vector<Workout>& other);
    std::vector<Trainer*> copyTrainers(const std::vector<Trainer*> & other_trainers);
    std::vector<BaseAction*> copyActions(const std::vector<BaseAction*> & otherActions);

};

#endif
