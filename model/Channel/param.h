#ifndef MODEL_CHANNEL_PARAM_H
#define MODEL_CHANNEL_PARAM_H

#include "../../lib/timef.h"
#include "../Matter/param.h"
#include "../Sensor/param.h"
#include "../EM/param.h"

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
