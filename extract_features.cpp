/*
 * Script counts signal values on given ranges in .bed file
 * Assumes that raw data .bed files are discretized
 * 
*/

#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <filesystem>

const size_t REGION_SIZE = 1500;
const size_t BINS_PER_REGION = 10;
const size_t BS_PER_BIN = REGION_SIZE / BINS_PER_REGION;

struct Region {
    std::string chr;
    size_t start, end;

    bool operator<(const Region& other) {
        return start < other.start;
    }
};

std::istream& operator>>(std::istream& in, Region& region) {
    in >> region.chr >> region.start >> region.end;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Region& region) {
    out << region.chr << "\t" << region.start << "\t" << region.end;
    return out;
}

struct RegionExtended : public Region {
    size_t val;
    
    bool operator<(const RegionExtended& other) {
        return start < other.start;
    }
    bool operator<(const Region& other) {
        return start < other.start;
    }
};

bool operator<(const Region& reg1, const RegionExtended& reg2) {
    return reg1.start < reg2.start;
}

std::istream& operator>>(std::istream& in, RegionExtended& region) {
    std::string null;
    in >> region.chr >> region.start >> region.end >> region.val >> null >> null >> null;
    return in;
}

std::ostream& operator<<(std::ostream& out, const RegionExtended& region) {
    out << region.chr << "\t" << region.start << "\t" << region.end << "\t" << region.val;
    return out;
}

using DataStorage = std::unordered_map< std::string, std::vector<RegionExtended> >;

void LoadDataToMemory(DataStorage& storage, std::ifstream& file) {
    RegionExtended region;
    while (file >> region) {
        storage[region.chr].push_back(region);
    }
}

size_t GetValue(const DataStorage& storage, const Region& region) {
    if (storage.find(region.chr) == storage.end()) {
        return 0;
    }
    auto& vec = storage.at(region.chr);
    auto result_it = std::upper_bound(vec.begin(), vec.end(), region);
    if (result_it == vec.end()) {
        result_it = vec.begin() + vec.size() - 1;
    } else if (result_it == vec.begin()) {
        // pass
    } else {
        result_it -= 1;
    }

    if (region.start >= result_it->start && region.start <= result_it->end) {
        return result_it->val;
    } else {
        return 0;
    }
}

const size_t REGION_LENGTH = 1500;
const size_t BINS_COUNT = 100;
const size_t BPS_PER_BIN = REGION_LENGTH / BINS_COUNT;

const size_t MAX_REGIONS = -1;

void ProcessRegions(std::ifstream& exp_file, std::ifstream& data_file, std::ofstream& out_file) {
    DataStorage data_storage;
    LoadDataToMemory(data_storage, data_file);
    
    out_file << "chr\tstart\tend\t";
    for (size_t i = 0; i < BINS_COUNT; ++i) {
        out_file << i << "\t";
    }
    out_file << std::endl;

    Region region;
    size_t regions_count;
    while (exp_file >> region) {
        ++regions_count;
        out_file << region << "\t";
        for (size_t bin = 0; bin < BINS_COUNT; ++bin) {
            // std::cout << bin << std::endl;
            out_file << GetValue(
                data_storage,
                {
                    region.chr,
                    region.start + bin * BPS_PER_BIN,
                    region.start + (bin + 1) * BPS_PER_BIN
                }
            );
            out_file << "\t";
        }
        out_file << std::endl;
        if (regions_count == MAX_REGIONS) {
            break;
        }
    }
}

const std::string PATH_TO_RAW_DATA = "sorted_data/";
const std::string EXP_NAME = "curax_14h_UNI_mm9";
const std::string EXP_DATA_PATH = "experiment_data/";
const std::string OUT_PATH = "feature_data/";

int main() {
    std::cout << "Positive:" << std::endl;
    for (const auto& entry : std::filesystem::directory_iterator(PATH_TO_RAW_DATA)) {
        auto data_name = entry.path().stem().string();
        std::cout << data_name << " : " << EXP_DATA_PATH + EXP_NAME + ".regions.sorted.bed";
        std::ifstream exp_file(EXP_DATA_PATH + EXP_NAME + ".regions.sorted.bed");
        std::ifstream data_file(entry.path());
        std::ofstream out_file(OUT_PATH + "positive/" + data_name + ".pkbns");
        ProcessRegions(exp_file, data_file, out_file);
        std::cout << " ... ok" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Negative:" << std::endl;
    for (const auto& entry : std::filesystem::directory_iterator(PATH_TO_RAW_DATA)) {
        auto data_name = entry.path().stem().string();
        std::cout << data_name << " : " << EXP_DATA_PATH + EXP_NAME + "-random.regions.sorted.bed";
        std::ifstream exp_file(EXP_DATA_PATH + EXP_NAME + "-random.regions.sorted.bed");
        std::ifstream data_file(entry.path());
        std::ofstream out_file(OUT_PATH + "negative/" + data_name + ".pkbns");
        ProcessRegions(exp_file, data_file, out_file);
        std::cout << " ... ok" << std::endl;
    }
}
