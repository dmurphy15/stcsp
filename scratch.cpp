#include <vector>
#include <iostream>
#include <functional>

//int radish() {
//    std::vector<int> fun{1, 2, 3, 4, 5};
//    while (fun.size() > 0) {
//        yield fun.pop_back();
//        std::cout<<"I'm a radish\n";
//    }
//}

class Food {
public:
    Food() {}
    ~Food() {std::cout<<"goodbye, food\n";}
};

int main(int argc, char **argv) {
//    for (int i=0; i<5; i++) {
//        std::cout<<radish();
//    }
    std::vector<std::reference_wrapper<Food>> v;
    Food food1 = Food();
    Food food2 = Food();
    Food &food3 = food1;
    Food &food4 = food2;
    v.push_back(food3);
    v.push_back(food4);
    v.erase(v.begin());
    std::cout<<"erased\n";
};