#pragma once

#include <compare>
#include <string>
#include <vector>

struct RentDTO
{
public:
    void FromJSON(const std::string &json);
    std::string ToJSON() const;

    std::string rentUid;
    std::string username;
    std::string paymentUid;
    std::string carUid;
    std::string dateFrom;
    std::string dateTo;
    std::string status;

    auto operator<=>(const RentDTO &) const = default;
};

using RentsDTO = std::vector<RentDTO>;

std::string ToJSON(const RentsDTO &Rents);
std::string ToJSON(const RentsDTO &Rents, size_t page, size_t pageSize);