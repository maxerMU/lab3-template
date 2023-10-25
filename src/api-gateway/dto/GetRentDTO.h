#pragma once

#include <compare>
#include <string>
#include <vector>

struct GetRentDTO
{
public:
    void FromJSON(const std::string &json);
    std::string ToJSON() const;

    std::string rentUid;
    std::string status;
    std::string dateFrom;
    std::string dateTo;

    struct Car
    {
        std::string carUid;
        std::string brand;
        std::string model;
        std::string registrationNumber;
    } car;

    struct Payment
    {
        std::string paymentUid;
        std::string status;
        size_t price;
    } payment;

    auto operator<=>(const GetRentDTO &) const = default;
};

using GetRentsDTO = std::vector<GetRentDTO>;

std::string ToJSON(const GetRentsDTO& rents);