#ifndef ACPS_PORT_CONTROLLER_H
#define ACPS_PORT_CONTROLLER_H

#include <dirent.h>
#include "../../../../dstructure.h"
#include "../../../../debug.h"
#include "../../../../app.h"
#include "../../../../serial.h"
#include "../../../main.h"
#include "../../main.h"
#include "../CommandNode.h"
#include "Param/main.h"

#define ACPSPC_CYCLE_DURATION_RUN			(struct timespec) {0, 70000UL}
#define ACPSPC_CYCLE_DURATION_TRY_OPEN		(struct timespec) {0, 700000000UL}

typedef struct acpsportcontroller_st ACPSPortController;
struct acpsportcontroller_st {
	int fd;
	char buf[ACP_BUF_MAX_LENGTH];
	Thread thread;
	Mutex mutex;
	ACPSPortControllerParam param;
	ACPLSCommandNode *acnodes;
	size_t acnodes_count;
	struct timespec cycle_duration;
	void (*control)(ACPSPortController *);
	ACPSPortController *next;
};

extern ACPSPortController *acpspc_newBegin(const char *serial_file_name, int serial_rate, const char *serial_config, ACPLSCommandNode *acnodes, size_t acnodes_count);

extern void acpspc_free(ACPSPortController *self);

#endif 
