#include <bits/stdc++.h>
#include "interval.h"

template <class T>
struct Node {
    interval::interval<T> interval;
    Node() = default;
    ~Node() = default;
    Node(interval::interval<T> _interval) : interval(_interval) {}

};


int main() {
    Node<int> test;
    test.interval.print();
    return 0;
}