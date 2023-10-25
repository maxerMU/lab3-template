#pragma once

#include <compare>
#include <string>
#include <vector>

struct CarAvailabilityDTO
{
public:
    void FromJSON(const std::string &json);
    std::string ToJSON() const;

    std::string carUid;
    bool available;

    auto operator<=>(const CarAvailabilityDTO &) const = default;
};