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
	double HC_Kf;
	///@}

	///@{
	/// Flow controller PID parameters
	double FC_Kp;
	double FC_Ki;
	double FC_Kd;
	double FC_Kf;
	uint16_t FC_dt;
	///@}

	/// Minimum solenoid duty cycle (deadband)
	double S_lowValue;

	/// Total flowrate (for cascade controller) (L/min)
	double HC_totalFlowrate;

	/// Smoothing factor of EMA filter for derivative
	double a;
} const defaultConfigStore = {
	"hum2",
	false,
	config::dt,

	config::HC_Kp,
	config::HC_Ki,
	config::HC_Kd,
	config::HC_Kf,

	config::FC_Kp,
	config::FC_Ki,
	config::FC_Kd,
	config::FC_Kf,
	config::FC_dt,

	config::S_lowValue,
	config::HC_totalFlowrate,
	config::a,
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
