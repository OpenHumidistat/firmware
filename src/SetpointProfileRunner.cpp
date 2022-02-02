#include "SetpointProfileRunner.h"

SetpointProfileRunner::SetpointProfileRunner(Humidistat *humidistat)
: humidistat(*humidistat) {}

void SetpointProfileRunner::update() {
	if(!running) {
		return;
	}

	humidistat.sp = profile[getCurrentPoint()].sp;

	if(millis() - timeStart > profile.back().time*1000) {
		// Reached end of profile
		running = false;
	}
}

void SetpointProfileRunner::toggle() {
	timeStart = millis();
	running = !running;
}

void SetpointProfileRunner::setProfile(const etl::span<const Point> &profile) {
	this->profile = profile;
}

bool SetpointProfileRunner::isRunning() const {
	return running;
}

size_t SetpointProfileRunner::getCurrentPoint() const {
	// Loop backwards over profile, returning the first (largest) index of the point whose time is less than runtime
	for (size_t i = profile.size() - 1; i >= 0; i--) {
		if(millis() - timeStart > profile[i].time*1000) {
			return i;
		}
	}
}
