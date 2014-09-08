#include <pebble.h>

Window *window;	
	
// Key values for AppMessage Dictionary
enum {
	STATUS_KEY = 0,	
	MESSAGE_KEY = 1
};

char *translate_error(AppMessageResult result) {
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

void accel_data_handler(AccelData *data, uint32_t num_samples) {
    
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", data->x);
    send_message(data->x);
}



// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
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
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "failed");
}
void click_handler(ClickRecognizerRef crr, void *context) {
	send_message(click_recognizer_get_button_id(crr));
}
void config_provider(Window *window) {
	window_raw_click_subscribe(BUTTON_ID_DOWN, NULL, click_handler, NULL);
	window_raw_click_subscribe(BUTTON_ID_UP, NULL, click_handler, NULL);
	window_raw_click_subscribe(BUTTON_ID_SELECT, NULL, click_handler, NULL);
}
void init(void) {
    APP_LOG(APP_LOG_LEVEL_DEBUG,"here goes");
	window = window_create();
	window_stack_push(window, true);
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
		
	window_set_click_config_provider(window, (ClickConfigProvider) config_provider);
}

void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}
