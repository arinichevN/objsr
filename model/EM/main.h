#ifndef MODEL_EM_H
#define MODEL_EM_H

#include "../../lib/app.h"
#include "../../lib/timef.h"
#include "param.h"

typedef struct em_st EM;

struct em_st {
	int id;
	double power;
	Ton tmr;
	void (*control)(EM *);
};

extern void em_setParam(EM *self, const EMParam *param);

extern void em_setPower(EM *self, double v);

extern double em_getPower(EM *self);

extern void em_start(EM *self);

extern void em_stop(EM *self);

extern const char *em_getStateStr(EM *self);

extern const char *em_getErrorStr(EM *self);

#endif
