#include <EEPROMex.h>

#include "EEPROMConfig.h"

bool EEPROMConfig::load() {
	EEPROM.readBlock(address, configStore);

	// Check whether loaded data is valid or if overrideEEPROM is set
	if(strcmp(configStore.version, defaultConfigStore.version) == 0 && !config::overrideEEPROM) {
		// Set loadedFromEEPROM flag
		configStore.loadedFromEEPROM = true;
		return true;
	} else {
		configStore = defaultConfigStore;
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
