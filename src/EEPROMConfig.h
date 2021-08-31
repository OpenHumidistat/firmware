#ifndef HUMIDISTAT_EEPROMCONFIG_H
#define HUMIDISTAT_EEPROMCONFIG_H

#include CONFIG_HEADER

/// Config store containing variables, which can be stored in EEPROM.
struct ConfigStore {
	char version[5];       //!< String identifying this block
	bool loadedFromEEPROM; //!< Whether this has been loaded from EEPROM

	uint16_t dt; //!< Global interval for PID/logger (based on polling rate of sensor, in millis)

	///@{
	/// Humidity controller PID parameters
	double HC_Kp;
	double HC_Ki;
	double HC_Kd;
	///@}

	///@{
	/// Flow controller PID parameters
	double FC_Kp;
	double FC_Ki;
	double FC_Kd;
	///@}

	/// Minimum solenoid duty cycle (deadband)
	uint8_t S_lowValue;

	/// Total flowrate (for cascade controller) (L/min)
	double HC_totalFlowrate;
} const defaultConfigStore = {
	"hum2",
	false,
	config::dt,

	config::HC_Kp,
	config::HC_Ki,
	config::HC_Kd,

	config::FC_Kp,
	config::FC_Ki,
	config::FC_Kd,

	config::S_lowValue,
	config::HC_totalFlowrate
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
