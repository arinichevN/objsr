#include "llistm.h"

int channelList_push(Channel *item, ChannelLListm *list){
	if(list->length >= INT_MAX){
		printde("can not load channel with sensor_id=%d - list length exceeded\n", item->sensor.id);
		return 0;
	}
	if(list->top == NULL){
		mutex_lock(&list->mutex);
		list->top = item;
		mutex_unlock(&list->mutex);
	} else {
		mutex_lock(&list->last->mutex);
		list->last->next = item;
		mutex_unlock(&list->last->mutex);
	}
	list->last = item;
	list->length++;
	printdo("channel with sensor_id=%d loaded\n", item->sensor.id);
	return 1;
}
//returns deleted channel
Channel * channelList_deleteItem(Channel *item, ChannelLListm *list){
	Channel *prev = NULL;
	FOREACH_LLIST(curr,list,Channel){
		if(curr == item){
			if(prev != NULL){
				mutex_lock(&prev->mutex);
				prev->next = curr->next;
				mutex_unlock(&prev->mutex);
			} else {//curr=top
				mutex_lock(&list->mutex);
				list->top = curr->next;
				mutex_unlock(&list->mutex);
			}
			if(curr == list->last){
				list->last = prev;
			}
			list->length--;
			return curr;
		}
		prev = curr;
	}
	return NULL;
}

void channelList_free (ChannelLListm *list){
	Channel *item = list->top, *temp;
	while(item != NULL) {
		temp = item;
		item = item->next;
		channel_free(temp);
	}
	list->top = NULL;
	list->last = NULL;
	list->length = 0;
	mutex_free(&list->mutex);
}
