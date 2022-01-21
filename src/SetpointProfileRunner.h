#ifndef FIRMWARE_SETPOINTPROFILERUNNER_H
#define FIRMWARE_SETPOINTPROFILERUNNER_H

#include <etl/span.h>

#include "control/Humidistat.h"

struct Point {
	const uint16_t time;
	const double sp;
};

class SetpointProfileRunner {
private:
	Humidistat& humidistat;
	etl::span<const Point> profile;
	uint32_t timeStart;
	bool running = false;
public:
	SetpointProfileRunner(Humidistat* humidistat, etl::span<const Point> profile);

	void toggle();
	void update();
};


#endif //FIRMWARE_SETPOINTPROFILERUNNER_H
