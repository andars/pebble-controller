#include "app_message.h"

// Key values for AppMessage Dictionary
enum {
    STATUS_KEY = 0,    
    MESSAGE_KEY = 1,
    IP_KEY = 2 
};

static char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY: return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
    default: return "UNKNOWN ERROR";
  }
}

void send_ip(unsigned char ip[]) {
    Tuplet value = TupletBytes(IP_KEY, ip, 4);
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
     
    if (iter == NULL) {
        return;
    }

    dict_write_tuplet(iter, &value);
    dict_write_end(iter);
    AppMessageResult res = app_message_outbox_send();
    APP_LOG(APP_LOG_LEVEL_DEBUG, translate_error(res));
}

// Write message to buffer & send
void send_message(int16_t val){
    Tuplet value = TupletInteger(MESSAGE_KEY, val);
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    if (iter == NULL) {
        return;
    }

    dict_write_tuplet(iter, &value);
    dict_write_end(iter);
    AppMessageResult res = app_message_outbox_send();
    APP_LOG(APP_LOG_LEVEL_DEBUG, translate_error(res));

}

// Called when a message is received from PebbleKitJS
void in_received_handler(DictionaryIterator *received, void *context) {
    Tuple *tuple;
    
    tuple = dict_find(received, STATUS_KEY);
    if(tuple) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32); 
    }
    
    tuple = dict_find(received, MESSAGE_KEY);
    if(tuple) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
    }
}

// Called when an incoming message from PebbleKitJS is dropped
void in_dropped_handler(AppMessageResult reason, void *context) {    
}

// Called when PebbleKitJS does not acknowledge receipt of a message
void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "failed");
}

