#ifndef ACPS_PORT_CONTROLLER_PARAM_H
#define ACPS_PORT_CONTROLLER_PARAM_H

#include "../../../../../serial.h"
#include "../../../../../app.h"

typedef struct {
	char *filename;
	char config[SERIAL_CONFIG_STRLEN + 1];
	int rate;
} ACPSPortControllerParam;

extern int acpspcParam_set(ACPSPortControllerParam *self, const char *serial_filename, int serial_rate, const char *serial_config);

extern void acpspcParam_free(ACPSPortControllerParam *self);

#endif 
