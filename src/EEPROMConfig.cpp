#include <EEPROMex.h>

#include "EEPROMConfig.h"

bool EEPROMConfig::load() {
	EEPROM.readBlock(address, configStore);

	// Check whether loaded data is valid and if overrideEEPROM is not set
	if(strcmp(configStore.version, defaultConfigStore.version) == 0 && !config::overrideEEPROM) {
		// Set loadedFromEEPROM flag
		configStore.loadedFromEEPROM = true;
		return true;
	} else {
		// Reset to defaults
		reset();
		save();
		return false;
	}
}

uint16_t EEPROMConfig::save() const {
	return EEPROM.updateBlock(address, configStore);
}

EEPROMConfig::EEPROMConfig() {
	load();
}

void EEPROMConfig::reset() {
	configStore = defaultConfigStore;
}
