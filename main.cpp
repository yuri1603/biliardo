#include <vector>
#include "biliard.hpp"
#include <iostream>

int main() {
    bl::Biliard bil{5., 5., 5.};
    std::vector<bl::Ball> v = bil.random_balls(10);
    for(auto const& b : v) {
        std::cout << "y: " << b.y_coord << "    " << "theta: " << b.angle << '\n';
    }
}