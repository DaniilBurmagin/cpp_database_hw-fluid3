#ifndef FAST_FIXED_H
#define FAST_FIXED_H

#include <cstdint>
#include <type_traits>
#include <stdexcept>
#include <iostream>

template <int N, int K>
class FastFixed {
    using BaseType = typename std::conditional<N <= 32, int_fast32_t, int_fast64_t>::type;
    BaseType value;

public:
    // ����������� �� ������������� �����
    FastFixed(double x = 0) : value(static_cast<BaseType>(x* (1 << K))) {}

    // �������������� ���������
    FastFixed operator+(const FastFixed& other) const {
        return FastFixed::fromRaw(value + other.value);
    }

    FastFixed operator-(const FastFixed& other) const {
        return FastFixed::fromRaw(value - other.value);
    }

    FastFixed operator*(const FastFixed& other) const {
        return FastFixed::fromRaw((value * other.value) >> K);
    }

    FastFixed operator/(const FastFixed& other) const {
        if (other.value == 0) {
            throw std::runtime_error("Division by zero in FastFixed type.");
        }
        return FastFixed::fromRaw((value << K) / other.value);
    }

    // ��������� ���������
    bool operator==(const FastFixed& other) const { return value == other.value; }
    bool operator!=(const FastFixed& other) const { return value != other.value; }
    bool operator<(const FastFixed& other) const { return value < other.value; }
    bool operator<=(const FastFixed& other) const { return value <= other.value; }
    bool operator>(const FastFixed& other) const { return value > other.value; }
    bool operator>=(const FastFixed& other) const { return value >= other.value; }

    // �������������� � double
    double toDouble() const {
        return static_cast<double>(value) / (1 << K);
    }

    // �������� �� "������" ��������
    static FastFixed fromRaw(BaseType rawValue) {
        FastFixed f;
        f.value = rawValue;
        return f;
    }

    // ���������� ��������� ������
    friend std::ostream& operator<<(std::ostream& os, const FastFixed& f) {
        os << f.toDouble();
        return os;
    }

    // ���������� ��������� �����
    friend std::istream& operator>>(std::istream& is, FastFixed& f) {
        double temp;
        is >> temp;
        f = FastFixed(temp);
        return is;
    }
};

#endif // FAST_FIXED_H