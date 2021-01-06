#ifndef APP_COMMON_H
#define APP_COMMON_H

#define APP_ID			5

#define APP_NAME objsr
#define APP_NAME_STR TOSTRING(APP_NAME)

#ifdef MODE_FULL
#define DB_PATH "/etc/controller/" APP_NAME_STR "/data.db"
#endif

#ifndef MODE_FULL
#define DB_PATH "./db/data.db"
#endif

#define SS_DEVICE_DIR "/dev"

#endif 
