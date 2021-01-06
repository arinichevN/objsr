#ifndef MODEL_SENSOR_H
#define MODEL_SENSOR_H

#include "../../lib/app.h"
#include "../../lib/timef.h"
#include "../interfaces/iSense.h"
#include "param.h"

typedef struct sensor_st Sensor;

struct sensor_st{
	int id;
	iSense *slave;
	Ton tmr;
	FTS output;
	void (*control)(Sensor *);
};

extern void sensor_setSlave (Sensor *self, iSense *slave);

extern void sensor_setParam(Sensor *self, const SensorParam *param);

extern void sensor_start(Sensor *self);

extern void sensor_stop(Sensor *self);

extern FTS sensor_getOutput(Sensor *self);

extern const char *sensor_getStateStr(Sensor *self);

extern const char *sensor_getErrorStr(Sensor *self);

#endif
