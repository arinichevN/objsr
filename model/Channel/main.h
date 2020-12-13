#ifndef MODEL_CHANNEL_H
#define MODEL_CHANNEL_H

#include "../../lib/debug.h"
#include "../../lib/dstructure.h"
#include "../../lib/app.h"
#include "../../lib/controller.h"
#include "../ChannelParam/main.h"
#include "../Matter/main.h"
#include "../EM/main.h"
#include "../Sensor/main.h"

typedef struct channel_st Channel;
struct channel_st {
	double ambient_temperature;
	Matter matter;
	Sensor sensor;
	EM heater;
	EM cooler;
	void (*control)(Channel *);
	struct timespec cycle_duration;
	Thread thread;
	Mutex mutex;
	Channel *next;
};

extern void channel_setParam(Channel *self, const ChannelParam *param);

extern Channel *channel_new();

extern int channel_begin(Channel *self);

extern void channel_free(Channel *self);

#endif
