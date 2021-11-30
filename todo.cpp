//modify and submit this file only
#include "pa3.h"
#include <iostream>

using namespace std;
//you are NOT allowed to include any additional library; see FAQ

#define MAX 9000
TrainCar* sub_array[MAX];
int subSum = 0, sub_size = 0;

TrainCar* createTrainHead()
{
    TrainCar* head = new TrainCar;

    head->type = HEAD; 
	head->load = 0; 
	head->maxLoad = 0; 
	head->prev = head->next = nullptr;

	return head;
}

int getSize(TrainCar* head)
{	
	TrainCar* temp = head;
	int size = 0;
	do
	{
		size++;
		temp = temp->next;
	} while (temp);

	return size;
}

bool addCar(TrainCar* head, int position, CarType type, int maxLoad)
{
	int size = getSize(head);	

	if (type == HEAD)
		return false;

	if (position <= 0 || position > size)
		return false;

	if (maxLoad <= 0)
		return false;

	// Create a new cargo
	TrainCar *newOne = new TrainCar;

	// Get the tail
	while (head->next)
		head = head->next;
	TrainCar *tail = head;

	// Link it to the tail
	tail->next = newOne;

	// Set attributes
	newOne->type = type;
	newOne->load = 0;
	newOne->maxLoad = maxLoad;
	newOne->prev = tail;
	newOne->next = nullptr;	
	
	return true;
}

bool deleteCar(TrainCar* head, int position)
{
	int size = getSize(head);

	if (position < 1 || position >= size)
		return false;

	// Go to the position and delete it
	int p = 0;
	TrainCar* temp = head->next;
	do
	{
		if (p++ == position)
		{
			if (temp->next)
			{
				temp->next->prev = temp->prev;
				temp->prev->next = temp->next;
			}

			delete temp;
			break;
		}
	} while (temp = temp->next);

	return true;
}

bool swapCar(TrainCar* head, int a, int b)
{
	if (a == b) return true;

	int size = getSize(head);

	if (a < 1 || a >= size)
		return false;

	if (b < 1 || b >= size)
		return false;

	// Get to the a
	TrainCar *aaa = head->next;
	int k = 0;
	while (aaa)
	{
		if (k++ == a) break;
		aaa = aaa->next;
	}
	
	// Get to the b
	TrainCar* bbb = head->next;
	k = 0;
	while (bbb)
	{
		if (k++ == b) break;
		bbb = bbb->next;
	}

	// Swap their prev and next
	TrainCar* temp;
	if (aaa && bbb)
	{
		temp = aaa->prev;
		aaa->prev = bbb->prev;
		bbb->prev = temp;

		temp = aaa->next;
		bbb->next = aaa->next;
		aaa->next = temp;
	}

	return true;	
}

void sortTrain(TrainCar* head, bool ascending)
{
	int size = getSize(head);
	
	for (int k = 0; k < size - 1; k++)
	{
		TrainCar* aaa = head->next;
		TrainCar* bbb = (aaa->next) ? aaa->next : nullptr;
		if (!bbb) break;
		
		do
		{
			// Swap the cars if needed
			if ((ascending && aaa->load > bbb->load) || (!ascending && aaa->load < bbb->load))
			{
				TrainCar* temp;
				if (aaa && bbb)
				{
					temp = aaa->prev;
					aaa->prev = bbb->prev;
					bbb->prev = temp;

					temp = aaa->next;
					bbb->next = aaa->next;
					aaa->next = temp;
				}
			}
		} while ((aaa = bbb) && (bbb = bbb->next));
	}
}

bool load(TrainCar* head, CarType type, int amount)
{
	TrainCar* temp = head->next;
	int remain = amount;
	do
	{
		if (temp->type == type)
		{
			int cap = temp->maxLoad - temp->load;
			if (cap >= remain)
				temp->load += remain;
			else
			{
				remain = remain - temp->maxLoad - temp->load;
				temp->load = temp->maxLoad;
			}
		}

		temp = temp->next;
	} while (temp);

	if (remain > 0)
		return false;

	return true;
}

bool unload(TrainCar* head, CarType type, int amount)
{
	// Go to the tail
	TrainCar* tail = head;
	while (tail && tail->next)
		tail = tail->next;

	int toUnload = amount;
	while (tail && tail->type != HEAD)
	{
		if (tail->type == type && tail->load > 0)
		{
			if (tail->load <= toUnload)
			{
				toUnload = toUnload - tail->load;
				tail->load = 0;
			}
			else
			{
				tail->load = tail->load - toUnload;
				toUnload = 0;
				break;
			}
		}

		tail = tail->prev;
	}

	if (toUnload > 0)
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

void subset_finder(TrainCar* origins[], TrainCar* temps[], int origin_size, int temp_size, int sum, int ite, int const upper)
{	
	for (int i = ite; i < origin_size; i++) {
		temps[temp_size] = origins[i];
		if (sum + origins[i]->load > upper) continue;
		subset_finder(origins, temps, origin_size, temp_size + 1, sum + origins[i]->load, i + 1, upper);
	}
	
	if (upper >= sum && subSum < sum)
	{
		subSum = sum;

		sub_size = temp_size;
		for (int i = 0; i < temp_size; ++i) sub_array[i] = temps[i];
			
		if (ite < origin_size)
			subset_finder(origins, temps, origin_size, temp_size - 1, sum - origins[ite]->load, ite + 1, upper);

		return;
	}
}

TrainCar* optimizeForMaximumPossibleCargos(const TrainCar* head, int upperBound)
{
	TrainCar *pos, *tempArray[MAX], *originals[MAX];

	pos = head->next;
	pos = pos->prev;

	int length = 0;
	while (pos = pos->next) {
		length++;
	}

	pos = head->next;
	pos = pos->prev;

	int i = 0;
	originals[0] = pos;
	tempArray[0] = nullptr;
	while (pos = pos->next)
	{
		originals[i] = pos;
		tempArray[i++] = nullptr;
	}
	subset_finder(originals, tempArray, length, 0, 0, 0, upperBound);

	TrainCar* n_train = createTrainHead(), *p;
	p = n_train;
	
	for (int i = 0; i < sub_size; ++i) {	 
		TrainCar* cargo = new TrainCar;
		cargo->load = sub_array[i]->load;
		cargo->maxLoad = sub_array[i]->maxLoad;
		cargo->type = sub_array[i]->type; 
		cargo->prev = p;
		cargo->next = nullptr;
		p->next = cargo;
		p = cargo;
	}

	return n_train;
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
