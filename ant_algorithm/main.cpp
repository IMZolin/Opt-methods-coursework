#include <bits/stdc++.h>
#include <limits.h>
#include <utility>
#include <fstream>
#include <vector>
class Source {
public:
    std::string filename;
    std::vector<int> vec;

public:
    Source(std::string filename_) {filename = filename_;}
    ~Source() {vec.clear();}

    void Read() {
        std::ifstream input;
        input.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        try {
            input.open(filename);
            int teg;
            while(true) {
                input >> teg;
                vec.push_back(teg);
            }

        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
};
int rand_num(int start, int end)
{
    int r = end - start;
    int rnum = start + rand() % r;
    return rnum;
}
int main() {
    std::cout << "Ant algorithm" << std::endl;
    return 0;
}
