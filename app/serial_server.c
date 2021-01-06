#include "serial_server.h"

#include "serial_server_config.c"

ACPSS *serial_server = NULL;

int getSerial_callback(void *data, int argc, char **argv, char **azColName){
	char name[LINE_SIZE + 1];
	memset(name, 0, sizeof name);
	int rate = -1;
	char config[SERIAL_CONFIG_STRLEN + 1];
	memset(config, 0, sizeof config);
	int c = 0;
	DB_FOREACH_COLUMN {
		if(DB_COLUMN_IS("name")){
			strncpy(name, DB_COLUMN_VALUE, LINE_SIZE);
			c++;
		} else if(DB_COLUMN_IS("rate")){
			rate = DB_CVI;
			c++;
		} else if(DB_COLUMN_IS("config")){
			strncpy(config, DB_COLUMN_VALUE, SERIAL_CONFIG_STRLEN);
			c++;
		} else {
			printde("unknown column(we will skip it): %s\n", DB_COLUMN_NAME);
		}
	}
#define N 3
	if(c != N){
		printde("required %d columns but %d found\n", N, c);
		return EXIT_FAILURE;
	}
#undef N
	size_t fn = strlen(name) + strlen(SS_DEVICE_DIR) + 2;
	printdo("FROM DB: %s %s\n", name, SS_DEVICE_DIR);
	char filename[fn] ;
	snprintf(filename, fn, "%s/%s", SS_DEVICE_DIR, name) ;
	if (!acpss_createNewPort(serial_server, filename, rate, config, acnodes, ACPL_CNODE_COUNT)) {
		printde("failed to initialize serial:%s at rate %d\n", filename, rate);
		return EXIT_FAILURE;
	}
	NANOSLEEP(0, 100000000);
	return EXIT_SUCCESS;
}

int serialServer_begin(const char *db_path){
	serial_server = acpss_new();
	if(serial_server == NULL){
		return 0;
	}
	sqlite3 *db;
	int r = db_openR(db_path, &db);
	if(!r){
		putsde(" failed\n");
		return 0;
	}
	if(!db_exec(db, "select * from serial", getSerial_callback, NULL)){
		putsde(" failed\n");
		db_close(db);
		return 0;
	}
	db_close(db);
	return 1;
}

extern void serialServer_free(){
	acpss_free(serial_server);
	serial_server = NULL;
}
