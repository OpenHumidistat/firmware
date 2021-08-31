#ifndef HUMIDISTAT_IPOW_H
#define HUMIDISTAT_IPOW_H

/// Constexpr function for computing integer power.
/// \tparam T Base type
/// \param base Base
/// \param pow  Exponent
/// \return base^pow
template <typename T>
constexpr T ipow(T base, unsigned int pow) {
	return (pow >= sizeof(unsigned int)*8) ? 0 :
	pow == 0 ? 1 : base * ipow(base, pow-1);
}

#endif //HUMIDISTAT_IPOW_H
