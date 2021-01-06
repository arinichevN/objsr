#ifndef MODEL_SENSOR_PARAM_H
#define MODEL_SENSOR_PARAM_H

typedef struct {
	int id;
} SensorParam;

extern int sensorParam_check(const SensorParam *self);

#endif 
