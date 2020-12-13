#include "main.h"

int matterParam_check(const MatterParam *self) {
	if(self->mass <= 0){
		putsde("expected matter_mass > 0 \n");
		return 0;
	}
	if(self->ksh <= 0){
		putsde("expected matter_ksh > 0 \n");
		return 0;
	}
	if(self->kl < 0){
		putsde("expected loss_factor >= 0 \n");
		return 0;
	}
	if(self->pl < 0){
		putsde("expected loss_power >= 0 \n");
		return 0;
	}
	if(self->temperature_pipe_length < 0){
		putsde("expected temperature_pipe_length >= 0\n");
		return 0;
	}
	return 1;
}
