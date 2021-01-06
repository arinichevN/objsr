#include "main.h"
#include "../lib/ACP/command/main.h"
#include "channels.h"

extern ChannelLListm channels;

ACPSSCommandNode *srvc_getServerCommandNext(int prev_command);
ACPSSCommandNode *srvc_getServerCommand(int command);
int srvc_getServerCommandCount();

Channel *srvc_getChannel(char *buf){
	int id;
	if(acp_packGetCellI(buf, ACP_REQUEST_IND_ID, &id)){
		//printdo("id: %d\n", id);
		FOREACH_CHANNEL {
			if(id == channel->sensor.id){
				return channel;
			}
		}
		putsde("\tchannel not found");
		return NULL;
	}
	putsde("\tfailed to get channel_id");
	return NULL;
}

Channel *srvc_getChannelBySensorId(char *buf){
	int id;
	if(acp_packGetCellI(buf, ACP_REQUEST_IND_ID, &id)){
		//printdo("id: %d\n", id);
		FOREACH_CHANNEL {
			if(id == channel->sensor.id){
				return channel;
			}
		}
		putsde("\tchannel not found");
		return NULL;
	}
	putsde("\tfailed to get channel_id");
	return NULL;
}

int srvc_forThisApp(char *buf){
	int id;
	if(acp_packGetCellI(buf, ACP_REQUEST_IND_ID, &id)){
		//printdo("id: %d\n", id);
		if(id == APP_ID){
			return 1;
		}else{
			putsde("\tnot for this app");
			return 0;
		}
	}
	putsde("\tfailed to get app_id");
	return 0;
}

int srvc_getrAppFieldSF(char *buf, const char * (*getfunc)()){
	if(!srvc_forThisApp(buf)) {return ACP_NOT_FOUND;}
	const char *v = getfunc();
	if(acpserial_buildPackIS(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, APP_ID, v)) return ACP_SENDING_REQUIRED;
	return ACP_ERROR;
}

int acnf_getIdExists(char *buf){
	int id;
	if(!acp_packGetCellI(buf, ACP_REQUEST_IND_ID, &id)){
		return ACP_ERROR;
	}
	//printdo("id: %d\n", id);
	if(id == APP_ID)	goto success;
	FOREACH_CHANNEL {
		if(id == channel->sensor.id) {
			goto success;
		} else if(id == channel->heater.id) {
			goto success;
		} else if(id == channel->cooler.id) {
			goto success;
		}
	}
	printde("id not found: %d\n", id);
	return ACP_NOT_FOUND;
	success:
	if(acpserial_buildPackII(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, id, 1)) return ACP_SENDING_REQUIRED;
	return ACP_ERROR;
}

int acnf_getFTS (char *buf){
	Channel *channel = srvc_getChannelBySensorId(buf);
	if(channel == NULL) return ACP_NOT_FOUND;
	mutex_lock(&channel->mutex);
	FTS fts = sensor_getOutput(&channel->sensor);
	mutex_unlock(&channel->mutex);
	if(acpserial_buildPackIFTS(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->sensor.id, &fts)) return ACP_SENDING_REQUIRED;
	return ACP_ERROR;
}

int acnf_setGoal(char *buf){
	double v;
	if(!acp_packGetCellF(buf, ACP_REQUEST_IND_PARAM1, &v)){
		return ACP_ERROR;
	}
	int id;
	if(!acp_packGetCellI(buf, ACP_REQUEST_IND_ID, &id)){
		return ACP_ERROR;
	}
	//printdo("id: %d\n", id);
	FOREACH_CHANNEL {
		if(id == channel->heater.id){
			mutex_lock(&channel->mutex);
			em_setPower(&channel->heater, v);
			mutex_unlock(&channel->mutex);
			return ACP_DONE;
		}
		if(id == channel->cooler.id){
			mutex_lock(&channel->mutex);
			em_setPower(&channel->cooler, v);
			mutex_unlock(&channel->mutex);
			return ACP_DONE;
		}
	}
	return ACP_NOT_FOUND;
}

int acnf_getStateStr(char *buf){
	int id;
	if(!acp_packGetCellI (buf, ACP_REQUEST_IND_ID, &id)) {
		return ACP_ERROR;
	}
	//printdo("id: %d\n", id);
	const char *out = UNKNOWN_STR;
	FOREACH_CHANNEL {
		if(id == channel->sensor.id) {
			out = sensor_getStateStr(&channel->sensor);
			goto success;
		} else if(id == channel->heater.id) {
			out = em_getStateStr(&channel->heater);
			goto success;
		} else if(id == channel->cooler.id) {
			out = em_getStateStr(&channel->cooler);
			goto success;
		}
	}
	return ACP_NOT_FOUND;
	success:
	if (acpserial_buildPackIS(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, id, out)) return ACP_SENDING_REQUIRED;
	return ACP_ERROR;
}

