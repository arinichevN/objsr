#ifndef ACPS_COMMAND_NODE_H
#define ACPS_COMMAND_NODE_H

typedef struct {
	int command;
	int (*func) (char *buf);
} ACPLSCommandNode;

#endif 
