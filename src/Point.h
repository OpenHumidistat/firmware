#ifndef FIRMWARE_POINT_H
#define FIRMWARE_POINT_H

#include <stdint.h>
#include <etl/span.h>

/// Point: a vector of a time and setpoint value.
struct Point {
	const uint16_t time;	//!< Time in seconds
	const uint8_t sp;
};

/// Setpoint profile: a label and a span over Points.
struct SPProfile {
	const char label[12];
	const etl::span<const Point> profile;
};

#endif //FIRMWARE_POINT_H
