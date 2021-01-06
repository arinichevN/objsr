#ifndef APP_SERIAL_SERVER_H
#define APP_SERIAL_SERVER_H

#include "../lib/debug.h"
#include "../lib/app.h"
#include "../lib/controller.h"
#include "../lib/dbl.h"
#include "../lib/ACP/serial/server/ACPSS.h"
#include "common.h"

extern ACPSS *serial_server;

extern int serialServer_begin(const char *db_path);

extern void serialServer_free();

#endif 
