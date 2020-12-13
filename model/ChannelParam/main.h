#ifndef MODEL_CHANNEL_PARAM_H
#define MODEL_CHANNEL_PARAM_H

#include "time.h"
#include "../MatterParam/main.h"
#include "../SensorParam/main.h"
#include "../EMParam/main.h"

typedef struct {
	double ambient_temperature;
	MatterParam matter;
	SensorParam sensor;
	EMParam heater;
	EMParam cooler;
	struct timespec cycle_duration;
} ChannelParam;

extern int channelParam_check(const ChannelParam *self);

#endif 
