# include "../include/Customer.h"

Customer:: Customer (std::string c_name, int c_id): name(c_name), id(c_id) {}

Customer:: ~Customer(){}

std::string Customer:: getName() const {
    return name;
}

int Customer:: getId() const {
    return  id;
}


//SweatyCustomer
SweatyCustomer:: SweatyCustomer(std::string name, int id): Customer(name, id) {}

std::vector<int> SweatyCustomer:: order(const std::vector<Workout> &workout_options) {
    std::vector<int> order;
    for (unsigned int i = 0; i < workout_options.size(); i++){
        if (workout_options[i].getType() == CARDIO){
            order.push_back(workout_options[i].getId());
        }
    }
    return order;
}

std::string SweatyCustomer:: toString() const{
    return getName() + ",swt";
}

Customer* SweatyCustomer:: createCustomerType(){
    SweatyCustomer* sweatyCustomerPtr = new SweatyCustomer(*this);
    return sweatyCustomerPtr;
}


//CheapCustomer
CheapCustomer:: CheapCustomer(std::string name, int id): Customer(name,id){}

std::vector<int> CheapCustomer:: order(const std::vector<Workout> &workout_options) {
    std::vector<int> order;
    int cheapest_index = 0;
    for (unsigned int i = 0; i < workout_options.size(); i++){
        if (workout_options[i].getPrice() < workout_options[cheapest_index].getPrice()){
            cheapest_index=i;
        }
    }
    order.push_back(cheapest_index);
    return order;
}

std::string CheapCustomer:: toString() const{
    return getName() + ",chp";
}

Customer* CheapCustomer:: createCustomerType(){
    CheapCustomer* cheapCustomerPtr = new CheapCustomer(*this);
    return cheapCustomerPtr;
}

//HeavyMuscleCustomer
HeavyMuscleCustomer:: HeavyMuscleCustomer(std::string name, int id): Customer(name,id){}

std::vector<int> HeavyMuscleCustomer:: order(const std::vector<Workout> &workout_options) {
    std::vector<int> order;
    for (unsigned int i = 0; i < workout_options.size(); i++){
        if (workout_options[i].getType() == ANAEROBIC){
            order.push_back(workout_options[i].getId());
        }
    }
    sortByPrice(workout_options, order);
    return order;
}

std::string HeavyMuscleCustomer:: toString() const{
    return getName() + ",mcl";
}

Customer* HeavyMuscleCustomer:: createCustomerType(){
    HeavyMuscleCustomer* heavyMuscleCustomerPtr = new HeavyMuscleCustomer(*this);
    return heavyMuscleCustomerPtr;
}

void HeavyMuscleCustomer:: swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}

int HeavyMuscleCustomer:: partition (const std::vector<Workout> &workout_options, std::vector<int> &order, int low, int high){
    int pivot = order[high]; // pivot
    int pivot_price = workout_options[pivot].getPrice();
    int i = (low - 1);  // Index of smaller element
    for (int j = low; j <= high - 1; j++){
        // If current element is higher than or
        // equal to pivot
        int j_price = workout_options[order[j]].getPrice();
        if (j_price >= pivot_price){
            i++;    // increment index of smaller element
            swap(&order[i], &order[j]);
        }
    }
    swap(&order[i + 1], &order[high]);
    return (i + 1);
}

void HeavyMuscleCustomer:: quickSort(const std::vector<Workout> &workout_options, std::vector<int> &order, int low, int high){
    if (low < high){
        // pi is partitioning index, arr[p] is now at right place
        int pi = partition(workout_options,order, low, high);
        // Separately sort elements before
        // partition and after partition
        quickSort(workout_options, order, low, pi - 1);
        quickSort(workout_options, order, pi + 1, high);
    }
}

void HeavyMuscleCustomer:: sortByPrice(const std::vector<Workout> &workout_options, std::vector<int> &order){
    quickSort(workout_options, order, 0, order.size() - 1);
}


//Full Body Customer
FullBodyCustomer:: FullBodyCustomer(std::string name, int id): Customer(name,id){}

std::vector<int> FullBodyCustomer:: order(const std::vector<Workout> &workout_options) {
    std::vector<int> order;
    int cardio = findCheapestOrExpensiveByType(workout_options, CARDIO, true);
    int mixed = findCheapestOrExpensiveByType(workout_options, MIXED, false);
    int anaerobic = findCheapestOrExpensiveByType(workout_options, ANAEROBIC, true);
    if (cardio != -1) {
        order.push_back(cardio);
    }
    if (mixed != -1) {
        order.push_back(mixed);
    }
    if (anaerobic != -1) {
        order.push_back(anaerobic);
    }
    return order;
}

std::string FullBodyCustomer:: toString() const{
    return getName() + ",fbd";
}

Customer* FullBodyCustomer:: createCustomerType(){
    FullBodyCustomer* fullBodyCustomerPtr = new FullBodyCustomer(*this);
    return fullBodyCustomerPtr;
}

/*
 * cheapest = true: find the cheapest
 * else; find the most expensive.
 * returns the cheapest/most expensive exercise of a given type
 * if no exists an exercise, of the given type, the method returns -1
 */
int  FullBodyCustomer:: findCheapestOrExpensiveByType(const std::vector<Workout> &workout_options, WorkoutType type, bool cheapest){
    int index = 0;
    if (cheapest) {
        for (unsigned int i = 0; i < workout_options.size(); i++)
            if (workout_options[i].getType() == type
            && workout_options[i].getPrice() < workout_options[index].getPrice())
                index = i;
    }else{
        for (unsigned int i = 0; i < workout_options.size(); ++i)
            if (workout_options[i].getType() == type
            && workout_options[i].getPrice() > workout_options[index].getPrice())
                index = i;
    }
    return workout_options[index].getType()==type ? index: -1;
}


