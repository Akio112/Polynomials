#include "classes/polynomials.h"
#include <string>
#include <algorithm>
#include <iostream>


int main() {
    std::string x, y;
    std::cin >> x >> y;
    Polynomial first(x), second(y);
    if (first == second) {
        std::cout << "Yes";
    } else {
        std::cout << "No";
    }
}
