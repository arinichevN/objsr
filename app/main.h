#ifndef APP_H
#define APP_H

#include "../lib/debug.h"
#include "../lib/app.h"
#include "serial_server.h"
#include "channels.h"
#include "common.h"

extern void (*app_control)();
extern void app_begin();
extern void app_reset();
extern const char *app_getStateStr();
extern const char *app_getErrorStr();

#endif 
