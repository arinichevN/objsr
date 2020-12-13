#ifndef MODEL_CHANNEL_LIST_H
#define MODEL_CHANNEL_LIST_H

#include "../../lib/debug.h"
#include "../../lib/dstructure.h"
#include "main.h"

DEC_LLISTM(Channel)

extern int channelList_push(Channel *item, ChannelLListm *list);

extern void channelList_free (ChannelLListm *list);

#endif
