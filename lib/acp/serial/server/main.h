#ifndef LIB_ACP_SERVER_H
#define LIB_ACP_SERVER_H

#include "CommandNode.h"
#include "PortController/main.h"
#include "../../../dstructure.h"
#include "../../../debug.h"
#include "../../../app.h"

#define ACPLS_MAX_CONTROLLER_COUNT			16UL

DEC_LLISTM(ACPSPortController)

typedef struct {
	ACPSPortControllerLListm port_controllers;
} ACPLS;

extern ACPLS *acpls_new();

extern int acpls_createNewController(ACPLS *self, const char *serial_file_name, int serial_rate, const char *serial_config, ACPLSCommandNode *acnodes, size_t acnodes_count);

extern void acpls_free(ACPLS *self);

#endif
