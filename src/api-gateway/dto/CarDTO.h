#pragma once

#include <compare>
#include <string>
#include <vector>

struct CarDTO
{
public:
    void FromJSON(const std::string &json);
    std::string ToJSON() const;

    std::string carUid;
    std::string brand;
    std::string model;
    std::string registrationNumber;
    size_t power;
    size_t price;
    std::string type;
    bool available;

    auto operator<=>(const CarDTO &) const = default;
};

using CarsDTO = std::vector<CarDTO>;

std::string ToJSON(const CarsDTO &cars);
std::string ToJSON(const CarsDTO &cars, size_t page, size_t pageSize);