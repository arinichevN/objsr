#include "main.h"

void app_RUN();
void app_INIT();
void app_INIT_MODEL();
void app_STOP();
void app_RESET();
void app_EXIT();
void app_OFF();

void app_exit();

void (*app_control)() = app_INIT;

void app_begin() {
#ifndef MODE_DEBUG
	daemon(0, 0);
#endif
	conSig(&app_exit);
}

int app_init() {
	if(!serialServer_begin(DB_PATH)) {
		putsde("failed to initialize serial_server\n");
		return 0;
	}
	return 1;
}

int app_initModel() {
	if(!channels_begin(DB_PATH)) {
		channels_free();
		goto failed;
	}
	return 1;
	failed:
	return 0;
}

void app_freeModel() {
	channels_free();
}

void app_free() {
	app_freeModel();
	serialServer_free();
}

void app_exit() {
	app_free();
	putsdo("\nexiting now...\n");
	exit(EXIT_SUCCESS);
}

void app_reset(){
	app_control = app_RESET;
}

const char *app_getStateStr(){
	if(app_control == app_RUN)				return "RUN";
	else if(app_control == app_INIT)		return "INIT";
	else if(app_control == app_INIT_MODEL)	return "INIT_DATA";
	else if(app_control == app_STOP)		return "STOP";
	else if(app_control == app_RESET)		return "RESET";
	else if(app_control == app_EXIT)		return "EXIT";
	else if(app_control == app_OFF)			return "OFF";
	return "?";
}

const char *app_getErrorStr(){
	return "NO";
}

void app_RUN(){
	nanosleep(&(struct timespec) {0,10000000}, NULL);
}

void app_INIT(){
	if(!app_init()) {
		exit(EXIT_FAILURE);
	}
	app_control = app_INIT_MODEL;
}

void app_INIT_MODEL(){
	app_initModel();
	app_control = app_RUN;
	delayUsIdle(1000000);
}

void app_STOP(){
	app_freeModel();
	app_control = app_OFF;
}

void app_RESET(){
	app_free();
	delayUsIdle(1000000);
	app_control = app_INIT;
}

void app_EXIT(){
	app_exit();
}

void app_OFF(){
	nanosleep(&(struct timespec) {0,10000000}, NULL);
}

