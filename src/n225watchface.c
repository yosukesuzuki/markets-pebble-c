#include <pebble.h>

#define KEY_N225 0
#define KEY_YENDOLLAR 1

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_markets_layer;

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
    s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_text(s_time_layer, "00:00");

    // Improve the layout to be more like a watchface
    text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

    // Create temperature Layer
    s_markets_layer = text_layer_create(GRect(0, 130, 144, 25));
    text_layer_set_background_color(s_markets_layer, GColorClear);
    text_layer_set_text_color(s_markets_layer, GColorWhite);
    text_layer_set_text_alignment(s_markets_layer, GTextAlignmentCenter);
    text_layer_set_text(s_markets_layer, "Loading...");

    // Create second custom font, apply it and add to Window
    text_layer_set_font(s_markets_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_markets_layer));

    // Make sure the time is displayed from the start
    update_time();
}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
    // Destroy weather elements
    text_layer_destroy(s_markets_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();

    // Get weather update every 30 minutes
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
  static char yendollar_buffer[32];
  static char markets_layer_buffer[32];

  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_N225:
      snprintf(n225_buffer, sizeof(n225_buffer), "%s", t->value->cstring);
      break;
    case KEY_YENDOLLAR:
      snprintf(yendollar_buffer, sizeof(yendollar_buffer), "%s", t->value->cstring);
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }

  // Assemble full string and display
  snprintf(markets_layer_buffer, sizeof(markets_layer_buffer), "%s, %s", markets_layer_buffer, yendollar_buffer);
  text_layer_set_text(s_markets_layer, markets_layer_buffer);
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
    #ifdef PBL_COLOR
    window_set_background_color(s_main_window, GColorDukeBlue);
    #else
    window_set_background_color(s_main_window, GColorBlack);
    #endif

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
