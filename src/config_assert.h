#ifndef HUMIDISTAT_CONFIG_ASSERT_H
#define HUMIDISTAT_CONFIG_ASSERT_H

#if !defined(HUMIDISTAT_CONTROLLER_SINGLE) && !defined(HUMIDISTAT_CONTROLLER_CASCADE)
#error No humidistat type defined! Configure the firmware in src/config.h first.
#endif

#if !defined(HUMIDISTAT_DHT) && !defined(HUMIDISTAT_SHT)
#error No humidity sensor type defined! Configure the firmware in src/config.h first.
#endif

#if !defined(HUMIDISTAT_INPUT_KS0256) && !defined(HUMIDISTAT_INPUT_KS0466)
#error No keypad type defined! Configure the firmware in src/config.h first.
#endif

#if !defined(HUMIDISTAT_UI_CHAR) && !defined(HUMIDISTAT_UI_GRAPH)
#error No UI type defined! Configure the firmware in src/config.h first.
#endif

#endif //HUMIDISTAT_CONFIG_ASSERT_H
