#pragma once

#include <compare>
#include <string>
#include <vector>

struct PostRentResultDTO
{
public:
    void FromJSON(const std::string &json);
    std::string ToJSON() const;

    std::string rentUid;
    std::string status;
    std::string carUid;
    std::string dateFrom;
    std::string dateTo;

    struct Payment
    {
        std::string paymentUid;
        std::string status;
        size_t price;
    } payment;

    auto operator<=>(const PostRentResultDTO &) const = default;
};