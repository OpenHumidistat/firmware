#ifndef HUMIDISTAT_IMATH_H
#define HUMIDISTAT_IMATH_H

/// Constexpr function for computing integer power.
/// \tparam T Base type
/// \param base Base
/// \param pow  Exponent
/// \return base^pow
template<typename T>
constexpr T ipow(T base, unsigned int pow) {
	return (pow >= sizeof(unsigned int) * 8) ? 0 : pow == 0 ? 1 : base * ipow(base, pow - 1);
}

/// Constexpr integer base-10 logarithm.
/// \param n
/// \return 10-log(n)
constexpr unsigned int ilog10(int n) {
	if (n <= 1) {
		return 0;
	} else {
		return ilog10(n / 10) + 1;
	}
}

#endif //HUMIDISTAT_IMATH_H
