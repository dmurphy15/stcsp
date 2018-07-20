#include <vector>
#include <iostream>

int radish() {
    std::vector<int> fun{1, 2, 3, 4, 5};
    while (fun.size() > 0) {
        yield fun.pop_back();
        std::cout<<"I'm a radish\n";
    }
}


int main(int argc, char **argv) {
    for (int i=0; i<5; i++) {
        std::cout<<radish();
    }
}