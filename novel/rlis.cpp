#include "rlis.h"
#include <iostream>

RLIS::RLIS(std::vector<int> _sequence, std::vector<std::pair<int,int>> _queries) {
    this->seq = _sequence;
    this->queries = _queries;
}

std::vector<int> RLIS::run() {
    for (int a_i : this->seq) {
        std::cout << a_i << '\n';
    }
    return this->seq;
}