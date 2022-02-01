#ifndef FIRMWARE_SETPOINTPROFILERUNNER_H
#define FIRMWARE_SETPOINTPROFILERUNNER_H

#include <etl/span.h>

#include "Point.h"
#include "control/Humidistat.h"

/// 'Runs' a setpoint profile.
class SetpointProfileRunner {
private:
	Humidistat& humidistat;
	etl::span<const Point> profile;
	uint32_t timeStart;
	bool running = false;
public:
	/// Constructor.
	/// \param humidistat Pointer to a Humidistat instance
	explicit SetpointProfileRunner(Humidistat* humidistat);

	/// Toggle the run state.
	void toggle();

	/// Set the profile.
	/// \param profile Span over Points
	void setProfile(const etl::span<const Point>& profile);

	/// Run the profile (if running).
	/// Call this periodically.
	void update();

	/// Get the run state.
	/// \return True if running
	[[nodiscard]] bool isRunning() const;

	/// Get the index of the current Point in the profile.
	/// \return index
	[[nodiscard]] size_t getCurrentPoint() const;
};


#endif //FIRMWARE_SETPOINTPROFILERUNNER_H
