#include <stdint.h>
#include <math.h>

#include "ConfigPar.h"
#include "asprintf.h"
#include "imath.h"

void ConfigPar::adjust(int16_t delta) const {
	switch (var.type) {
		case ConfigParType::ui8:
			*var.ui8 += delta;
			return;
		case ConfigParType::ui16:
			*var.ui16 += delta;
			return;
		case ConfigParType::d:
			*var.d += static_cast<double>(delta) / ipow(10, NUM_DECIMALS);
			return;
	}
}

char *ConfigPar::asprint() const {
	switch (var.type) {
		case ConfigParType::ui8:
			return asprintf("%-8s % " XSTR(WIDTH) "u", label, *var.ui8);
		case ConfigParType::ui16:
			return asprintf("%-8s % " XSTR(WIDTH) "u", label, *var.ui16);
		case ConfigParType::d:
			return asprintf("%-8s % " XSTR(WIDTH) "." XSTR(NUM_DECIMALS) "f", label, *var.d);
	}
}

uint8_t ConfigPar::magnitude() const {
	auto mag = [](auto n) -> uint8_t {return floor(ilog10(floor(abs(n))));};

	switch (var.type) {
		case ConfigParType::ui8:
			return mag(*var.ui8);
		case ConfigParType::ui16:
			return mag(*var.ui16);
		case ConfigParType::d:
			return mag(*var.d);
	}
}
