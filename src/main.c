#include <pebble.h>
#include "app_message.h"

#define SEL_COLOR COLOR_FALLBACK(GColorBlueMoon, GColorWhite)

#define BG_COLOR COLOR_FALLBACK(GColorDarkGray, GColorBlack)

#define FIELD_SPACING 37

static Window *s_window; 
static bool s_ip_set = false;

static unsigned char s_ip[4] = {192,168,1,0};
static TextLayer *s_ip_layers[4];
static TextLayer *s_top_layer;

static ClickHandler click_up_handler;
static ClickHandler click_back_handler;
static ClickHandler click_select_handler;
static ClickHandler click_down_handler;

static int s_current_field = 0;
static char s_value_buffers[4][8];

static void init();
static void config_provider(Window *window);

static void update_ui() {
    for (int i = 0; i<4; i++) {
        GColor color = BG_COLOR;
        if (s_current_field == i) {
            color = SEL_COLOR;
#ifdef PBL_BW
            text_layer_set_text_color(s_ip_layers[i], GColorBlack);
#endif
        } else {
            text_layer_set_text_color(s_ip_layers[i], GColorWhite);
        }
        text_layer_set_background_color(s_ip_layers[i], color); 
        snprintf(s_value_buffers[i], 4, "%d", s_ip[i]);
        text_layer_set_text(s_ip_layers[i], s_value_buffers[i]);
    }
}

static void click_handler(ClickRecognizerRef crr, void *context) {
    send_message(click_recognizer_get_button_id(crr));
}

static void select_handler(ClickRecognizerRef crr, void *context) {
    s_current_field++;
    if (s_current_field > 3) {
        s_ip_set = true;
        send_ip(s_ip);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "changing handlers");

        click_down_handler = click_handler;
        click_up_handler = click_handler;
        click_select_handler = click_handler;
        window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider); 

    }
    update_ui();
}

static void up_handler(ClickRecognizerRef crr, void *context) {
    //s_ip[s_current_field] = (s_ip[s_current_field]+1)%256;
    s_ip[s_current_field]++;

    update_ui();
}

static void down_handler(ClickRecognizerRef crr, void *context) {
    s_ip[s_current_field]--;

    update_ui();
}

static void config_provider(Window *window) {

    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 50, click_down_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_UP, 50, click_up_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_SELECT, 200, click_select_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, click_handler);

    /*
    window_raw_click_subscribe(BUTTON_ID_DOWN, NULL, click_handler, NULL);
    window_raw_click_subscribe(BUTTON_ID_UP, NULL, click_handler, NULL);
    window_raw_click_subscribe(BUTTON_ID_SELECT, NULL, click_handler, NULL);
    */
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    s_top_layer = text_layer_create(GRect(0,20,144,40));
    text_layer_set_text(s_top_layer, "Enter IP:");
    text_layer_set_text_alignment(s_top_layer, GTextAlignmentCenter);
    text_layer_set_font(s_top_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(s_top_layer));

    for (int i = 0; i<4; i++) {
        s_ip_layers[i] = text_layer_create(GRect(i * FIELD_SPACING, 68, 34, 24));
        text_layer_set_text_color(s_ip_layers[i], GColorWhite);
        text_layer_set_background_color(s_ip_layers[i], BG_COLOR);
        text_layer_set_text(s_ip_layers[i], "0");
        text_layer_set_font(s_ip_layers[i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
        text_layer_set_text_alignment(s_ip_layers[i], GTextAlignmentCenter); 
        layer_add_child(window_layer, text_layer_get_layer(s_ip_layers[i]));
    }
}

static void window_unload(Window *window) {
    for (int i = 0; i<4; i++) {
        text_layer_destroy(s_ip_layers[i]);
    }

    text_layer_destroy(s_top_layer);
    window_destroy(window);
    s_window = NULL;
}

static void init(void) {
    APP_LOG(APP_LOG_LEVEL_DEBUG,"here goes");
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider);
    window_stack_push(s_window, true);

    app_message_register_inbox_received(in_received_handler); 
    app_message_register_inbox_dropped(in_dropped_handler); 
    app_message_register_outbox_failed(out_failed_handler);
        
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

    update_ui();
}

static void deinit(void) {
    app_message_deregister_callbacks();
    window_destroy(s_window);
}

int main( void ) {
    click_down_handler = down_handler;
    click_up_handler = up_handler;
    click_select_handler = select_handler;
    click_back_handler = click_handler;

    init();
    app_event_loop();
    deinit();
}
