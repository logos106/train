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
