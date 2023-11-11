#pragma once

#include <compare>
#include <string>
#include <vector>

struct ErrorMessageDTO
{
public:
    void FromJSON(const std::string &json);
    std::string ToJSON() const;

    std::string message;

    auto operator<=>(const ErrorMessageDTO &) const = default;
};