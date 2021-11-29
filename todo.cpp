//modify and submit this file only
#include "pa3.h"
#include <iostream>

using namespace std;
//you are NOT allowed to include any additional library; see FAQ

TrainCar* createTrainHead()
{
    TrainCar* head = new TrainCar;

    head->type = HEAD; 
	head->load = 0; 
	head->maxLoad = 0; 
	head->prev = head->next = nullptr;

	return head;
}

bool addCar(TrainCar* head, int position, CarType type, int maxLoad)
{
	TrainCar *pos;
	int len;

	// Get length of train
	pos = head;
	len = 0; 
	while (pos)
	{	
		len++;
		pos = pos->next;
	}		

	// Fails when
	if (type == HEAD || position <= 0 || position > len || maxLoad <= 0)
		return false;

	// Update type and maxLoad if exists
	if (position < len)
	{
		pos = head;
		for (size_t i = 0; i < position; i++)
		{
			if (pos)
				pos = pos->next;
			else
				return false;
		}
		
		if (pos)
		{
			pos->type = type;
			pos->maxLoad = maxLoad;
		}
		
		return true;
	}

	// Add new car to train
	TrainCar *newCar, *last;
	newCar = new TrainCar;

	// Get the last
	pos = head;
	while (pos->next)
		pos = pos->next;
	last = pos;

	// Set the attributes of the car
	newCar->type = type; 
	newCar->load = 0; 
	newCar->maxLoad = maxLoad; 
	newCar->prev = last;
	newCar->next = nullptr;
	last->next = newCar;
	
	return true;
}

bool deleteCar(TrainCar* head, int position)
{
	TrainCar *pos;
	int len;

	// Get the length
	pos = head;
	len = 0;
	while (pos)
	{
		len++;
		pos = pos->next;
	}

	// Check the position
	if (position < 1 || position >= len)
		return false;

	// Find the car
	pos = head;
	for (size_t i = 0; i < position; i++)
	{
		if (pos)
			pos = pos->next;
		else
			return false;
	}

	// Remove it
	if (pos && pos->prev)
	{
		pos->prev->next = pos->next;
		delete pos;
	}

	return true;
}

bool swapCar(TrainCar* head, int a, int b)
{
	TrainCar *pos, *first, *second;
	int len, load, maxLoad;
	CarType type;

	// Get the length
	pos = head;
	len = 0;
	while (pos)
	{
		len++;
		pos = pos->next;
	}

	// Check the position
	if (a < 1 || a >= len || b < 1 || b >= len)
		return false;

	// Find the first
	pos = head;
	int index = 0;
	while(pos = pos->next)
	{
		index ++;
		if (index == a) break;
	}
	first = pos;

	// Find the second
	pos = head;
	index = 0;
	while(pos = pos->next)
	{
		index ++;
		if (index == b) break;
	}
	second = pos;

	// Swap them
	load = first->load;
	maxLoad = first->maxLoad;
	type = first->type;

	first->load = second->load;
	first->maxLoad = second->maxLoad;
	first->type = second->type;

	second->load = load;
	second->maxLoad = maxLoad;
	second->type = type;

	return true;	
}

void sortTrain(TrainCar* head, bool ascending)
{
	TrainCar* pos, * first, * second;
	int len;

	// Get the length
	pos = head;
	len = 0;
	while (pos)
	{
		len++;
		pos = pos->next;
	}
	len--;

	for (size_t i = 0; i < len; i++)
	{
		pos = head;
		while (pos->next && pos->next->next)
		{
			first = pos->next;
			second = pos->next->next;

			if ((ascending && first->load > second->load) || (!ascending && first->load < second->load))
			{
				// Swap them
				int load, maxLoad;
				CarType type;

				load = first->load;
				maxLoad = first->maxLoad;
				type = first->type;

				first->load = second->load;
				first->maxLoad = second->maxLoad;
				first->type = second->type;

				second->load = load;
				second->maxLoad = maxLoad;
				second->type = type;
			}

			pos = pos->next;
		}
	}
}

bool load(TrainCar* head, CarType type, int amount)
{
	TrainCar *pos;
	int rest;

	pos = head;
	rest = amount;
	while (pos)
	{
		if (pos->type == type)
		{
			// Load
			if (pos->maxLoad - pos->load >= rest)
				pos->load += rest;
			else
			{
				rest -= pos->maxLoad - pos->load;
				pos->load = pos->maxLoad;
			}
		}

		pos = pos->next;
	}

	if (rest > 0)
		return false;

	return true;
}

bool unload(TrainCar* head, CarType type, int amount)
{
	TrainCar* pos;
	int rest;

	// Go to the tail
	pos = head;
	while (pos->next)
		pos = pos->next;

	rest = amount;
	while (pos && pos->type != HEAD)
	{
		if (pos->type == type && pos->load > 0)
		{
			// Unload
			if (pos->load <= rest)
			{
				rest -= pos->load;
				pos->load = 0;
			}
			else
			{
				pos->load -= rest;
				rest = 0;
				break;
			}
		}

		pos = pos->prev;
	}

	if (rest > 0)
		return false;

	return true;
}

void printCargoStats(const TrainCar* head)
{
	const char enumToStringMapping[6][8] = { "HEAD", "OIL", "COAL", "WOOD", "STEEL", "SUGAR" };

	TrainCar* pos;

	pos = head->next;
	while (pos->next)
	{
		cout << enumToStringMapping[pos->type];
		cout << ":";
		cout << pos->load;
		cout << "/";
		cout << pos->maxLoad;
		cout << ",";

		pos = pos->next;
	}

	// The last car
	cout << enumToStringMapping[pos->type];
	cout << ":";
	cout << pos->load;
	cout << "/";
	cout << pos->maxLoad;
	cout << endl;
}

void divide(const TrainCar* head,  TrainCar* results[CARGO_TYPE_COUNT])
{
	TrainCar* pos, * tmp;
	
	// Init the results
	for (size_t i = 0; i < CARGO_TYPE_COUNT; i++)
	{
		results[i] = createTrainHead();
	}

	pos = head->next;
	while (pos)
	{
		// Get length of train
		tmp = results[pos->type - 1];
		int len = 0;
		while (tmp)
		{
			len++;
			tmp = tmp->next;
		}

		// Add a car
		addCar(results[pos->type - 1], len, pos->type, pos->maxLoad);

		// Go to the tail
		tmp = results[pos->type - 1];
		while (tmp->next)
			tmp = tmp->next;
		
		// Change the load of the car
		tmp->load = pos->load;
		
		pos = pos->next;
	}

}

TrainCar* optimizeForMaximumPossibleCargos(const TrainCar* head, int upperBound)
{
	return nullptr;
}

void deallocateTrain(TrainCar* head)
{
	TrainCar* pos, *tmp;

	pos = head;
	while (pos)
	{
		tmp = pos;
		pos = pos->next;

		delete tmp;
	}
}
