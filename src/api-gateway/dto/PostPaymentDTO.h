#pragma once

#include <compare>
#include <string>
#include <vector>

struct PostPaymentDTO
{
public:
    void FromJSON(const std::string &json);
    std::string ToJSON() const;

    size_t price;

    auto operator<=>(const PostPaymentDTO &) const = default;
};