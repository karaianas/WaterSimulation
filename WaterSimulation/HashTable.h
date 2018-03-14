#pragma once
#include "Core.h"
#include "Particle.h"
#include <unordered_map>
#include <list>

using namespace std;

class HashTable{
public:
	HashTable()
	{
	}

	HashTable(int gridSize, float spacing) 
	{ 
		m = gridSize; 
		d = spacing; 
	}

	int getCell(glm::vec3 pos)
	{
		int cellIndex = ((int(floor(pos.x / d)) * 73856093) ^ (int(floor(pos.y / d)) * 19349663) ^ (int(floor(pos.z / d)) * 83492791)) % m;
		return cellIndex;
	}

	// These can suffer from race condition!!!
	void addToCell2(int cellIndex, Particle* ptr)
	{
		if (htable[cellIndex].empty())
		{
			list<Particle*> newList;
			newList.push_back(ptr);
			htable[cellIndex] = newList;
		}
		else
			htable[cellIndex].push_back(ptr);
	}

	void addToCell(Particle* ptr)
	{
		int cellIndex = getCell(ptr->position);

		if (htable[cellIndex].empty())
		{
			list<Particle*> newList;
			newList.push_back(ptr);
			htable[cellIndex] = newList;
		}
		else
		{
			htable[cellIndex].push_back(ptr);
		}

		//cout << cellIndex << ": " << ptr->id << " added" << endl;
	}

	void deleteFromCell(int cellIndex, Particle* ptr)
	{
		if (htable[cellIndex].size() <= 0)
			return;
		else if (htable[cellIndex].size() == 1)
		{
			htable[cellIndex].clear();
			htable.erase(cellIndex);
		}
		else
		{
			list<Particle*>::iterator it = htable[cellIndex].begin();
			for (auto p : htable[cellIndex])
			{
				if (p->id == ptr->id)
				{
					htable[cellIndex].erase(it);
					break;
				}
				else
					it++;
			}
		}
	};

	vector<Particle*> getNeighbors(Particle* ptr)
	{
		glm::vec3 pos = ptr->position;
		vector<Particle*> neighbors;
		unordered_map<int, int> minitable;

		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				for (int k = -1; k <= 1; k++)
				{
					int cellIndex = ((int(floor(pos.x / d + i)) * 73856093) ^ (int(floor(pos.y / d + j)) * 19349663) ^ (int(floor(pos.z / d + k)) * 83492791)) % m;
					if (minitable[cellIndex] > 0)
						minitable[cellIndex]++;
					else
						minitable.emplace(cellIndex, 1);
				}

		for (auto element : minitable)
			for (auto p : htable[element.first])
			{
				if (p->id != ptr->id && glm::length(p->position - pos) <= 2 * d)
				{
					//cout << p->id << " is a neighbor" << endl;
					if (ptr->id == 0)
						p->flag = true;
					neighbors.push_back(p);
				}
			}

		ptr->neighbors = neighbors;
		//cout << neighbors.size() << endl;
		return neighbors;
	}

	void printTable()
	{
		int count = 0;
		for (auto element : htable)
		{
			cout << count << ": ";
			for (auto p : element.second)
			{
				cout << p->id << " ";
			}
			cout << endl;
			count++;
		}
	}

	void reset()
	{
		htable.clear();
	}



private:
	int m;
	float d;
	unordered_map<int, list<Particle*>> htable;
};