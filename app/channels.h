#ifndef APP_CHANNELS_H
#define APP_CHANNELS_H

#include "../model/Channel/main.h"
#include "../model/Channel/llistm.h"
#include "../lib/app.h"
#include "../lib/dbl.h"

extern ChannelLListm channels;

extern int channels_begin(const char *db_path);

extern void channels_free();

#endif
