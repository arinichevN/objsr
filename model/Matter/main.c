#include "main.h"

#define PER_TIME(V,DTM)(V*DTM.tv_sec*1000.0 + V*DTM.tv_nsec/1000000.0)

#define VOID_TO_SELF Matter *self = (Matter *) vself;

void matter_control(Matter *self, double ambient_temperature, double heater_power, double cooler_power) {
	struct timespec dTM = getTimePassed_ts(self->t1);
	//printf("time passed: %ld, %ld ", dTM.tv_sec, dTM.tv_nsec);
	double temperature_k = self->energy / (self->ksh * self->mass);
	double ambient_temperature_k = 273 + ambient_temperature;
	double dE = 0.0;
	//aiming to ambient temperature
	double dTemp =(temperature_k > ambient_temperature_k) ?(temperature_k - ambient_temperature_k) :(ambient_temperature_k - temperature_k);
	//double aE=pow((dTemp*(self->kl)),(0.2 * dTemp));
	double aE = self->kl * pow(dTemp, self->pl);
	aE = PER_TIME(aE, dTM);
	if(temperature_k > ambient_temperature_k) {
		dE -= aE;
	} else if(temperature_k < ambient_temperature_k) {
		dE += aE;
	}
	//actuator affect
	double actuator_power = heater_power - cooler_power;
	dE += PER_TIME(actuator_power, dTM);
	//total affect
	self->energy += dE;
	if(self->energy < 0.0) {
		self->energy = 0.0;
		dE = 0.0;
	}
	//delay for temperature
	double temperature_c = temperature_k - 273;
	//printf("tempc: %.2f\n", temperature_c);
	if(self->temperature_pipe.length > 0) {
		PIPE_MOVE(&self->temperature_pipe, 0.0)
		PIPE_IN(&self->temperature_pipe, temperature_c)
		self->temperature = PIPE_OUT(&self->temperature_pipe);
	} else {
		self->temperature = temperature_c;
	}
	#ifdef MODE_DEBUG
	 //PIPE_PRINT(&self->temperature_pipe, "%.2f ")
	#endif
	self->t1 = getCurrentTime();
}

double matter_getTemperature(void *vself){
	Matter *self = (Matter *) vself;
	return self->temperature;
}

int matter_setParam(Matter *self, const MatterParam *param){
	PIPE_BEGIN(&self->temperature_pipe, param->temperature_pipe_length)
	if(self->temperature_pipe.length != param->temperature_pipe_length){
		return 0;
	}
	self->mass = param->mass;
	self->ksh = param->ksh;
	self->kl = param->kl;
	self->pl = param->pl;
	return 1;
}

void matter_start(Matter *self, double ambient_temperature){
	self->temperature = ambient_temperature;
	self->energy =(self->temperature + 273.0) * self->ksh * self->mass;
	PIPE_FILL(&self->temperature_pipe, self->temperature);
}

void matter_begin(Matter *self){
	self->im_temperature.self = self;
	self->im_temperature.get = matter_getTemperature;
}

void matter_free(Matter *self){
	PIPE_FREE(&self->temperature_pipe);
}

#undef VOID_TO_SELF
