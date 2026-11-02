#pragma once
//screen size
#define HEIGTH 320
#define WIDTH 240
//tile size
//
#define TILE_SIZE 20
//max number of game objects that can fit into a snapshot
#define MAX_OBJECT_COUNT 100
//number of tiles
#define MAX_TILE_COUNT ((HEIGTH/TILE_SIZE) * (WIDTH/TILE_SIZE))
// debounce time for the buttons
// in microseconds
#define DEBOUNCE_TIME_IN_NS 20000


//PINS
#define BUZZER_PIN 4
#define A_BUTTON_PIN 48
#define B_BUTTON_PIN 36
#define ARROW_UP_BUTTON_PIN 50
#define ARROW_DOWN_BUTTON_PIN 51
#define ARROW_RIGHT_BUTTON_PIN 49
#define ARROW_LEFT_BUTTON_PIN 52
#define LCD_D0 35
#define LCD_D1 34
#define LCD_D2 33
#define LCD_D3 32
#define LCD_D4 31
#define LCD_D5 23
#define LCD_D6 22
#define LCD_D7 21
#define LCD_RS 28
#define LCD_WR 37
#define LCD_CS 29
#define LCD_RST 30


//Prios
// activates audio if true
#define AUDIO_ON true 
//period time of the audio task
#define AUDIO_TASK_PERIOD_TIME_IN_MS 5

// settings for dynamic framerate adaption
//activates adaptation if true
#define DYNAMICFRAMERATE true
//defines how many measured values of the frame rate the engine is currently supporting will be considered for the adaptation
#define STATSVALUESCOMPARECOUNT 20
// start frame rate value
#define STARTFPS 30.0f
// period time of the statistics task
#define STATSPERIODINMS 1000
// defines the percentage of the frame time reserved for engine overhead, keeping the game logic and rendering utilazation below 100 percent
#define UTALIZATION_BUFFER 0.2f
//defines at which difference between new possible frame rate and current frame rate the adaptation will start
// prevents flickering
#define FPS_THRESHOLD 1 // ab wie viel fps unterschied angepasst werden soll
// max frame rate adaption step that is possible in a statistic task iteration
#define FPS_MAX_OFFSET_PER_ITERATION 5 
//min frame rate
#define MIN_FPS 20.0f
//max frame rate
#define MAX_FPS 120.0f
// defines the number of deadline misses required to trigger a frame rate decrease
#define DEADLINE_MISS_THRESHOLD 3
// defines the number of transfer misses required to trigger a frame rate decrease
#define TRANSFER_MISS_THRESHOLD 3
// amount by which the frame rate is reduced when the threshold is exceeded
#define FPS_DECREASE_ON_DEADLINE_MISS 2
// amount by which the frame rate is reduced when the threshold is exceeded
#define  FPS_DECREASE_ON_TRANSFER_MISS 2
//number of rendering buffers
#define BUFFER_COUNT 6 
