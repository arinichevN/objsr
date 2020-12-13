#ifndef MODEL_EM_PARAM_H
#define MODEL_EM_PARAM_H

typedef struct {
	int id;
} EMParam;

extern int emParam_check(const EMParam *self);

#endif 
