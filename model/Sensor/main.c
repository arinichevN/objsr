#include "main.h"

FTS sensor_read(Sensor *self){
	FTS out;
	out.value = self->slave->get(self->slave->self);
	out.tm = getCurrentTime();
	out.state = 1;
	return out;
}

void sensor_RUN(Sensor *self){
	if(tonr(&self->tmr)){
		//putsdo("sensor read\n");
		self->output = sensor_read(self);
	}
}

void sensor_OFF(Sensor *self){
	;
}

void sensor_setSlave (Sensor *self, iSense *slave) {
	self->slave = slave;
}

void sensor_setParam(Sensor *self, const SensorParam *param){
	self->id = param->id;
	ton_setInterval((struct timespec){1, 0}, &self->tmr);
	self->control = sensor_OFF;
}

void sensor_start(Sensor *self){
	ton_reset(&self->tmr);
	self->control = sensor_RUN;
}

void sensor_stop(Sensor *self){
	memset(&self->output, 0, sizeof self->output);
	self->control = sensor_OFF;
}

FTS sensor_getOutput(Sensor *self){
	return self->output;
}

const char *sensor_getStateStr(Sensor *self){
	if (self->control == sensor_RUN) {
		return "RUN";
	} else if(self->control == sensor_OFF) {
		return "OFF";
	}
	return UNKNOWN_STR;
}

const char *sensor_getErrorStr(Sensor *self){
	return "no";
}
