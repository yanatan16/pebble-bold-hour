/*

   Bold Hour watch

   A digital watch with very large hour digits that take up the entire screen
   and smaller minute digits that fit in the null space of the hour digits.

   This watch's load/unload code is mostly taken from the big_time watchface which has to
   load/unload images as necessary. The same is true for bold-hour.

   Author: Jon Eisen

 */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xA3, 0x33, 0x71, 0xE8, 0x33, 0xCB, 0x42, 0xD2, 0x8E, 0x91, 0xC6, 0x6F, 0x26, 0x72, 0xE5, 0xF2 }
PBL_APP_INFO(MY_UUID,
             "Bold Hour", "joneisen.me",
             1, 1, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;

TextLayer minuteLayer;

#define UNINITTED -1
int image_state = UNINITTED;

BmpContainer imageContainer;

#define TEXT_COLOR GColorWhite

#define NUMBER_OF_IMAGES 12

// These are all of our images. Each is the entire screen in size.
const int IMAGE_RESOURCE_IDS[NUMBER_OF_IMAGES] = {
  RESOURCE_ID_IMAGE_NUM_1, RESOURCE_ID_IMAGE_NUM_2,
  RESOURCE_ID_IMAGE_NUM_3, RESOURCE_ID_IMAGE_NUM_4, RESOURCE_ID_IMAGE_NUM_5,
  RESOURCE_ID_IMAGE_NUM_6, RESOURCE_ID_IMAGE_NUM_7, RESOURCE_ID_IMAGE_NUM_8,
  RESOURCE_ID_IMAGE_NUM_9, RESOURCE_ID_IMAGE_NUM_10, RESOURCE_ID_IMAGE_NUM_11,
  RESOURCE_ID_IMAGE_NUM_12
};


void load_digit_image(int digit_value) {
  /*
     Loads the digit image from the application's resources and
     displays it on-screen in the correct location.

     Each slot is the whole screen.

   */

  if ((digit_value < 1) || (digit_value > 12)) {
    return;
  }

  if (image_state == UNINITTED) {
    bmp_init_container(IMAGE_RESOURCE_IDS[digit_value-1], &imageContainer);
    imageContainer.layer.layer.frame.origin.x = 0;
    imageContainer.layer.layer.frame.origin.y = 0;
    layer_add_child(&imageContainer.layer.layer, &minuteLayer.layer);
    layer_add_child(&window.layer, &imageContainer.layer.layer);
    image_state = digit_value;
  }

}

void unload_digit_image() {
  /*

     Removes the digit from the display and unloads the image resource
     to free up RAM.

   */

  if (image_state != UNINITTED) {
    layer_remove_child_layers(&imageContainer.layer.layer);
    layer_remove_from_parent(&imageContainer.layer.layer);
    bmp_deinit_container(&imageContainer);
    image_state = UNINITTED;
  }

}

void reinit_text_layer(unsigned short horiz) {
  static unsigned short current_horiz = 0;

  if (current_horiz != horiz) {
    layer_remove_from_parent(&minuteLayer.layer);
    text_layer_init(&minuteLayer, GRect(horiz, 23, 40 /* width */, 40 /* height */));
    text_layer_set_text_color(&minuteLayer, TEXT_COLOR);
    text_layer_set_background_color(&minuteLayer, GColorClear);
    text_layer_set_font(&minuteLayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MINUTE_38)));
    layer_add_child(&imageContainer.layer.layer, &minuteLayer.layer);
    current_horiz = horiz;
  }
}

void display_time(PblTm * tick_time) {

  // 24 hour clock not supported
  // if (clock_is_24h_style()) {
  //   return hour;
  // }

  unsigned short hour = tick_time->tm_hour % 12;

  // Converts "0" to "12"
  hour = hour ? hour : 12;

  // Only do memory unload/load if necessary
  if (image_state != hour) {
    unload_digit_image();
    load_digit_image(hour);
  }

  // Show minute
  static char text[] = "00";

  string_format_time(text, sizeof(text), "%M", tick_time);

  unsigned short n1s = (text[0]=='1') + (text[1]=='1');

  if (hour == 10 || hour == 12) {
    reinit_text_layer(70 + 3*n1s);
  } else {
    reinit_text_layer(53 + 3*n1s);
  }

  text_layer_set_text(&minuteLayer, text);
}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx;

  display_time(t->tick_time);
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Bold Hour watch");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorWhite);

  resource_init_current_app(&APP_RESOURCES);

  // Avoids a blank screen on watch start.
  PblTm tick_time;

  get_time(&tick_time);
  display_time(&tick_time);
}

void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  unload_digit_image();

}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
