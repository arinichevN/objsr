#ifndef MODEL_MATTER_PARAM_H
#define MODEL_MATTER_PARAM_H

#include "../../lib/debug.h"

typedef struct {
	double mass;
	double ksh;
	double kl;
	double pl;
	int temperature_pipe_length;
} MatterParam;

extern int matterParam_check(const MatterParam *self);

#endif 
