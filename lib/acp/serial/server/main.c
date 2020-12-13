#include "main.h"

int acpls_addController(ACPLS *self, ACPSPortController *controller) {
	ACPSPortControllerLListm *list = &self->port_controllers;
	if(list->length >= ACPLS_MAX_CONTROLLER_COUNT) {
		printde("can not add controller: list length (%zu) limit (%lu) \n", list->length, ACPLS_MAX_CONTROLLER_COUNT);
		return 0;
	}
	if(list->top == NULL) {
		mutex_lock(&list->mutex);
		list->top = controller;
		mutex_unlock(&list->mutex);
	} else {
		mutex_lock(&list->last->mutex);
		list->last->next = controller;
		mutex_unlock(&list->last->mutex);
	}
	list->last = controller;
	list->length++;
	putsdo("controller added to list\n");
	return 1;
}

ACPLS *acpls_new(){
	size_t sz = sizeof (ACPLS);
	ACPLS *self = malloc(sz);
	if(self == NULL) {
		putsde("failed to allocate memory for new ACPLS\n");
		return self;
	}
	memset(self, 0, sz);
	LLIST_RESET(&self->port_controllers)
	if(!mutex_init(&self->port_controllers.mutex)) {
		free(self);
		return NULL;
	}
	return self;
}

int acpls_createNewController(ACPLS *self, const char *serial_file_name, int serial_rate, const char *serial_config, ACPLSCommandNode *acnodes, size_t acnodes_count){
	ACPSPortController *new_controller = acpspc_newBegin(serial_file_name, serial_rate, serial_config, acnodes, acnodes_count);
	if(new_controller == NULL) {
		return 0;
	}
	if(!acpls_addController(self, new_controller)) {
		acpspc_free(new_controller);
		return 0;
	}
	return 1;
}

void acpls_free(ACPLS *self){
	ACPSPortControllerLListm *list = &self->port_controllers;
	ACPSPortController *item = list->top, *temp;
	while(item != NULL) {
		temp = item;
		item = item->next;
		acpspc_free(temp);
	}
	list->top = NULL;
	list->last = NULL;
	list->length = 0;
	mutex_free(&list->mutex);
	free(self);
}
