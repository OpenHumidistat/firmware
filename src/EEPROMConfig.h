#ifndef HUMIDISTAT_EEPROMCONFIG_H
#define HUMIDISTAT_EEPROMCONFIG_H

#include "config.h"

/// Config store containing variables, which can be stored in EEPROM.
struct ConfigStore {
	char version[5];       //!< String identifying this block
	bool loadedFromEEPROM; //!< Whether this has been loaded from EEPROM

	uint16_t dt; //!< Global interval for PID/logger (based on polling rate of sensor, in millis)

	///@{
	/// PID parameters
	uint8_t lowValue;
	double Kp;
	double Ki;
	double Kd;
	///@}
} const defaultConfigStore = {
	"hum2",
	false,
	config::dt,
	config::lowValue,
	config::Kp,
	config::Ki,
	config::Kd,
};

/// Load/save an (internal) ConfigStore in EEPROM.
class EEPROMConfig {
private:
	uint8_t address = config::EEPROMAddress;

public:
	ConfigStore configStore;

	/// Constructor.
	EEPROMConfig();

	/// Load config values from EEPROM into configStore.
	/// \return 1 if valid data was read, 0 if not
	bool load();

	/// Saves current content of configStore into EEPROM.
	/// \return number of bytes written
	uint16_t save() const;

	/// Reset the config store: overwrite the configStore with the default values.
	void reset();
};


#endif //HUMIDISTAT_EEPROMCONFIG_H
