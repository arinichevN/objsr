#ifndef MODEL_I_SENSE_H
#define MODEL_I_SENSE_H

typedef struct{
	void *self;
	double (*get)(void *);
} iSense;

#endif 
