#pragma once

#include <vector>

class maxHeap
{
private:
    std::vector<int> heap;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    maxHeap();

    bool empty() const;
    
    int size() const;

    void insert(int value);

    int getMax() const;

    void removeMax();

    void print() const;
};