int acnf_getErrorStr(char *buf){
	int id;
	if(!acp_packGetCellI (buf, ACP_REQUEST_IND_ID, &id)) {
		return ACP_ERROR;
	}
	//printdo("id: %d\n", id);
	const char *out = UNKNOWN_STR;
	FOREACH_CHANNEL {
		if(id == channel->sensor.id) {
			out = sensor_getErrorStr(&channel->sensor);
			goto success;
		} else if(id == channel->heater.id) {
			out = em_getErrorStr(&channel->heater);
			goto success;
		} else if(id == channel->cooler.id) {
			out = em_getErrorStr(&channel->cooler);
			goto success;
		}
	}
	return ACP_NOT_FOUND;
	success:
	if (acpserial_buildPackIS(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, id, out)) return ACP_SENDING_REQUIRED;
	return ACP_ERROR;
}

int acnf_getDeviceKind(char *buf){
	int id;
	if(!acp_packGetCellI (buf, ACP_REQUEST_IND_ID, &id)) {
		return ACP_ERROR;
	}
	//printdo("id: %d\n", id);
	int out = DEVICE_KIND_UNKNOWN;
	FOREACH_CHANNEL {
		if(id == channel->sensor.id) {
			out = DEVICE_KIND_SENSOR;
			goto success;
		} else if(id == channel->heater.id) {
			out = DEVICE_KIND_EM;
			goto success;
		} else if(id == channel->cooler.id) {
			out = DEVICE_KIND_EM;
			goto success;
		}
	}
	return ACP_NOT_FOUND;
	success:
	if (acpserial_buildPackIII(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, id, out, 1)) return ACP_SENDING_REQUIRED; 
	return ACP_ERROR;
}

int acnf_getrDeviceKind(char *buf){
	int id;
	if(!acp_packGetCellI (buf, ACP_REQUEST_IND_ID, &id)) {
		return ACP_ERROR;
	}
	//printdo("id: %d\n", id);
	int out = DEVICE_KIND_UNKNOWN;
	FOREACH_CHANNEL {
		if(id == channel->sensor.id) {
			out = DEVICE_KIND_SENSOR;
			goto success;
		} else if(id == channel->heater.id) {
			out = DEVICE_KIND_EM;
			goto success;
		} else if(id == channel->cooler.id) {
			out = DEVICE_KIND_EM;
			goto success;
		}
	}
	return ACP_NOT_FOUND;
	success:
	if (acpserial_buildPackII(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, id, out)) return ACP_SENDING_REQUIRED; 
	return ACP_ERROR;
}

int acnf_start(char *buf){
	int id;
	if(!acp_packGetCellI(buf, ACP_REQUEST_IND_ID, &id)){
		return ACP_ERROR;
	}
	//printdo("id: %d\n", id);
	FOREACH_CHANNEL {
		if(id == channel->sensor.id){
			mutex_lock(&channel->mutex);
			sensor_start(&channel->sensor);
			mutex_unlock(&channel->mutex);
			return ACP_DONE;
		}
		if(id == channel->heater.id){
			mutex_lock(&channel->mutex);
			em_start(&channel->heater);
			mutex_unlock(&channel->mutex);
			return ACP_DONE;
		}
		if(id == channel->cooler.id){
			mutex_lock(&channel->mutex);
			em_start(&channel->cooler);
			mutex_unlock(&channel->mutex);
			return ACP_DONE;
		}
	}
	return ACP_NOT_FOUND;
}

int acnf_stop(char *buf){
	int id;
	if(!acp_packGetCellI(buf, ACP_REQUEST_IND_ID, &id)){
		return ACP_ERROR;
	}
	//printdo("id: %d\n", id);
	FOREACH_CHANNEL {
		if(id == channel->sensor.id){
			mutex_lock(&channel->mutex);
			sensor_stop(&channel->sensor);
			mutex_unlock(&channel->mutex);
			return ACP_DONE;
		}
		if(id == channel->heater.id){
			mutex_lock(&channel->mutex);
			em_stop(&channel->heater);
			mutex_unlock(&channel->mutex);
			return ACP_DONE;
		}
		if(id == channel->cooler.id){
			mutex_lock(&channel->mutex);
			em_stop(&channel->cooler);
			mutex_unlock(&channel->mutex);
			return ACP_DONE;
		}
	}
	return ACP_NOT_FOUND;
}

