#include <iostream>
#include "maxHeap.h"

maxHeap::maxHeap()
{
}

bool maxHeap::empty() const
{
    return heap.empty();
}

int maxHeap::size() const
{
    return heap.size();
}

void maxHeap::insert(int value)
{
    heap.push_back(value);

    heapifyUp(heap.size() - 1);
}

void maxHeap::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap[parent] >= heap[index])
            break;

        std::swap(heap[parent], heap[index]);

        index = parent;
    }
}

int maxHeap::getMax() const
{
    if (heap.empty())
        throw std::runtime_error("Heap is empty.");

    return heap[0];
}

void maxHeap::removeMax()
{
    if (heap.empty())
        return;

    heap[0] = heap.back();

    heap.pop_back();

    if (!heap.empty())
        heapifyDown(0);
}

void maxHeap::heapifyDown(int index)
{
    int size = heap.size();

    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        int largest = index;

        if (left < size && heap[left] > heap[largest])
            largest = left;

        if (right < size && heap[right] > heap[largest])
            largest = right;

        if (largest == index)
            break;

        std::swap(heap[index], heap[largest]);

        index = largest;
    }
}

void maxHeap::print() const
{
    for (int value : heap)
    {
        std::cout << value << " ";
    }

    std::cout << std::endl;
}