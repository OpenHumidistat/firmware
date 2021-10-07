#ifndef HUMIDISTAT_ASPRINTF_H
#define HUMIDISTAT_ASPRINTF_H

#include <stdlib.h>
#include <stdio.h>

/// Print formatted data to string. Automatically allocates string on the heap. Make sure to delete it
/// immediately afterwards.
/// \param fmt  Format string
/// \param args Arguments specifying data to print
/// \return Pointer to char string
template <typename... T>
char * asprintf(const char *fmt, T... args) {
	size_t len = snprintf(nullptr, 0, fmt, args...);
	char *buf = new char [len+1];
	sprintf(buf, fmt, args...);
	return buf;
}

#endif //HUMIDISTAT_ASPRINTF_H
