/*

   Bold Hour watch

   A digital watch with very large hour digits that take up the entire screen
   and smaller minute digits that fit in the null space of the hour digits.

   This watch's load/unload code is mostly taken from the big_time watchface which has to
   load/unload images as necessary. The same is true for bold-hour.

   24 hour clock not supported

   Author: Jon Eisen <jon@joneisen.me>

 */

#include <pebble.h>

// -- build configuration --
#define LIGHT_WATCHFACE

// -- macros --
#define UNINITTED -1

#ifdef LIGHT_WATCHFACE
  #define TEXT_COLOR GColorWhite
  #define BKGD_COLOR GColorBlack
#else
  #define TEXT_COLOR GColorBlack
  #define BKGD_COLOR GColorWhite
#endif

// -- Global assets --
Window *window;

TextLayer *minuteLayer;
BitmapLayer *hourLayer;

GRect minuteFrame;
GBitmap *hourImage;

int loaded_hour;

// These are all of our images. Each is the entire screen in size.
const int IMAGE_RESOURCE_IDS[12] = {
  RESOURCE_ID_IMAGE_NUM_1, RESOURCE_ID_IMAGE_NUM_2,
  RESOURCE_ID_IMAGE_NUM_3, RESOURCE_ID_IMAGE_NUM_4, RESOURCE_ID_IMAGE_NUM_5,
  RESOURCE_ID_IMAGE_NUM_6, RESOURCE_ID_IMAGE_NUM_7, RESOURCE_ID_IMAGE_NUM_8,
  RESOURCE_ID_IMAGE_NUM_9, RESOURCE_ID_IMAGE_NUM_10, RESOURCE_ID_IMAGE_NUM_11,
  RESOURCE_ID_IMAGE_NUM_12
};


void load_digit_image(int digit_value) {
  if ((digit_value < 1) || (digit_value > 12)) {
    return;
  }

  if (!hourImage) {
    hourImage = gbitmap_create_with_resource(IMAGE_RESOURCE_IDS[digit_value-1]);
    bitmap_layer_set_bitmap(hourLayer, hourImage);
    loaded_hour = digit_value;
  }

}

void unload_digit_image() {

  if (hourImage) {
    gbitmap_destroy(hourImage);
    hourImage = 0;
    loaded_hour = 0;
  }

}

void set_minute_layer_location(unsigned short horiz) {
  if (minuteFrame.origin.x != horiz) {
    minuteFrame.origin.x = horiz;
    layer_set_frame(text_layer_get_layer(minuteLayer), minuteFrame);
    layer_mark_dirty(text_layer_get_layer(minuteLayer));
  }
}

void display_time(struct tm * tick_time) {

  // 24 hour clock not supported

  unsigned short hour = tick_time->tm_hour % 12;

  // Converts "0" to "12"
  hour = hour ? hour : 12;

  // Only do memory unload/load if necessary
  if (loaded_hour != hour) {
    unload_digit_image();
    load_digit_image(hour);
  }

  // Show minute
  static char text[] = "00";

  strftime(text, sizeof(text), "%M", tick_time);

  unsigned short n1s = (text[0]=='1') + (text[1]=='1');

  if (hour == 10 || hour == 12) {
    set_minute_layer_location(70 + 3*n1s);
  } else {
    set_minute_layer_location(53 + 3*n1s);
  }

  text_layer_set_text(minuteLayer, text);
}


void handle_minute_tick(struct tm * tick_time, TimeUnits units_changed) {
  display_time(tick_time);
}

void handle_init() {

  minuteFrame = GRect(53, 23, 40, 40);

  // Dynamic allocation of assets
  window = window_create();
  minuteLayer = text_layer_create(minuteFrame);
  hourLayer = bitmap_layer_create(GRect(0, 0, 144, 168));

  // Configure window
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, BKGD_COLOR);
  window_set_fullscreen(window, true);

  // Setup minute layer
  text_layer_set_text_color(minuteLayer, TEXT_COLOR);
  text_layer_set_background_color(minuteLayer, GColorClear);
  text_layer_set_font(minuteLayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MINUTE_38)));

  // Setup layers
  layer_add_child(bitmap_layer_get_layer(hourLayer), text_layer_get_layer(minuteLayer));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(hourLayer));

  // Avoids a blank screen on watch start.
  time_t tick_time = time(NULL);
  display_time(localtime(&tick_time));
}

void handle_deinit() {
  unload_digit_image();

  text_layer_destroy(minuteLayer);
  bitmap_layer_destroy(hourLayer);
  window_destroy(window);
}

int main(void) {
  handle_init();
  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
  app_event_loop();
  handle_deinit();
}
