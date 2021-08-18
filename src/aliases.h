#ifndef HUMIDISTAT_ALIASES_H
#define HUMIDISTAT_ALIASES_H

#include "config.h"

// Humidity sensor
#ifdef HUMIDISTAT_DHT
#include "sensor/DHTHumiditySensor.h"
using HumiditySensor = DHTHumiditySensor;
#endif
#ifdef HUMIDISTAT_SHT
#include "sensor/SHTHumiditySensor.h"
using HumiditySensor = SHTHumiditySensor;
#endif

// Input
#ifdef HUMIDISTAT_INPUT_KS0256
#include "input/Ks0256VoltLadder.h"
using VoltLadder = Ks0256VoltLadder;
#endif
#ifdef HUMIDISTAT_INPUT_KS0466
#include "input/Ks0466VoltLadder.h"
using VoltLadder = Ks0466VoltLadder;
#endif

#endif //HUMIDISTAT_ALIASES_H
