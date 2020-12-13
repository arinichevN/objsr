#include "main.h"

int channelParam_check(const ChannelParam *self){
	if(!matterParam_check(&self->matter)){
		return 0;
	}
	if(!sensorParam_check(&self->sensor)){
		return 0;
	}
	if(!emParam_check(&self->heater)){
		return 0;
	}
	if(!emParam_check(&self->cooler)){
		return 0;
	}
	return 1;
}
