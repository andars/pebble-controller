#ifndef APP_MESSAGE_H
#define APP_MESSAGE_H

#include <pebble.h>

void send_message(int16_t val);
void send_ip(unsigned char ip[]);

void in_received_handler(DictionaryIterator *rec, void *context);

void in_dropped_handler(AppMessageResult reason, void *context);

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context);
#endif
