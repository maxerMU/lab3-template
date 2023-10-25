#pragma once

#include <compare>
#include <string>
#include <vector>

struct PostRentDTO
{
public:
    void FromJSON(const std::string &json);
    std::string ToJSON() const;

    std::string paymentUid;
    std::string carUid;
    std::string dateFrom;
    std::string dateTo;

    auto operator<=>(const PostRentDTO &) const = default;
};
