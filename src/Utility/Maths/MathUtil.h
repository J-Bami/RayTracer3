#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <cstddef>

typedef unsigned char Byte;

namespace Maths
{
    template<typename ftype>
    constexpr ftype pi = 3.1415926535897932384626433832795028841971;

    template<typename ftype>
    constexpr ftype two_pi = 2. * pi<ftype>;

    template<typename ftype>
    constexpr ftype four_pi = 4. * pi<ftype>;
    
    template<typename ftype>
    constexpr ftype inv_pi = 1 / pi<ftype>;

    template<typename ftype>
    constexpr ftype half_pi = 0.5 * pi<ftype>;

    template<typename ftype>
    constexpr ftype radtodeg = ftype(180.0) / pi<ftype>;

    template<typename ftype>
    constexpr ftype degtorad = pi<ftype>/ftype(180.0);

    size_t factorial(const size_t n)
    {
        size_t out = 1;
        for (size_t i = 1; i <= n; i++) { out *= i; }
        return out;
    }

    // does nCr
    size_t binomial_coefficient(const size_t n, const size_t r)
    {
        return factorial(n) / (factorial(n - r) * factorial(r));
    }

    //calculates x^y
    long power(const long x, const size_t y)
    {
        if (y == 0)
        {
            return 1;
        }
        else if (!(y & 1))
        {
            return power(x, y / 2) * power(x, y / 2);
        }
        else
        {
            return x * power(x, y / 2) * power(x, y / 2);
        }
    }

    //same as abs
    template <typename type>
    type modulus(const type& value)
    {
        return value < 0 ? -value : value;
    }

    template<typename type>
    inline type product(const type* array, const size_t length)
    {
        type prod = 1;
        for (size_t i = 0; i < length; i++)
        {
            prod *= array[i];
        }
        return prod;
    }

    template<typename type>
    inline const type& max(const type& a, const type& b)
    {
        return a > b? a : b;
    }

    template<typename type>
    inline type& max(const type* a, const size_t n)
    {
        type& out = a[0]
        for (size_t i = 1; i < n; i++)
        {
            if(out < a[i])
            {
                out = a[i];
            }
        }
        return out;
    }

    template<typename type>
    inline const type& min(const type& a, const type& b)
    {
        return a < b? a : b;
    }

    template<typename type>
    inline const type& min(const type* a, const size_t n)
    {
        type& out = a[0]
        for (size_t i = 1; i < n; i++)
        {
            if(out > a[i])
            {
                out = a[i];
            }
        }
        return out;
    }

    template<typename T>
    inline void swap(T& a, T& b)
    {
        const T temp = b;
        b = a;
        a = temp;
    }

    template<typename numeric_type>
    inline numeric_type clamp(
        const numeric_type& x,
        const numeric_type lower = 0,
        const numeric_type upper = 1)
    {
        if(x < lower)
        {
            return lower;
        }
        else if(x > upper)
        {
            return upper;
        }
        else
        {
            return x;
        }
    }

    template<typename itype, typename ftype>
    inline itype iround(const ftype floating)
    {
        return static_cast<itype>(floating + ftype(0.5));
    }


    template<typename ftype>
    inline ftype true_angle(const ftype x, const ftype y)
    {
        if (x < 0)
        {
            if (y > 0)
            {
                return atan(y / x) + half_pi<ftype>;
            }
            else
            {
                return atan(y / x) - half_pi<ftype>;
            }
        }
        else if (x == 0)
        {
            if (y > 0)
            {
                return half_pi<ftype>;
            }
            else if (y < 0)
            {
                return -half_pi<ftype>;
            }
            return 0;
        }
        return atan(y / x);
    }
}

#endif