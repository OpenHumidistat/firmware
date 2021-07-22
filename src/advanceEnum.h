#ifndef HUMIDISTAT_ADVANCEENUM_H
#define HUMIDISTAT_ADVANCEENUM_H

/// Cycle through (advance) an enum.
/// \tparam T Enum type (should have _count as last element)
/// \param e Enum instance
template <typename T>
void advanceEnum(T &e) {
	e = static_cast<T>((static_cast<int>(e) + 1) % static_cast<int>(T::_count));
}

#endif //HUMIDISTAT_ADVANCEENUM_H
