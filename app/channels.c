#include "channels.h"

ChannelLListm channels = LLISTM_INITIALIZER;

int channels_addNew(const ChannelParam *param){
	Channel *item = channel_new();
	if(item == NULL){
		return 0;
	}
	channel_setParam(item, param);
	if(!channel_begin(item)){
		return 0;
	}
	if(!channelList_push(item, &channels)){
		channel_free(item);
		return 0;
	}

	return 1;
}

int getChannel_callback(void *data, int argc, char **argv, char **azColName){
	ChannelParam param;
	int c = 0;
	DB_FOREACH_COLUMN {
		if(DB_COLUMN_IS("sensor_id")){
			param.sensor.id = DB_CVI;
			c++;
		} else if(DB_COLUMN_IS("heater_id")){
			param.heater.id = DB_CVI;
			c++;
		} else if(DB_COLUMN_IS("cooler_id")){
			param.cooler.id = DB_CVI;
			c++;
		} else if(DB_COLUMN_IS("ambient_temperature")){
			param.ambient_temperature = DB_CVF;
			c++;
		} else if(DB_COLUMN_IS("matter_mass")){
			param.matter.mass = DB_CVF;
			c++;
		} else if(DB_COLUMN_IS("matter_ksh")){
			param.matter.ksh = DB_CVF;
			c++;
		} else if(DB_COLUMN_IS("loss_factor")){
			param.matter.kl = DB_CVF;
			c++;
		} else if(DB_COLUMN_IS("loss_power")){
			param.matter.pl = DB_CVF;
			c++;
		} else if(DB_COLUMN_IS("temperature_pipe_length")){
			param.matter.temperature_pipe_length = DB_CVI;
			c++;
		} else if(DB_COLUMN_IS("cycle_duration_sec")){
			param.cycle_duration.tv_sec = DB_CVI;
			c++;
		} else if(DB_COLUMN_IS("cycle_duration_nsec")){
			param.cycle_duration.tv_nsec = DB_CVI;
			c++;
		} else {
			printde("unknown column(we will skip it): %s\n", DB_COLUMN_NAME);
		}
	}
#define N 11
	if(c != N){
		printde("required %d columns but %d found\n", N, c);
		return EXIT_FAILURE;
	}
#undef N
	if(!channelParam_check(&param)){
		return EXIT_FAILURE;
	}
	if(!channels_addNew(&param)){
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int channels_begin(const char *db_path){
	sqlite3 *db;
	int r = db_openR(db_path, &db);
	if(!r){
		putsde(" failed\n");
		return 0;
	}
	if(!db_exec(db, "select * from channel", getChannel_callback, NULL)){
		putsde(" failed\n");
		db_close(db);
		return 0;
	}
	db_close(db);
	return 1;
}

void channels_free(){
	channelList_free (&channels);
}
