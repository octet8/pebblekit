#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"




/**************************************************************************************************
* GLOBAL DEFINITIONS AND VARIABLES
**************************************************************************************************/
#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 168

enum {
  CMD_KEY = 0x0, // TUPLE_INTEGER
};

enum {
  BUTTON_UP = 0x00,
  BUTTON_SELECT = 0x01,
  BUTTON_DOWN = 0x02,
};

#define TEXT_LAYER_HEIGHT  (SCREEN_HEIGHT/3)


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
MenuLayer menu_layer;


static int tmp_status=0; //fixme remove this debug variable

/**************************************************************************************************
* COMMUNICATION RELATED STUFF
**************************************************************************************************/

static bool callbacks_registered;
static AppMessageCallbacksNode app_callbacks;

static void send_button(uint8_t cmd) {
  Tuplet value = TupletInteger(CMD_KEY, cmd);
  
  DictionaryIterator *iter;
  app_message_out_get(&iter);
  
  if (iter == NULL)
    return;
  
  dict_write_tuplet(iter, &value);
  dict_write_end(iter);
  
  app_message_out_send();
  app_message_out_release();
}


static void dumpError(AppMessageResult reason)
{
  text_layer_set_text(&text_layer_1,"MSG out failed");
  switch(reason){
    case APP_MSG_OK :
      text_layer_set_text(&text_layer_2,"APP_MSG_OK");
      break;
    case APP_MSG_SEND_TIMEOUT :
      text_layer_set_text(&text_layer_2,"APP_MSG_SEND_TIMEOUT");
      break;
    case APP_MSG_SEND_REJECTED :
      text_layer_set_text(&text_layer_2,"APP_MSG_SEND_REJECTED");
      break;
    case APP_MSG_NOT_CONNECTED :
      text_layer_set_text(&text_layer_2,"APP_MSG_NOT_CONNECTED");
      break;
    case APP_MSG_APP_NOT_RUNNING :
      text_layer_set_text(&text_layer_2,"APP_MSG_APP_NOT_RUNNING");
      break;
    case APP_MSG_INVALID_ARGS :
      text_layer_set_text(&text_layer_2,"APP_MSG_INVALID_ARGS");
      break;
    case APP_MSG_BUSY :
      text_layer_set_text(&text_layer_2,"APP_MSG_BUSY");
      break;
    case APP_MSG_BUFFER_OVERFLOW :
      text_layer_set_text(&text_layer_2,"APP_MSG_BUFFER_OVERFLOW");
      break;
    case APP_MSG_ALREADY_RELEASED :
      text_layer_set_text(&text_layer_2,"APP_MSG_ALREADY_RELEASED");
      break;
    case APP_MSG_CALLBACK_ALREADY_REGISTERED :
      text_layer_set_text(&text_layer_2,"APP_MSG_CALLBACK_ALREADY_REGISTERED");
      break;
    case APP_MSG_CALLBACK_NOT_REGISTERED :
      text_layer_set_text(&text_layer_2,"APP_MSG_CALLBACK_NOT_REGISTERED");
      break;
    default :
      text_layer_set_text(&text_layer_2,"WTF!");
      break;
  }

}


static void app_send_done(DictionaryIterator* done, void* context) {
  vibes_short_pulse();
}

static void app_send_failed(DictionaryIterator* failed, AppMessageResult reason, void* context) {
  vibes_double_pulse();
  dumpError(reason);
}

static void app_received_done(DictionaryIterator* received, void* context) {
  vibes_long_pulse();
}

static void app_received_failed(void* context, AppMessageResult reason) {
  vibes_double_pulse();
}







/**************************************************************************************************
* BUTTONS HANDLERS
**************************************************************************************************/

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  if(tmp_status&1)
    text_layer_set_text(&text_layer_1,"single click");
  else
    text_layer_set_text(&text_layer_1,"click single");
  tmp_status++;
  send_button(BUTTON_UP);
}

void up_multi_click_handler(ClickRecognizerRef recognizer, Window *window) {
   //const uint16_t count = click_number_of_clicks_counted(recognizer);
   text_layer_set_text(&text_layer_1,"multi-clicks");
 }

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
   text_layer_set_text(&text_layer_2,"single click");
    send_button(BUTTON_SELECT);
}

void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
  text_layer_set_text(&text_layer_2,"long click start");
}

void select_long_click_release_handler(ClickRecognizerRef recognizer, Window *window) {
  text_layer_set_text(&text_layer_2,"long click is released");
}

void down_multi_click_handler(ClickRecognizerRef recognizer, Window *window) {
  text_layer_set_text(&text_layer_3,"multi click (only last)");
  send_button(BUTTON_DOWN);
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

/**************************************************************************************************
* INTITIALISATIONS STUFF
**************************************************************************************************/



void AppInitHandler(AppContextRef ctx) 
{
  (void)ctx;
  window_init(&root_window, "yRootWindow");
  window_stack_push(&root_window, true /* Animated */);
  window_set_background_color(&root_window, GColorBlack);
#if 0
  // main Text Layer
  text_layer_init(&text_layer_1, GRect(0, 0, SCREEN_WIDTH , TEXT_LAYER_HEIGHT ));
  text_layer_set_text_color(&text_layer_1, GColorWhite);
  text_layer_set_background_color(&text_layer_1, GColorClear);
  text_layer_set_text(&text_layer_1,"First TextLayer");
  layer_add_child(&root_window.layer, &text_layer_1.layer);

  //second Text layer
  text_layer_init(&text_layer_2, GRect(0, TEXT_LAYER_HEIGHT, SCREEN_WIDTH , 2*TEXT_LAYER_HEIGHT ));
  text_layer_set_text_color(&text_layer_2, GColorBlack);
  text_layer_set_background_color(&text_layer_2, GColorWhite);
  text_layer_set_text(&text_layer_2,"Second text layer");
  layer_add_child(&root_window.layer, &text_layer_2.layer);

  // main Text Layer
  text_layer_init(&text_layer_3, GRect(0, 2*TEXT_LAYER_HEIGHT ,SCREEN_WIDTH, 3*TEXT_LAYER_HEIGHT ));
  text_layer_set_text_color(&text_layer_3, GColorWhite);
  text_layer_set_background_color(&text_layer_3, GColorBlack);
  text_layer_set_text(&text_layer_3,"Third TextLayer");
  layer_add_child(&root_window.layer, &text_layer_3.layer);
#endif

  menu_layer_init(&menu_layer, GRect(0, 0 ,SCREEN_WIDTH, SCREEN_HEIGHT ));
  layer_add_child(&root_window.layer, &menu_layer.layer);
  

  // Attach our desired button functionality
  window_set_click_config_provider(&root_window, (ClickConfigProvider) click_config_provider);

}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &AppInitHandler,
    .messaging_info = {
      .buffer_sizes = {
        .inbound = 256,
        .outbound = 32,
      },
      .default_callbacks.callbacks = {
        .out_sent = app_send_done,
        .out_failed = app_send_failed,
        .in_received = app_received_done,
        .in_dropped = app_received_failed,
      },
    }

  };
  app_event_loop(params, &handlers);
}
