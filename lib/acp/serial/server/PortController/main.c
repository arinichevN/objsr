#include "main.h"

void acpspc_RUN(ACPSPortController *self);
void acpspc_TRY_OPEN(ACPSPortController *self);
void acpspc_OFF(ACPSPortController *self);
void acpspc_FAILURE(ACPSPortController *self);

void acpspc_RUN(ACPSPortController *self){
	char buf[ACP_BUF_MAX_LENGTH];
	memset(buf, 0, sizeof buf);
	switch(acpserial_readRequest(self->fd, buf, ACP_BUF_MAX_LENGTH)){
		case ACP_SUCCESS: break;
		case ACP_ERROR_DEVICE_DISCONNECTED:
			close(self->fd);
			self->cycle_duration = ACPSPC_CYCLE_DURATION_TRY_OPEN;
			self->control = acpspc_TRY_OPEN;
			return;
		default: return;
	}
	if(!acpserial_packCheckCRC(buf)){
		putsde("bad CRC\n");
		return;
	}
	char sign = buf[ACP_IND_SIGN];
	switch(sign){
		case ACP_SIGN_REQUEST_GET:				break;
		case ACP_SIGN_REQUEST_SET:				break;
		case ACP_SIGN_REQUEST_SET_BROADCAST:	break;
		case ACP_SIGN_REQUEST_GET_BROADCAST:	break;
		default:								putsde("bad sign\n"); return;
	}
	int cmd;
	if(!acp_packGetCellI(buf, ACP_REQUEST_IND_CMD, &cmd)){
		putsde("failed to get command\n");
		return;
	}
	//printdo("command: %d\n", cmd);
	for(size_t i = 0; i < self->acnodes_count; i++){
		ACPLSCommandNode *node = &self->acnodes[i];
		if(cmd == node->command){
			int r = node->func(buf);
			//putsdo("command found\n");
			if(r == ACP_SENDING_REQUIRED){
				acpserial_send(buf, self->fd);
				return;
			}
			return;
		}
		
	}
	putsde("command not found");
	
}

void acpspc_TRY_OPEN(ACPSPortController *self){
	if (!serial_init(&self->fd, self->param.filename, self->param.rate, self->param.config)) {
		printde("failed to initialize serial: %s %d %s\n", self->param.filename, self->param.rate, self->param.config);
		return;
	}
	printdo("serial opened: %s %d %s\n", self->param.filename, self->param.rate, self->param.config);
	self->cycle_duration = ACPSPC_CYCLE_DURATION_RUN;
	self->control = acpspc_RUN;
}


void acpspc_OFF(ACPSPortController *self){
	;
}

void acpspc_FAILURE(ACPSPortController *self){
	;
}

#ifdef MODE_DEBUG
void acpspc_cleanup_handler(void *arg) {
	ACPSPortController *self = arg;
	printf("cleaning up thread (fd=%s)\n", self->param.filename);
}
#endif

void *acpspc_main(void *arg) {
	ACPSPortController *self = arg;
	printdo("thread with fd=%d has been started\n", self->fd);
#ifdef MODE_DEBUG
    pthread_cleanup_push(acpspc_cleanup_handler, self);
#endif
	while(1) {
		struct timespec t1 = getCurrentTime();
		int old_state;
		if(threadCancelDisable(&old_state)) {
			CONTROL(self);
			threadSetCancelState(old_state);
		}
		delayTsIdleRest(self->cycle_duration, t1);
	}
#ifdef MODE_DEBUG
	pthread_cleanup_pop(1);
#endif
}

ACPSPortController *acpspc_new() {
	size_t sz = sizeof (ACPSPortController);
	ACPSPortController *self = malloc(sz);
	if(self == NULL) {
		putsde("failed to allocate memory for new ACPSPortController\n");
		return self;
	}
	memset(self, 0, sz);
	return self;
}

ACPSPortController *acpspc_newBegin(const char *serial_file_name, int serial_rate, const char *serial_config, ACPLSCommandNode *acnodes, size_t acnodes_count){
	ACPSPortController *self = acpspc_new();
	if(self == NULL){
		return NULL;
	}
	self->acnodes = acnodes;
	self->acnodes_count = acnodes_count;
	self->next = NULL;
	self->cycle_duration = ACPSPC_CYCLE_DURATION_TRY_OPEN;
	self->control = acpspc_TRY_OPEN;
	if(!acpspcParam_set(&self->param, serial_file_name, serial_rate, serial_config)){
		printde("bad serial param:%s %s at rate %d\n", serial_file_name, serial_config, serial_rate);
		free(self);
		return NULL;
	}
	if(!mutex_init(&self->mutex)) {
		free(self);
		return NULL;
	}
	if(!thread_create(&self->thread, acpspc_main, self)) {
		mutex_free(&self->mutex);
		free(self);
		return NULL;
	}
	
	return self;
}

void acpspc_free(ACPSPortController *self) {
	printdo("serial controller ramp-down (fd = %d):\n", self->fd);
	STOP_THREAD(self->thread)
	close(self->fd);
	mutex_free(&self->mutex);
	acpspcParam_free(&self->param);
	free(self);
	putsdo("\tdone");
}
