#include <pebble.h>
#include "app_message.h"

#define SEL_COLOR COLOR_FALLBACK(GColorBlueMoon, GColorWhite)
#define BG_COLOR COLOR_FALLBACK(GColorDarkGray, GColorBlack)

#define INPUT_TOP_Y 68

#define FIELD_SPACING 37
#define FIELD_WIDTH 34
#define FIELD_HEIGHT 24

#define IP_PERSIST_KEY 0xdeadbeef


static Window *s_window; 
static bool s_ip_set = false;

static unsigned char s_ip[4] = {192,168,1,0};
static TextLayer *s_ip_layers[4];
static TextLayer *s_top_layer;

static Layer *s_plus_layer;
static Layer *s_minus_layer;

//TODO: don't think these are a good way to do this, use different config provider
static ClickHandler click_up_handler;
static ClickHandler click_back_handler;
static ClickHandler click_select_handler;
static ClickHandler click_down_handler;

static int s_current_field = 0;
static char s_value_buffers[4][8];

static void init();
static void config_provider(Window *window);

static void update_ui() {
    layer_set_bounds(s_plus_layer, GRect(s_current_field*FIELD_SPACING, 0, FIELD_WIDTH, FIELD_HEIGHT));
    layer_set_bounds(s_minus_layer, GRect(s_current_field*FIELD_SPACING, 0, FIELD_WIDTH, FIELD_HEIGHT));
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

static void render_plus(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);

    graphics_context_set_fill_color(ctx, GColorBlack);

    //graphics_fill_rect(ctx, GRect(0,0,FIELD_WIDTH, FIELD_HEIGHT), 0, GCornerNone);
    graphics_draw_line(ctx, GPoint(FIELD_WIDTH/4, FIELD_HEIGHT/2),
                            GPoint(3*FIELD_WIDTH/4, FIELD_HEIGHT/2));
    graphics_draw_line(ctx, GPoint(FIELD_WIDTH/2, FIELD_HEIGHT/4),
                            GPoint(FIELD_WIDTH/2, 3*FIELD_HEIGHT/4));
}

static void render_minus(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);
    graphics_context_set_fill_color(ctx, GColorBlack);

    //graphics_fill_rect(ctx, GRect(0,0,FIELD_WIDTH, FIELD_HEIGHT), 0, GCornerNone);
    graphics_draw_line(ctx, GPoint(FIELD_WIDTH/4, FIELD_HEIGHT/2),
                            GPoint(3*FIELD_WIDTH/4, FIELD_HEIGHT/2));
}

static void click_handler(ClickRecognizerRef crr, void *context) {
    send_message(click_recognizer_get_button_id(crr));
}

static void select_handler(ClickRecognizerRef crr, void *context) {
    s_current_field++;
    if (s_current_field > 3) {
        s_ip_set = true;

        persist_write_data(IP_PERSIST_KEY, s_ip, 4);
        send_ip(s_ip);

        click_down_handler = click_handler;
        click_up_handler = click_handler;
        click_select_handler = click_handler;
        window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider); 
    }
    update_ui();
}

static void up_handler(ClickRecognizerRef crr, void *context) {
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
}


static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);

    GRect plus_frame = GRect(0, INPUT_TOP_Y-FIELD_HEIGHT, 144, FIELD_HEIGHT);
    s_plus_layer = layer_create(plus_frame);
    layer_set_bounds(s_plus_layer, (GRect){ .size = GSize(FIELD_WIDTH, FIELD_HEIGHT) });
    layer_set_update_proc(s_plus_layer, render_plus);
    layer_add_child(window_layer, s_plus_layer);

    GRect minus_frame = GRect(0, INPUT_TOP_Y+FIELD_HEIGHT, 144, FIELD_HEIGHT);
    s_minus_layer = layer_create(minus_frame);
    layer_set_bounds(s_minus_layer, (GRect){ .size = GSize(FIELD_WIDTH, FIELD_HEIGHT) });
    layer_set_update_proc(s_minus_layer, render_minus);
    layer_add_child(window_layer, s_minus_layer);

    s_top_layer = text_layer_create(GRect(0,10,144,30));
    text_layer_set_text(s_top_layer, "Enter IP:");
    text_layer_set_text_alignment(s_top_layer, GTextAlignmentCenter);
    text_layer_set_font(s_top_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(s_top_layer));

    for (int i = 0; i<4; i++) {
        s_ip_layers[i] = text_layer_create(GRect(i * FIELD_SPACING, INPUT_TOP_Y, FIELD_WIDTH, FIELD_HEIGHT));
        text_layer_set_text_color(s_ip_layers[i], GColorWhite);
        text_layer_set_text(s_ip_layers[i], "0");
        text_layer_set_font(s_ip_layers[i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
        text_layer_set_text_alignment(s_ip_layers[i], GTextAlignmentCenter); 
        layer_add_child(window_layer, text_layer_get_layer(s_ip_layers[i]));
    }

    update_ui();
}

static void window_unload(Window *window) {
    for (int i = 0; i<4; i++) {
        text_layer_destroy(s_ip_layers[i]);
    }

    text_layer_destroy(s_top_layer);
    layer_destroy(s_plus_layer);
    layer_destroy(s_minus_layer);
    window_destroy(window);
    s_window = NULL;
}

static void init(void) {
    if (persist_exists(IP_PERSIST_KEY)) {
        persist_read_data(IP_PERSIST_KEY, s_ip, 4); 
    }

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
}

static void deinit(void) {
    app_message_deregister_callbacks();
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
