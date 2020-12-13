#include "main.h"

void em_SECURED(EM *self){
	;
}

void em_RUN(EM *self){
	if(ton(&self->tmr)){
		self->power = 0.0;
		self->control = em_SECURED;
	}
}

void em_OFF(EM *self){
	;
}

void em_setParam(EM *self, const EMParam *param){
	self->id = param->id;
	ton_setInterval((struct timespec){3, 0}, &self->tmr);
	self->control = em_OFF;
}

void em_setPower(EM *self, double v){
	self->power = v;
	ton_reset(&self->tmr);
	self->control = em_RUN;
}

double em_getPower(EM *self){
	return self->power;
}

void em_start(EM *self){
	ton_reset(&self->tmr);
	self->control = em_RUN;
}

void em_stop(EM *self){
	self->control = em_OFF;
}

const char *em_getStateStr(EM *self){
	if(self->control == em_RUN){
		return "RUN";
	}else if(self->control == em_OFF){
		return "OFF";
	}else if(self->control == em_SECURED){
		return "SECURED";
	}
	return UNKNOWN_STR;
}

const char *em_getErrorStr(EM *self){
	return "no";
}
