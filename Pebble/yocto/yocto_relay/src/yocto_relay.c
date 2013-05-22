#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x7D, 0x93, 0xA5, 0xD5, 0xEA, 0x73, 0x41, 0x92, 0xB3, 0x89, 0x01, 0xA9, 0x42, 0x42, 0xCA, 0x82 }
PBL_APP_INFO(MY_UUID,
             "Yocto-Relay", "Yoctopuce",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window root_window;
TextLayer text_layer_1;
TextLayer text_layer_2;
TextLayer text_layer_3;

int tmp_status=0;
void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  if(tmp_status&1)
    text_layer_set_text(&text_layer_1,"single click");
  else
    text_layer_set_text(&text_layer_1,"click single");
  tmp_status++;
}

void up_multi_click_handler(ClickRecognizerRef recognizer, Window *window) {
   //const uint16_t count = click_number_of_clicks_counted(recognizer);
   text_layer_set_text(&text_layer_1,"multi-clicks");
 }

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
   text_layer_set_text(&text_layer_2,"single click");
}

void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
  text_layer_set_text(&text_layer_2,"long click start");
}

void select_long_click_release_handler(ClickRecognizerRef recognizer, Window *window) {
  text_layer_set_text(&text_layer_2,"long click is released");
}

void down_multi_click_handler(ClickRecognizerRef recognizer, Window *window) {
  text_layer_set_text(&text_layer_3,"multi click (only last)");
}



void click_config_provider(ClickConfig **config, Window *window) {
  // See ui/click.h for more information and default values.

  // single click / repeat-on-hold config:
  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  config[BUTTON_ID_UP]->click.repeat_interval_ms = 1000; // "hold-to-repeat" gets overridden if there's a long click handler configured!

  // multi click config:
  config[BUTTON_ID_UP]->multi_click.handler = (ClickHandler) up_multi_click_handler;
  config[BUTTON_ID_UP]->multi_click.min = 2;
  config[BUTTON_ID_UP]->multi_click.max = 10;

  // long click config:
  config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;
  config[BUTTON_ID_SELECT]->long_click.release_handler = (ClickHandler) select_long_click_release_handler;
  config[BUTTON_ID_SELECT]->long_click.delay_ms = 700;

  // single click / repeat-on-hold config:
  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;
  config[BUTTON_ID_SELECT]->click.repeat_interval_ms = 1000; // "hold-to-repeat" gets overriden if there's a long click handler configured!

  // multi click config:
  config[BUTTON_ID_DOWN]->multi_click.handler = (ClickHandler) down_multi_click_handler;
  config[BUTTON_ID_DOWN]->multi_click.min = 2;
  config[BUTTON_ID_DOWN]->multi_click.max = 10;
  config[BUTTON_ID_DOWN]->multi_click.last_click_only = true;

 
  (void)window;
}

#define TEXT_LAYER_HEIGHT  (168/3)

void AppInitHandler(AppContextRef ctx) 
{
  (void)ctx;
  window_init(&root_window, "yRootWindow");
  window_stack_push(&root_window, true /* Animated */);
  window_set_background_color(&root_window, GColorBlack);

  // main Text Layer
  text_layer_init(&text_layer_1, GRect(0, 0, 144 , TEXT_LAYER_HEIGHT ));
  text_layer_set_text_color(&text_layer_1, GColorWhite);
  text_layer_set_background_color(&text_layer_1, GColorClear);
  text_layer_set_text(&text_layer_1,"First TextLayer");
  layer_add_child(&root_window.layer, &text_layer_1.layer);

  //second Text layer
  text_layer_init(&text_layer_2, GRect(0, TEXT_LAYER_HEIGHT, 144 , 2*TEXT_LAYER_HEIGHT ));
  text_layer_set_text_color(&text_layer_2, GColorBlack);
  text_layer_set_background_color(&text_layer_2, GColorWhite);
  text_layer_set_text(&text_layer_2,"Second text layer");
  layer_add_child(&root_window.layer, &text_layer_2.layer);

  // main Text Layer
  text_layer_init(&text_layer_3, GRect(0, 2*TEXT_LAYER_HEIGHT ,144, 3*TEXT_LAYER_HEIGHT ));
  text_layer_set_text_color(&text_layer_3, GColorWhite);
  text_layer_set_background_color(&text_layer_3, GColorBlack);
  text_layer_set_text(&text_layer_3,"Third TextLayer");
  layer_add_child(&root_window.layer, &text_layer_3.layer);


  // Attach our desired button functionality
  window_set_click_config_provider(&root_window, (ClickConfigProvider) click_config_provider);

}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &AppInitHandler
  };
  app_event_loop(params, &handlers);
}
