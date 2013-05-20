#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#include "resource_ids.auto.h"

//PBL_APP_INFO_SIMPLE("YWatch", "Yoctopuce", 1 /* App version */);
PBL_APP_INFO("YWatch", "Yoctopuce   ", 2, RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE);

Window window;              // All apps must have at least one window
Layer boardLayer;           // The logo layer
TextLayer my_text_layer;
BmpContainer image_container;

#define TIME_STR_BUFFER_BYTES 32
char s_time_str_buffer[TIME_STR_BUFFER_BYTES];


void handle_tick(AppContextRef ctx, PebbleTickEvent *event) {
    (void)ctx;
    (void)event;
    string_format_time(s_time_str_buffer, TIME_STR_BUFFER_BYTES, "%I:%M:%S %p", event->tick_time);
    text_layer_set_text(&my_text_layer,s_time_str_buffer);
}


// Handle the start-up of the app
void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "yWinName");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  // Display the black and white image with transparency.
  resource_init_current_app(&APP_RESOURCES);

    // Note: This needs to be "de-inited" in the application's
  //       deinit handler.
  bmp_init_container(RESOURCE_ID_IMAGE_LOGO_BRIGHT, &image_container);

  layer_add_child(&window.layer, &image_container.layer.layer);




  // Init the text layer used to show the time
  // TODO: Wrap this boilerplate in a function?
  text_layer_init(&my_text_layer, GRect(0, 144, 144 /* width */, 168-144 /* height */));
  text_layer_set_text_color(&my_text_layer, GColorWhite);
  text_layer_set_background_color(&my_text_layer, GColorClear);
  //text_layer_set_font(&my_text_layer , fonts_get_system_font(RESOURCE_ID_GOTHIC_18));

  text_layer_set_text(&my_text_layer,"yhello");

  layer_add_child(&window.layer, &my_text_layer.layer);

  vibes_double_pulse();
}



void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  // Note: Failure to de-init this here will result in instability and
  //       unable to allocate memory errors.
  bmp_deinit_container(&image_container);
}



void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = SECOND_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
