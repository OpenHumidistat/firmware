#include <stdint.h>

#include "ConfigPar.h"
#include "asprintf.h"

void ConfigPar::adjust(int8_t delta) const {
	switch (var.type) {
		case ConfigParType::ui8:
			*var.ui8 += delta;
			return;
		case ConfigParType::ui16:
			*var.ui16 += delta;
			return;
		case ConfigParType::d:
			*var.d += static_cast<double>(delta) / 100;
			return;
	}
}

char *ConfigPar::asprint() const {
	switch (var.type) {
		case ConfigParType::ui8:
			return asprintf("%-4s %u", label, *var.ui8);
		case ConfigParType::ui16:
			return asprintf("%-4s %u", label, *var.ui16);
		case ConfigParType::d:
			return asprintf("%-4s %.2f", label, *var.d);
	}
}
