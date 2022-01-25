#ifndef HUMIDISTAT_ADVANCEENUM_H
#define HUMIDISTAT_ADVANCEENUM_H

#include <stdint.h>

/// Cycle through (advance) an enum.
/// \tparam T Enum type (should have _last as final element, equaling the last valid element)
/// \param e Enum instance
/// \param n Step (can be negative)
template <typename T>
void advanceEnum(T &e, int8_t n = 1) {
	e = static_cast<T>((static_cast<typename std::underlying_type<T>::type>(e) + n) % (static_cast<typename std::underlying_type<T>::type>(T::_last) + 1));
}

#endif //HUMIDISTAT_ADVANCEENUM_H
