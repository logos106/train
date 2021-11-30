//modify and submit this file only
#include "pa3.h"
#include <iostream>

using namespace std;
//you are NOT allowed to include any additional library; see FAQ
#define MAX 9000
TrainCar* optimizeArray[MAX];
int optSum = 0, cnt = 0;

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
		pos->next->prev = pos->prev;
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

	if (a == b) return true;

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
	for (size_t i = 0; i < a; i++)
	{
		if (pos)
			pos = pos->next;
		else
			return false;
	}
	first = pos;

	// Find the second
	pos = head;
	for (size_t i = 0; i < b; i++)
	{
		if (pos)
			pos = pos->next;
		else
			return false;
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
	while (pos && pos->next)
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

	int load[CARGO_TYPE_COUNT];
	int maxLoad[CARGO_TYPE_COUNT];
	CarType type[CARGO_TYPE_COUNT];

	// Initialize load
	for (size_t i = 0; i < CARGO_TYPE_COUNT; i++)
	{
		type[i] = HEAD;
		load[i] = 0;
		maxLoad[i] = 0;
	}		
	
	TrainCar* pos;
	pos = head->next;
	int index = 0;
	while (pos)
	{
		// Find the index
		for (int i = 0; i < CARGO_TYPE_COUNT; i++)
		{
			index = i;
			if ((type[i] == HEAD) || (type[i] == pos->type)) break;
		}

		type[index] = pos->type;
		load[index] += pos->load;
		maxLoad[index] += pos->maxLoad;

		pos = pos->next;
	}

	for (size_t i = 0; i < CARGO_TYPE_COUNT; i++)
	{
		if (type[i] == HEAD) break;

		cout << enumToStringMapping[type[i]];
		cout << ":";
		cout << load[i];
		cout << "/";
		cout << maxLoad[i];
		if ((i < CARGO_TYPE_COUNT - 1) && (load[i + 1] != -1))
			cout << ",";
	}

	cout << endl;
}

void divide(const TrainCar* head,  TrainCar* results[CARGO_TYPE_COUNT])
{
	TrainCar* pos, * tmp;
	int index;
	
	// Init the results
	for (size_t i = 0; i < CARGO_TYPE_COUNT; i++)
	{
		results[i] = createTrainHead();
	}

	pos = head->next;
	while (pos)
	{
		// Find the index
		index = 0;
		for (int i = 0; i < CARGO_TYPE_COUNT; i++)
		{
			if (!results[i]) continue;

			index = i; 
			if ((results[i]->next == nullptr) || (results[i]->next->type == pos->type))
				break;
		}

		// Get length of train
		tmp = results[index];
		int len = 0;
		while (tmp)
		{
			len++;
			tmp = tmp->next;
		}

		// Add a car
		addCar(results[index], len, pos->type, pos->maxLoad);

		// Go to the tail
		tmp = results[index];
		while (tmp && tmp->next)
			tmp = tmp->next;
		
		// Change the load of the car
		if (tmp)
			tmp->load = pos->load;
		
		pos = pos->next;
	}
}

void subset_sum(TrainCar* o[], TrainCar* t[], int o_size, int t_size, int sum, int ite, int const target_sum)
{	
	for (int i = ite; i < o_size; i++) {
		t[t_size] = o[i];
		if (sum + o[i]->load > target_sum) continue;
		subset_sum(o, t, o_size, t_size + 1, sum + o[i]->load, i + 1, target_sum);
	}
	
	if (target_sum >= sum && optSum < sum)
	{
		optSum = sum;

		cnt = t_size;
		for (int i = 0; i < t_size; ++i) optimizeArray[i] = t[i];
			
		if (ite < o_size)
			subset_sum(o, t, o_size, t_size - 1, sum - o[ite]->load, ite + 1, target_sum);

		return;
	}
}

TrainCar* optimizeForMaximumPossibleCargos(const TrainCar* head, int upperBound)
{
	TrainCar* pos;
	pos = head->next->prev;
	int length = 0;
	while (pos = pos->next) {
		length++;
	}
	pos = head->next->prev;
	TrainCar* candidates[MAX], * originals[MAX];
	int i = 0;
	originals[0] = pos;
	candidates[0] = nullptr;
	while (pos = pos->next)
	{
		originals[i] = pos;
		candidates[i++] = nullptr;
	}
	subset_sum(originals, candidates, length, 0, 0, 0, upperBound);

	TrainCar* train = createTrainHead(), *p;
	p = train;
	
	for (int i = 0; i < cnt; ++i) {	 
		TrainCar* newCar = new TrainCar;
		 newCar->load = optimizeArray[i]->load;
		 newCar->maxLoad = optimizeArray[i]->maxLoad;
		 newCar->type = optimizeArray[i]->type; 
		 newCar->prev = p;
		 newCar->next = nullptr;
		 p->next = newCar;
		 p = newCar;
	}

	return train;
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
