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
	TrainCar *pos, *first, *second, *tmp;
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

	tmp = new TrainCar; tmp->type = first->type; tmp->load = first->load; tmp->maxLoad = first->maxLoad;
	first->type = second->type; second->type = tmp->type;
	first->load = second->load; second->load = tmp->load;
	first->maxLoad = second->maxLoad; second->maxLoad = tmp->maxLoad;
	

	// // Swap them
	// if (first && second && first->prev && second->prev)
	// {
	// 	tmp = first->prev;
	// 	first->prev = second->prev;
	// 	second->prev = tmp;
	// }
	// else
	// 	return false;

	// if (first && second && first->next && second->next)
	// {
	// 	tmp = first->next;
	// 	first->next = second->next;
	// 	second->next = tmp;
	// }
	// else
	// 	return false;

	return true;	
}

void sortTrain(TrainCar* head, bool ascending)
{
	TrainCar* pos, * first, * second, * tmp;
	int len;

	// Get the length
	pos = head;
	len = 0;
	while (pos)
	{
		len++;
		pos = pos->next;
	}

	for (size_t i = 0; i < len - 1; i++)
	{
		pos = head;
		while (pos->next && pos->next->next)
		{
			first = pos->next;
			second = pos->next->next;

			if ((ascending && first->load > second->load) || (!ascending && first->load < second->load))
			{
				// Swap them
				if (first->prev && second->prev)
				{
					tmp = first->prev;
					first->prev = second->prev;
					second->prev = tmp;
				}

				if (first->next && second->next)
				{
					tmp = first->next;
					first->next = second->next;
					second->next = tmp;
				}
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
			if (pos->maxLoad - pos->load <= amount)
				pos->load += amount;
			else
			{
				pos->load = pos->maxLoad;
				rest -= pos->maxLoad - pos->load;
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
		if (pos->type == type)
		{
			// Unload
			if (pos->load > 0)
			{
				rest -= pos->load;
				pos->load = 0;
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
	while (pos)
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
}

void divide(const TrainCar* head,  TrainCar* results[CARGO_TYPE_COUNT])
{
	TrainCar* pos, * tmp, * newTrain;
	
	pos = head->next;
	while (pos)
	{
		if (!results[pos->type])
		{
			newTrain = createTrainHead();
			addCar(newTrain, 1, pos->type, pos->maxLoad);
			newTrain->next->load = pos->load;

			results[pos->type] = newTrain;
		}
		else
		{
			// Get length of train
			tmp = results[pos->type];
			int len = 0;
			while (tmp)
			{
				len++;
				tmp = tmp->next;
			}

			// Add a car
			addCar(results[pos->type], len, pos->type, pos->maxLoad);

			// Go to the tail
			tmp = results[pos->type];
			while (tmp->next)
				tmp = tmp->next;
			
			// Change the load of the car
			tmp->load = pos->load;
		}

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
