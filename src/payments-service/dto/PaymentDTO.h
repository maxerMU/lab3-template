#pragma once

#include <compare>
#include <string>
#include <vector>

struct PaymentDTO
{
public:
    void FromJSON(const std::string &json);
    std::string ToJSON() const;

    std::string paymentUid;
    std::string status;
    size_t price;

    auto operator<=>(const PaymentDTO &) const = default;
};

using PaymentsDTO = std::vector<PaymentDTO>;

std::string ToJSON(const PaymentsDTO &Payments);