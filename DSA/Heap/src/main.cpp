#include <iostream>
#include "maxHeap.h"


int main()
{
    maxHeap heap;

    heap.insert(50);
    heap.insert(30);
    heap.insert(80);
    heap.insert(20);
    heap.insert(70);
    heap.insert(100);
    heap.insert(40);

    std::cout << "Heap: ";
    heap.print();

    std::cout << "Max = " << heap.getMax() << std::endl;

    std::cout << "\nRemove Max\n";

    heap.removeMax();

    std::cout << "Heap: ";
    heap.print();

    std::cout << "Max = " << heap.getMax() << std::endl;

    return 0;
}