#pragma once

#include <compare>
#include <string>
#include <vector>

struct PostRentCarDTO
{
public:
    void FromJSON(const std::string &json);
    std::string ToJSON() const;

    std::string carUid;
    std::string dateFrom;
    std::string dateTo;

    auto operator<=>(const PostRentCarDTO &) const = default;
};
