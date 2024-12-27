#pragma once
#include "core/sd_functions.h"
#include "Arduino.h"
#include "driver/i2s.h"

#define RECORDING_DURATION_MS 10000 // Durée de l'enregistrement (10 secondes)

bool recordAudioToSD();