int acnf_getAppAcpCommandExists(char *buf){
	if(!srvc_forThisApp(buf)) return ACP_NOT_FOUND;
	int command;
	int success = 0;
	if(!acp_packGetCellI(buf, ACP_REQUEST_IND_PARAM1, &command)){
		goto done;
	}
	{ACPSSCommandNode *node = srvc_getServerCommand(command);
	if(node != NULL) {
		success = 1;
	}}
	done:
	if(acpserial_buildPackIII(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, APP_ID, command, success)) return ACP_SENDING_REQUIRED; 
	return ACP_ERROR;
}

int acnf_getAppServerCommandCount(char *buf){
	if(!srvc_forThisApp(buf)) return ACP_NOT_FOUND;
	int c = srvc_getServerCommandCount();
	if(acpserial_buildPackII(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 APP_ID, c)) return ACP_SENDING_REQUIRED; 
	return ACP_ERROR;
}

int acnf_getAppServerCommandFirst(char *buf){
	if(!srvc_forThisApp(buf)) return ACP_NOT_FOUND;
	extern ACPSSCommandNode acnodes[];
	int first_command = acnodes[0].command;
	if(acpserial_buildPackII(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 APP_ID, first_command)) return ACP_SENDING_REQUIRED; 
	return ACP_ERROR;
}

int acnf_getAppServerCommandNext(char *buf){
	if(!srvc_forThisApp(buf)) return ACP_NOT_FOUND;
	int prev_command;
	int next_command = 0;
	int success = 0;
	if(!acp_packGetCellI(buf, ACP_REQUEST_IND_PARAM1, &prev_command)){
		goto done;
	}
	{ACPSSCommandNode *node = srvc_getServerCommandNext(prev_command);
	if(node != NULL){
		success = 1;
		next_command = node->command;
	}}
	done:
	if(acpserial_buildPackIII(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, APP_ID, next_command, success)) return ACP_SENDING_REQUIRED; 
	return ACP_ERROR;
}

int acnf_getAppId(char *buf){
	if(acpserial_buildPackII(buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, APP_ID, 1)) return ACP_SENDING_REQUIRED; 
	return ACP_ERROR;
}

int acnf_getAppState(char *buf){return srvc_getrAppFieldSF(buf, &app_getStateStr);}
int acnf_getAppError(char *buf){return srvc_getrAppFieldSF(buf, &app_getErrorStr);}


int acnf_appReset(char *buf){
	if(!srvc_forThisApp(buf)) return ACP_NOT_FOUND;
	app_reset();
	return ACP_DONE;
}


ACPSSCommandNode acnodes[] = {
	
	{CMD_GETR_CHANNEL_FTS,						acnf_getFTS},
	{CMD_SET_CHANNEL_GOAL,						acnf_setGoal},
	{CMD_GETR_CHANNEL_STATE,					acnf_getStateStr},
	{CMD_GETR_CHANNEL_ERROR,					acnf_getErrorStr},


	{CMD_GET_ID_EXISTS,							acnf_getIdExists},
	
	{CMD_GET_APP_ACP_COMMAND_EXISTS,			acnf_getAppAcpCommandExists},
	
	{CMD_CHANNEL_START,							acnf_start},
	{CMD_CHANNEL_STOP,							acnf_stop},

	{CMD_GET_CHANNEL_DEVICE_KIND,				acnf_getDeviceKind},
	{CMD_GETR_CHANNEL_DEVICE_KIND,				acnf_getrDeviceKind},


	{CMD_APP_RESET,								acnf_appReset},

};

#define ACPL_CNODE_COUNT (sizeof acnodes / sizeof acnodes[0])

int srvc_getServerCommandCount(){
	return ACPL_CNODE_COUNT;
}

ACPSSCommandNode *srvc_getServerCommandNext(int prev_command){
	int found = 0;
	for(size_t i = 0; i<ACPL_CNODE_COUNT; i++){
		if(found) return &acnodes[i];
		if(acnodes[i].command == prev_command){
			found = 1;
			continue; 
		}
	}
	return NULL;
}

ACPSSCommandNode *srvc_getServerCommand(int command){
	for(size_t i = 0; i<ACPL_CNODE_COUNT; i++){
		if(acnodes[i].command == command){
			return &acnodes[i];
		}
	}
	return NULL;
}

