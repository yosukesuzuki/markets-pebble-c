#include <pebble.h>

#define KEY_N225_PRICE 0
#define KEY_N225_DIFF 1
#define KEY_N225_DIFFPERCENT 2
#define KEY_N225_NP 3
#define KEY_YENDOLLAR_PRICE 4
#define KEY_YENDOLLAR_DIFF 5
#define KEY_YENDOLLAR_DIFFPERCENT 6
#define KEY_YENDOLLAR_NP 7
#define KEY_YENEURO_PRICE 8
#define KEY_YENEURO_DIFF 9
#define KEY_YENEURO_DIFFPERCENT 10
#define KEY_YENEURO_NP 11

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *n225_title_layer;
static TextLayer *n225_layer;
static TextLayer *yendollar_title_layer;
static TextLayer *yendollar_layer;
static TextLayer *yeneuro_title_layer;
static TextLayer *yeneuro_layer;

static GFont s_custom_font;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}


static void main_window_load(Window *window) {
    // Create time TextLayer
    s_time_layer = text_layer_create(GRect(0, -1, 144, 20));
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_text(s_time_layer, "00:00");

    // Improve the layout to be more like a watchface
    text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

    // Create n225 Layer
    n225_title_layer = text_layer_create(GRect(0, 17, 144, 25));
    text_layer_set_background_color(n225_title_layer, GColorClear);
    text_layer_set_text_color(n225_title_layer, GColorWhite);
    text_layer_set_text_alignment(n225_title_layer, GTextAlignmentCenter);
    text_layer_set_text(n225_title_layer, "Nikkei225");

    // Create second custom font, apply it and add to Window
    s_custom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CUSTOM_FONT_22));
    text_layer_set_font(n225_title_layer, s_custom_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(n225_title_layer));

    // Create n225 Layer
    n225_layer = text_layer_create(GRect(0, 41, 144, 25));
    text_layer_set_background_color(n225_layer, GColorClear);
    text_layer_set_text_color(n225_layer, GColorWhite);
    text_layer_set_text_alignment(n225_layer, GTextAlignmentCenter);
    text_layer_set_text(n225_layer, "Loading...");

    // Create second custom font, apply it and add to Window
    text_layer_set_font(n225_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(n225_layer));

    // Create yendollar Layer
    yendollar_title_layer = text_layer_create(GRect(0, 69, 144, 25));
    text_layer_set_background_color(yendollar_title_layer, GColorClear);
    text_layer_set_text_color(yendollar_title_layer, GColorWhite);
    text_layer_set_text_alignment(yendollar_title_layer, GTextAlignmentCenter);
    text_layer_set_text(yendollar_title_layer, "USD/JPY");

    // Create second custom font, apply it and add to Window
    text_layer_set_font(yendollar_title_layer, s_custom_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(yendollar_title_layer));

    // Create n225 Layer
    yendollar_layer = text_layer_create(GRect(0, 93, 144, 25));
    text_layer_set_background_color(yendollar_layer, GColorClear);
    text_layer_set_text_color(yendollar_layer, GColorWhite);
    text_layer_set_text_alignment(yendollar_layer, GTextAlignmentCenter);
    text_layer_set_text(yendollar_layer, "Loading...");

    // Create second custom font, apply it and add to Window
    text_layer_set_font(yendollar_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(yendollar_layer));

    yeneuro_title_layer = text_layer_create(GRect(0, 121, 144, 25));
    text_layer_set_background_color(yeneuro_title_layer, GColorClear);
    text_layer_set_text_color(yeneuro_title_layer, GColorWhite);
    text_layer_set_text_alignment(yeneuro_title_layer, GTextAlignmentCenter);
    text_layer_set_text(yeneuro_title_layer, "EURO/JPY");

    // Create second custom font, apply it and add to Window
    text_layer_set_font(yeneuro_title_layer, s_custom_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(yeneuro_title_layer));

    // Create yeneuro Layer
    yeneuro_layer = text_layer_create(GRect(0, 145, 144, 25));
    text_layer_set_background_color(yeneuro_layer, GColorClear);
    text_layer_set_text_color(yeneuro_layer, GColorWhite);
    text_layer_set_text_alignment(yeneuro_layer, GTextAlignmentCenter);
    text_layer_set_text(yeneuro_layer, "Loading...");

    // Create second custom font, apply it and add to Window
    text_layer_set_font(yeneuro_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(yeneuro_layer));

    // Make sure the time is displayed from the start
    update_time();
}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
    // Destroy weather elements
    text_layer_destroy(n225_title_layer);
    text_layer_destroy(n225_layer);
    text_layer_destroy(yendollar_title_layer);
    text_layer_destroy(yendollar_layer);
    text_layer_destroy(yeneuro_title_layer);
    text_layer_destroy(yeneuro_layer);
    fonts_unload_custom_font(s_custom_font);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();

    // Get markets update every 5 minutes
    if(tick_time->tm_min % 30 == 0) {
        // Begin dictionary
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);

        // Add a key-value pair
        dict_write_uint8(iter, 0, 0);

        // Send the message!
        app_message_outbox_send();
    }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    // Store incoming information
    static char n225_buffer[32];
    static char n225_diff_buffer[32];
    static char n225_price_and_diff_buffer[32];
    static char n225_np[32];
    static char yendollar_buffer[32];
    static char yendollar_diff_buffer[32];
    static char yendollar_price_and_diff_buffer[32];
    static char yendollar_np[32];
    static char yeneuro_buffer[32];
    static char yeneuro_diff_buffer[32];
    static char yeneuro_price_and_diff_buffer[32];
    static char yeneuro_np[32];
    static char markets_layer_buffer[32];

    // Read first item
    Tuple *t = dict_read_first(iterator);

    // For all items
    while(t != NULL) {
        // Which key was received?
        switch(t->key) {
            case KEY_N225_PRICE:
                snprintf(n225_buffer, sizeof(n225_buffer), "%s", t->value->cstring);
                break;
            case KEY_N225_DIFF:
                snprintf(n225_diff_buffer, sizeof(n225_diff_buffer), "%s", t->value->cstring);
                break;
            case KEY_N225_NP:
                snprintf(n225_np, sizeof(n225_np), "%s", t->value->cstring);
                break;
            case KEY_YENDOLLAR_PRICE:
                snprintf(yendollar_buffer, sizeof(yendollar_buffer), "%s", t->value->cstring);
                break;
            case KEY_YENDOLLAR_DIFF:
                snprintf(yendollar_diff_buffer, sizeof(yendollar_diff_buffer), "%s", t->value->cstring);
                break;
            case KEY_YENDOLLAR_NP:
                snprintf(yendollar_np, sizeof(yendollar_np), "%s", t->value->cstring);
                break;
            case KEY_YENEURO_PRICE:
                snprintf(yeneuro_buffer, sizeof(yeneuro_buffer), "%s", t->value->cstring);
                break;
            case KEY_YENEURO_DIFF:
                snprintf(yeneuro_diff_buffer, sizeof(yeneuro_diff_buffer), "%s", t->value->cstring);
                break;
            case KEY_YENEURO_NP:
                snprintf(yeneuro_np, sizeof(yeneuro_np), "%s", t->value->cstring);
                break;
            default:
                APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
                break;
        }

        // Look for next item
        t = dict_read_next(iterator);
    }
    snprintf(n225_price_and_diff_buffer, sizeof(n225_price_and_diff_buffer),"%s(%s)", n225_buffer, n225_diff_buffer);
    snprintf(yendollar_price_and_diff_buffer, sizeof(yendollar_price_and_diff_buffer),"%s(%s)", yendollar_buffer, yendollar_diff_buffer);
    snprintf(yeneuro_price_and_diff_buffer, sizeof(yeneuro_price_and_diff_buffer),"%s(%s)", yeneuro_buffer, yeneuro_diff_buffer);
    #ifdef PBL_COLOR
        if(strcmp( n225_np, "0") == 0){
            text_layer_set_background_color(n225_title_layer, GColorTiffanyBlue);
            text_layer_set_background_color(n225_layer, GColorTiffanyBlue);
        }else{
            text_layer_set_background_color(n225_title_layer, GColorRed);
            text_layer_set_background_color(n225_layer, GColorRed);
        }
        if(strcmp( yendollar_np, "0") == 0){
            text_layer_set_background_color(yendollar_title_layer, GColorTiffanyBlue);
            text_layer_set_background_color(yendollar_layer, GColorTiffanyBlue);
        }else{
            text_layer_set_background_color(yendollar_title_layer, GColorRed);
            text_layer_set_background_color(yendollar_layer, GColorRed);
        }
        if(strcmp( yeneuro_np, "0") == 0){
            text_layer_set_background_color(yeneuro_title_layer, GColorTiffanyBlue);
            text_layer_set_background_color(yeneuro_layer, GColorTiffanyBlue);
        }else{
            text_layer_set_background_color(yeneuro_title_layer, GColorRed);
            text_layer_set_background_color(yeneuro_layer, GColorRed);
        }
    #else
    text_layer_set_background_color(n225_layer, GColorBlack);
    text_layer_set_background_color(yendollar_layer, GColorBlack);
    text_layer_set_background_color(yeneuro_layer, GColorBlack);
    #endif
    text_layer_set_text(n225_layer, n225_price_and_diff_buffer);
    text_layer_set_text(yendollar_layer, yendollar_price_and_diff_buffer);
    text_layer_set_text(yeneuro_layer, yeneuro_price_and_diff_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
    // Create main Window element and assign to pointer
    s_main_window = window_create();
    window_set_background_color(s_main_window, GColorBlack);

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);
    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    // Register callbacks
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);

    // Open AppMessage
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
