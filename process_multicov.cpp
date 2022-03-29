#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

struct Region {
    std::string chr;
    size_t start, end;
};

std::istream& operator>>(std::istream& in, Region& region) {
    in >> region.chr >> region.start >> region.end;
    return in;
}

int main() {
    std::ifstream exp_file("curax_14h_UNI_mm9.regions-unified.sorted.bed");
    std::ifstream data_file("sorted_data/mm9_Histone_H2A.X.sorted.bed");
    std::ifstream out_file("curax_14h_UNI_mm9.signals-on-bins.csv");
    bool data_file_end = false;

    Region cur_region;
    data_file >> cur_region;

    Region exp_region;
    while (exp_file >> exp_region) {
        while (cur_region.chr != exp_region.chr) {
            if (!(data_file >> cur_region)) {
                break;
            }
        }
    }
}