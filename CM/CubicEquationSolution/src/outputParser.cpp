#include "../include/outputParser.h"
#include <iostream>

void outputParser::showResults(std::vector<double> solution, std::vector<int> multiplicity) {
    std::cout << "roots found: " << solution.size() << std::endl;
    for (int i = 0; i < solution.size(); i++) {
        std::cout << "root № " << i + 1 << ": " << solution.at(i) << ", multiplicity: " << multiplicity.at(i) << std::endl;
    }

}
