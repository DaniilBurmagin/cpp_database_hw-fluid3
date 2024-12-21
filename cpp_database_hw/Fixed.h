#ifndef FIXED_H
#define FIXED_H

#include <cstdint>
#include <type_traits>
#include <stdexcept>
#include <iostream>

template <int N, int K>
class Fixed {
    static_assert(N > K, "N must be greater than K");
    static_assert(N <= 64, "N must be at most 64");

    using BaseType = typename std::conditional<N <= 32, int32_t, int64_t>::type;
    BaseType value;

public:
    // ����������� �� ������������� �����
    Fixed(double x = 0) : value(static_cast<BaseType>(x* (1 << K))) {}

    // �������������� ���������
    Fixed operator+(const Fixed& other) const {
        return Fixed::fromRaw(value + other.value);
    }

    Fixed operator-(const Fixed& other) const {
        return Fixed::fromRaw(value - other.value);
    }

    Fixed operator*(const Fixed& other) const {
        return Fixed::fromRaw((value * other.value) >> K);
    }

    Fixed operator/(const Fixed& other) const {
        if (other.value == 0) {
            throw std::runtime_error("Division by zero in Fixed type.");
        }
        return Fixed::fromRaw((value << K) / other.value);
    }

    // ��������� ���������
    bool operator==(const Fixed& other) const {
        return value == other.value;
    }

    bool operator!=(const Fixed& other) const {
        return value != other.value;
    }

    bool operator<(const Fixed& other) const {
        return value < other.value;
    }

    bool operator<=(const Fixed& other) const {
        return value <= other.value;
    }

    bool operator>(const Fixed& other) const {
        return value > other.value;
    }

    bool operator>=(const Fixed& other) const {
        return value >= other.value;
    }

    // �������������� � double
    double toDouble() const {
        return static_cast<double>(value) / (1 << K);
    }

    // �������� �� "������" ��������
    static Fixed fromRaw(BaseType rawValue) {
        Fixed f;
        f.value = rawValue;
        return f;
    }

    // ���������� ��������� ������
    friend std::ostream& operator<<(std::ostream& os, const Fixed& f) {
        os << f.toDouble();
        return os;
    }

    // ���������� ��������� �����
    friend std::istream& operator>>(std::istream& is, Fixed& f) {
        double temp;
        is >> temp;
        f = Fixed(temp);
        return is;
    }
};

#endif // FIXED_H