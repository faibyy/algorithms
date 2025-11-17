#include "LinkedList.h"
#include <iostream>

int main() {
    LinkedList<int> list1(5, 10);
    LinkedList<int> list2(5);
    std::cout << list1 << '\n';
    std::cout << list2 << '\n';
    reverse_list(list2);
    std::cout << list2 << '\n';
    return 0;
}