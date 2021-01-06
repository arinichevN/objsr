#ifndef MODEL_MATTER_H
#define MODEL_MATTER_H

#include <math.h>
#include "../../lib/debug.h"
#include "../../lib/dstructure.h"
#include "../../lib/timef.h"
#include "../interfaces/iSense.h"
#include "param.h"

DEC_PIPE(double, DPipe)

typedef struct {
	double temperature;
	double energy;
	double mass;
	double ksh;
	double kl; //loose factor
	double pl;//loose power
	size_t temperature_pipe_length;
	DPipe temperature_pipe;
	struct timespec t1;
	iSense im_temperature;
} Matter;

extern int matter_setParam(Matter *self, const MatterParam *param);

extern void matter_begin(Matter *self);

extern void matter_control(Matter *self, double ambient_temperature, double heater_power, double cooler_power);

extern void matter_start(Matter *self, double ambient_temperature);

extern void matter_free(Matter *self);

#endif
