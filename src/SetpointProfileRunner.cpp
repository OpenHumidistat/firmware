#include "SetpointProfileRunner.h"

SetpointProfileRunner::SetpointProfileRunner(Humidistat *humidistat, etl::span<const Point> profile)
: humidistat(*humidistat), profile(profile) {}

void SetpointProfileRunner::update() {
	if(!running) {
		return;
	}

	for(auto point : profile) {
		if(millis() - timeStart > point.time*1000) {
			humidistat.sp = point.sp;
		}
	}
	if(millis() - timeStart > profile.back().time*1000) {
		// Reached end of profile
		running = false;
	}
}

void SetpointProfileRunner::toggle() {
	timeStart = millis();
	running = !running;
}
