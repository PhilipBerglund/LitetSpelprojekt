#pragma once

#include <random>

template<typename T>
using UniformRealDistribution = std::uniform_real_distribution<T>;
template<typename T>
using UniformIntDistribution = std::uniform_int_distribution<T>;


class Random
{
public:
    using Device = std::random_device;
    using Engine = std::mt19937;

public:
    template<typename IntegralType>
    static int
        Integer(IntegralType lower = static_cast<IntegralType>(0), IntegralType upper = static_cast<IntegralType>(100))
    {
        static_assert(std::is_integral<IntegralType>::value, "IntegralType must be integral type");
        static Device s_Device;
        static Engine s_Engine(s_Device());
        UniformIntDistribution<IntegralType> distribution(lower, upper);
        return distribution(s_Engine);
    }

    template<typename RealType = float>
    static RealType Real(RealType lower = static_cast<RealType>(0), RealType upper = static_cast<RealType>(1))
    {
        static_assert(std::is_floating_point<RealType>::value, "RealType must be floating point type");
        static Device s_Device;
        static Engine s_Engine(s_Device());
        UniformRealDistribution<RealType> distribution(lower, upper);
        return distribution(s_Engine);
    }
};