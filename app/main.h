#ifndef APP_H
#define APP_H

#include "../lib/debug.h"
#include "../lib/app.h"
#include "serial_server.h"
#include "../model/Channel/app.h"

#define APP_ID			5

#define APP_NAME objsr
#define APP_NAME_STR TOSTRING(APP_NAME)

#ifdef MODE_FULL
#define DB_PATH "/etc/controller/" APP_NAME_STR "/data.db"
#endif

#ifndef MODE_FULL
#define DB_PATH "./db/data.db"
#endif

extern void (*app_control)();
extern void app_begin();
extern void app_reset();
extern const char *app_getStateStr();
extern const char *app_getErrorStr();

#endif 
