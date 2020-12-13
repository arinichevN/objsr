#include "main.h"

void channel_RUN(Channel * self) {
#ifdef MODE_DEBUG
	//printf("channel: id:%d amtemp:%.2f mass:%.2f ksh:%.2f kl:%.2f pl:%.2f \n",
			//self->sensor.id,
			//self->ambient_temperature,
			//self->matter.mass,
			//self->matter.ksh,
			//self->matter.kl,
			//self->matter.pl
			
		  //);
	//FTS tempr  = sensor_getOutput(&self->sensor);
	//printf("channel: id:%d temp:%.2f energy:%.2f power(h c):%.2f %.2f \n",
			 //self->sensor.id,
			 //tempr.value,
			 //self->matter.energy,
			 //em_getPower(&self->heater),
			 //em_getPower(&self->cooler)
		  //);
#endif
	mutex_lock(&self->mutex);
	CONTROL_N(self->sensor);
	CONTROL_N(self->heater);
	CONTROL_N(self->cooler);
	matter_control(&self->matter, self->ambient_temperature, em_getPower(&self->heater), em_getPower(&self->cooler));
	mutex_unlock(&self->mutex);
}

void channel_OFF(Channel * self) {
	;
}

void channel_start(Channel * self) {
	sensor_start(&self->sensor);
	em_start(&self->heater);
	em_start(&self->cooler);
	matter_start(&self->matter, self->ambient_temperature);
	self->control = channel_RUN;
}

void channel_stop(Channel * self) {
	sensor_stop(&self->sensor);
	em_stop(&self->heater);
	em_stop(&self->cooler);
	self->control = channel_OFF;
}

void channel_setParam(Channel *self, const ChannelParam *param){
	self->ambient_temperature = param->ambient_temperature;
	matter_setParam(&self->matter, &param->matter);
	sensor_setParam(&self->sensor, &param->sensor);
	em_setParam(&self->heater, &param->heater);
	em_setParam(&self->cooler, &param->cooler);
	self->cycle_duration = param->cycle_duration;
}

#ifdef MODE_DEBUG
void channel_cleanup_handler(void *arg) {
	Channel *self = arg;
	printf("cleaning up sensor_id %d\n", self->sensor.id);
}
#endif

void *channel_main (void *arg) {
	Channel *self = arg;
	printdo("channel with serial_id=%d has been started\n", self->sensor.id);
#ifdef MODE_DEBUG
    pthread_cleanup_push(channel_cleanup_handler, self);
#endif
	while(1) {
		struct timespec t1 = getCurrentTime();
		CONTROL(self);
		delayTsIdleRest(self->cycle_duration, t1);
	}
#ifdef MODE_DEBUG
	pthread_cleanup_pop(1);
#endif
}

Channel *channel_new() {
	size_t sz = sizeof (Channel);
	Channel *self = malloc(sz);
	if(self == NULL) {
		putsde("failed to allocate memory for new Channel\n");
		return self;
	}
	memset(self, 0, sz);
	return self;
}

int channel_begin(Channel *self){
	matter_begin(&self->matter);
	sensor_setSlave (&self->sensor, &self->matter.im_temperature);
	self->next = NULL;
	self->control = channel_OFF;
	if(!mutex_init(&self->mutex)) {
		free(self);
		return 0;
	}
	if(!thread_create(&self->thread, channel_main, self)) {
		mutex_free(&self->mutex);
		free(self);
		return 0;
	}
	channel_start(self);
	return 1;
}

void channel_free(Channel *self){
	printdo("channel ramp-down (sensor_id = %d):\n", self->sensor.id);
	STOP_THREAD(self->thread)
	mutex_free(&self->mutex);
	matter_free(&self->matter);
	free(self);
	putsdo("\tdone\n");
}




