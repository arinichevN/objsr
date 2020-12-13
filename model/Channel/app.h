#ifndef MODEL_CHANNEL_APP_H
#define MODEL_CHANNEL_APP_H

#include "main.h"
#include "list.h"
#include "../../lib/app.h"
#include "../../lib/dbl.h"

extern ChannelLListm channels;

extern int channels_begin(const char *db_path);

extern void channels_free();

#endif
