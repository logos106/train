//modify and submit this file only
#include "pa3.h"
#include <iostream>
using namespace std;
//you are NOT allowed to include any additional library; see FAQ

TrainCar* createTrainHead()
{
    TrainCar* head = new TrainCar;
    head->type = HEAD; head->load = 0; head->maxLoad = 0; head->prev = head->next = nullptr;
}

int getLength(TrainCar* head)
{
    int cnt = 1;
    TrainCar *curCar = head;

    while (true)
    {
        if (curCar->next)
            cnt ++;
        else
            break;
        curCar = curCar->next;
    }
    return cnt;
}

TrainCar* getCarAtPosition(TrainCar* head, int position)
{
    TrainCar* curCar = head;
    int cnt = 0;

    while (true)
    {
        if (cnt == position)
            return curCar;
        if (curCar->next)
            cnt ++;
        else
            break;
        curCar = curCar->next;
    }
    return head;
}

TrainCar* getPrevCar(TrainCar* head)
{
    TrainCar* curCar = head;
    while (true)
    {
        if (curCar->next)
            curCar = curCar->next;
        else
            return curCar;
    }
    return head;
}

bool addCar(TrainCar* head, int position, CarType type, int maxLoad)
{
    // Get length of train
    int length = getLength(head);

    // CargoType is 'HEAD'
    // position is not larger than 0
    // position is larger than current length
    if (type == HEAD || position <= 0 || position > length || maxLoad <= 0)
        return false;
    if (position <= length)
    {
        TrainCar *carAt = getCarAtPosition(head, position);
        carAt->load = 0; carAt->maxLoad = maxLoad;
        return true;
    }

    // Add new car to train
    TrainCar *car, *prev;
    car = new TrainCar;

    prev = head;

    while (true)
    {
        cout << "????????" << endl;
        if (prev->next)
            prev = prev->next;
        else
            break;
    }

    car->type = type; car->load = 0; car->maxLoad = maxLoad; car->prev = prev; car->next = nullptr;
    prev->next = car;

    return true;
}

bool deleteCar(TrainCar* head, int position)
{
}

bool swapCar(TrainCar* head, int a, int b)
{
}

void sortTrain(TrainCar* head, bool ascending)
{
}

bool load(TrainCar* head, CarType type, int amount)
{
}

bool unload(TrainCar* head, CarType type, int amount)
{
}

void printCargoStats(const TrainCar* head)
{
}

void divide(const TrainCar* head,  TrainCar* results[CARGO_TYPE_COUNT])
{
}

TrainCar* optimizeForMaximumPossibleCargos(const TrainCar* head, int upperBound)
{
}

void deallocateTrain(TrainCar* head)
{
}
