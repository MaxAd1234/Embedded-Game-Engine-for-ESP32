
#include "button_types.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "input_events.hpp"
#include "InputService.hpp"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_ili9341.h"
#include "esp_lcd_io_i80.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "EngineConfig.h"
#include "World.hpp"
#include "Texture.hpp"
#include <cstring>
#include <cstdio>
#include <stdio.h>
#include "esp_log.h"
#include "assets.hpp"
#include <algorithm>
#include <limits>
#include "driver/ledc.h"
#include "Sound.hpp"
#include "AudioService.hpp"

// saves start and end time of each iteration of the render task to adjust the framerate
//  saves time in cpu cycles
//  5000 pairs
uint32_t renderStats[10000];
int statsCount = 10000;
int renderStatCurrentId = 0;

// saves start and end time of each iteration of the logic task to adjust the framerate
//  saves time in cpu cycles
uint32_t logicStats[10000];
int logicStatCurrentId = 0;

// Queues for the Statistics-Task
QueueHandle_t deadlineMissedSignal;
QueueHandle_t transferNotDoneBeforeNextRenderingSignal;
QueueHandle_t transferTokens;

void increamentCounter(int *counter)
{
    *counter += 1;
    if (*counter >= statsCount)
        *counter = 0;
}

// Engine frame rate
float FPS = STARTFPS;
TickType_t xEngineFrequency;

// Statistics Task
// for detailed implementation documentation, read my thesis
void statisticsTask(void *param)
{

    const TickType_t xFrequencyStatistics = pdMS_TO_TICKS(STATSPERIODINMS);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    vTaskDelayUntil(&xLastWakeTime, xFrequencyStatistics);

    for (;;)
    {
        bool deadlineMissed = false;
        bool transferNotFinished = false;

        int deadlineMissCnt = uxQueueMessagesWaiting(deadlineMissedSignal);

        if (deadlineMissCnt >= DEADLINE_MISS_THRESHOLD)
        {
            deadlineMissed = true;
            ESP_LOGI("STATS", "Deadlines were missed");
        }
        else
        {
            deadlineMissed = false;
            ESP_LOGI("STATS", "Deadlines were not missed");
        }
        // misses löschen
        for (int i = 0; i < deadlineMissCnt; i++)
        {
            bool tmp = false;
            xQueueReceive(deadlineMissedSignal, &tmp, 0);
        }

        if (xQueueReceive(transferNotDoneBeforeNextRenderingSignal, &transferNotFinished, 0) == pdTRUE)
        {
            transferNotFinished = true;
            ESP_LOGI("STATS", "Transfer did not finish before next rendering run");
        }
        else
        {
            transferNotFinished = false;
            ESP_LOGI("STATS", "Transfer finished before next rendering run");
        }

        // get possible rendering framerate
        int lastRenderID = renderStatCurrentId;
        int lastLogicID = logicStatCurrentId;
       
        int newestPairIndex = (lastRenderID % 2 == 0) ? lastRenderID : lastRenderID - 1;

        int startPairIndex = (newestPairIndex < STATSVALUESCOMPARECOUNT * 2) ? 0 : newestPairIndex - (STATSVALUESCOMPARECOUNT * 2);
        int pairCount = (newestPairIndex < STATSVALUESCOMPARECOUNT * 2) ? newestPairIndex / 2 : STATSVALUESCOMPARECOUNT;
        ESP_LOGI("STATS", "Pair count: %d", pairCount);

        float averageRenderWorktimeInMS = 0;

        for (int i = startPairIndex; i < newestPairIndex; i += 2)
        {
            uint32_t startCycleCount = renderStats[i];
            uint32_t endCycleCount = renderStats[i + 1];

            // falls overflow von cyclecount
            if (endCycleCount < startCycleCount)
            {
                uint32_t system_max = std::numeric_limits<uint32_t>::max();
                endCycleCount += system_max - startCycleCount;
                startCycleCount = 0;
            }

            uint32_t timeDifferenceInCycles = endCycleCount - startCycleCount;

            float timeDifferenceInMicroSecs = (float)timeDifferenceInCycles / esp_rom_get_cpu_ticks_per_us();
            float timeDifferenceInMillies = timeDifferenceInMicroSecs / 1000.0f;

            averageRenderWorktimeInMS += timeDifferenceInMillies;
        }

        if (pairCount != 0)
            averageRenderWorktimeInMS = averageRenderWorktimeInMS / static_cast<float>(pairCount);

        newestPairIndex = (lastLogicID % 2 == 0) ? lastLogicID : lastLogicID - 1;

        startPairIndex = (newestPairIndex < STATSVALUESCOMPARECOUNT * 2) ? 0 : newestPairIndex - (STATSVALUESCOMPARECOUNT * 2);
        pairCount = (newestPairIndex < STATSVALUESCOMPARECOUNT * 2) ? newestPairIndex / 2 : STATSVALUESCOMPARECOUNT;
        ESP_LOGI("STATS", "Pair count: %d", pairCount);

        float averageLogicWorktimeInMS = 0;

        for (int i = startPairIndex; i < newestPairIndex; i += 2)
        {
            uint32_t startCycleCount = logicStats[i];
            uint32_t endCycleCount = logicStats[i + 1];

            if (endCycleCount < startCycleCount)
            {
                uint32_t system_max = std::numeric_limits<uint32_t>::max();
                endCycleCount += system_max - startCycleCount;
                startCycleCount = 0;
            }

            uint32_t timeDifferenceInCycles = endCycleCount - startCycleCount;

            float timeDifferenceInMicroSecs = (float)timeDifferenceInCycles / esp_rom_get_cpu_ticks_per_us();
            float timeDifferenceInMillies = timeDifferenceInMicroSecs / 1000.0f;

            averageLogicWorktimeInMS += timeDifferenceInMillies;
        }

        if (pairCount != 0)
            averageLogicWorktimeInMS = averageLogicWorktimeInMS / static_cast<float>(pairCount);

        if (deadlineMissed)
        {
            FPS = FPS - FPS_DECREASE_ON_DEADLINE_MISS;
            if (FPS < MIN_FPS)
                FPS = MIN_FPS;
            if (FPS > MAX_FPS)
                FPS = MAX_FPS;
            xEngineFrequency = pdMS_TO_TICKS(1000 / FPS);
            ESP_LOGI("FPS", "FPS angepasst an: %f", FPS);
            vTaskDelayUntil(&xLastWakeTime, xFrequencyStatistics);
            continue;
        }
        if (transferNotFinished)
        {
            FPS = FPS - FPS_DECREASE_ON_TRANSFER_MISS;
            if (FPS < MIN_FPS)
                FPS = MIN_FPS;
            if (FPS > MAX_FPS)
                FPS = MAX_FPS;
            xEngineFrequency = pdMS_TO_TICKS(1000 / FPS);
            ESP_LOGI("FPS", "FPS angepasst an: %f", FPS);
            vTaskDelayUntil(&xLastWakeTime, xFrequencyStatistics);
            continue;
        }

        if (averageLogicWorktimeInMS != 0.0f && averageRenderWorktimeInMS != 0.0f)
        {
            // util = worktime/period
            //  FPS = 1000[ms]/period
            // period = worktime/util
            //  FPS = 1000[ms]/(worktime/util)

            float newFPS = (int)(1000 / ((averageLogicWorktimeInMS + averageRenderWorktimeInMS) / (1.0f - UTALIZATION_BUFFER)));

            if (newFPS < MIN_FPS)
                newFPS = MIN_FPS;
            if (newFPS > MAX_FPS)
                newFPS = MAX_FPS;

            if (newFPS >= FPS + FPS_THRESHOLD)
            {
                FPS = (newFPS > (FPS + FPS_MAX_OFFSET_PER_ITERATION)) ? FPS + FPS_MAX_OFFSET_PER_ITERATION : newFPS;
                xEngineFrequency = pdMS_TO_TICKS(1000 / FPS);
                ESP_LOGI("FPS", "FPS angepasst an: %f", newFPS);
            }
        }

        ESP_LOGI("FPS", "Current FPS: %f", FPS);
        vTaskDelayUntil(&xLastWakeTime, xFrequencyStatistics);
    }
}

// index position of a redrawn tile
struct RedrawedTile
{
    int row;
    int col;
};

InputService service;
uint64_t debounceTime = DEBOUNCE_TIME_IN_NS;

// button pins sorted after the enum order
int buttonPins[6] = {A_BUTTON_PIN, B_BUTTON_PIN, ARROW_UP_BUTTON_PIN, ARROW_DOWN_BUTTON_PIN, ARROW_RIGHT_BUTTON_PIN, ARROW_LEFT_BUTTON_PIN};
BUTTON_TYPE types[6] = {A_BUTTON,
                        B_BUTTON,
                        ARROW_UP_BUTTON,
                        ARROW_DOWN_BUTTON,
                        ARROW_RIGHT_BUTTON,
                        ARROW_LEFT_BUTTON};
int buttonCount = 6;

gpio_config_t buttonConfigs[6];
// queues for communication
QueueHandle_t inputQueue;
QueueHandle_t snapShotQueue;
QueueHandle_t freeBufferQueue;
QueueHandle_t bufferInUseQueue;
QueueHandle_t redrawedTilesQueue;
QueueHandle_t soundEffectQueue, backgroundMusicQueue;

// rendering buffer
uint16_t buff1[TILE_SIZE * TILE_SIZE];
uint16_t buff2[TILE_SIZE * TILE_SIZE];
uint16_t buff3[TILE_SIZE * TILE_SIZE];
uint16_t buff4[TILE_SIZE * TILE_SIZE];
uint16_t buff5[TILE_SIZE * TILE_SIZE];
uint16_t buff6[TILE_SIZE * TILE_SIZE];

AudioService audioService;

World gameWorld(WIDTH, HEIGTH, TILE_SIZE, service, audioService);
WorldSnapshot snap, snap2;

esp_lcd_panel_handle_t panel_handle = NULL;

void setButtonCongigs()
{
    for (size_t i = 0; i < buttonCount; i++)
    {
        buttonConfigs[i] = {
            .pin_bit_mask = 1ULL << buttonPins[i],
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_ANYEDGE,
            .hys_ctrl_mode = GPIO_HYS_SOFT_DISABLE};
    }
}

void configureGPIOs()
{
    for (size_t i = 0; i < buttonCount; i++)
    {
        gpio_config(&buttonConfigs[i]);
        gpio_intr_enable(static_cast<gpio_num_t>(buttonPins[i]));
    }
}

// for debouncing and valid input state transitions
uint64_t lastInterruptTimes[6] = {0, 0, 0, 0, 0, 0};
int lastStates[6] = {1, 1, 1, 1, 1, 1};
// handling inputs
void InputHandler(void *buttonType)
{
    BUTTON_TYPE *type = static_cast<BUTTON_TYPE *>(buttonType);
    int currentLevel = gpio_get_level(static_cast<gpio_num_t>(buttonPins[*type]));
    uint64_t time = esp_timer_get_time();
    INPUT_TYPE input = (currentLevel == 0) ? PRESSED : RELEASED;

    // debouncing
    if (time - lastInterruptTimes[*type] < debounceTime)
    {
        return;
    }

    if (currentLevel == lastStates[*type])
    {
        return;
    }
    lastInterruptTimes[*type] = time;
    lastStates[*type] = currentLevel;

    InputEvent event = {
        .button = *type,
        .inputType = input,
        .timeStamp = time};
    // send the input event to the Logic-Task
    xQueueSendToBackFromISR(inputQueue, &event, NULL);
}

void renderTask(void *param)
{

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        // For statistics task
        //
        if (uxQueueMessagesWaiting(transferTokens) >= TRANSFER_MISS_THRESHOLD)
        {
            bool transferNotDoneBeforeNextRun = true;
            xQueueOverwrite(transferNotDoneBeforeNextRenderingSignal, &transferNotDoneBeforeNextRun);
        }

        // skip rendering if no new snapshot is available
        if (xQueueReceive(snapShotQueue, &snap, 0) != pdPASS)
        {
            vTaskDelayUntil(&xLastWakeTime, xEngineFrequency);
            continue;
        }

        renderStats[renderStatCurrentId] = esp_cpu_get_cycle_count();
        increamentCounter(&renderStatCurrentId);

        // render the snapshot
        for (int tileIndex = 0; tileIndex < snap.tileCount; tileIndex++)
        {
            Tile t = snap.allTiles[tileIndex];
            // render only if needed
            if (t.getNeedsRedraw() != true)
            {
                continue;
            }
            // check if tile has texture
            Texture *tileTex = t.getTexture();
            if (tileTex == nullptr)
            {
                continue;
            }
            if ((*tileTex).getSprite() == nullptr)
            {
                continue;
            }
            // get a free buffer
            uint16_t *buff;
            // skip if no buffer is avialbale
            if (xQueueReceive(freeBufferQueue, &buff, 0) != pdPASS)
            {
                continue;
            }
            // copy tile sprite into buffer
            memcpy(buff, (*tileTex).getSprite(), (*tileTex).getWidth() * (*tileTex).getHeigth() * sizeof(uint16_t));
            // copy a objects texture into the buffer if it is in the tile
            for (int objectIndex = 0; objectIndex < snap.objectCount; objectIndex++)
            {
                GameObject *o = snap.allObjects[objectIndex];

                if (o == nullptr)
                    continue;

                // skip object if it has no tile or is inactive
                if (o->getTexture().getSprite() == nullptr || !o->getActive())
                {
                    continue;
                }

                if (t.isInTile(o))
                {
                    // ESP_LOGI("RENDER", "Object ist in der Tile");

                    int objectX = o->getPosition().getX();
                    int objectY = o->getPosition().getY();
                    Texture objectTex = o->getTexture();

                    int tileX = t.getAbsoluteXPos() * t.getTileSize();
                    int tileY = t.getAbsoluteYPos() * t.getTileSize();
                    int tSize = t.getTileSize();

                    int xStartGlobal = std::max(t.getAbsoluteXPos() * t.getTileSize(), objectX);
                    int xEndGlobal = std::min(t.getAbsoluteXPos() * t.getTileSize() + t.getTileSize(), objectX + o->getTexture().getWidth());
                    int yStartGlobal = std::max(t.getAbsoluteYPos() * t.getTileSize(), objectY);
                    int yEndGlobal = std::min(t.getAbsoluteYPos() * t.getTileSize() + t.getTileSize(), objectY + o->getTexture().getHeigth());

                    if (o->getTexture().getBitMask() == nullptr)
                    {
                        for (int row = yStartGlobal; row < yEndGlobal; row++)
                        {
                            const uint16_t *startObject = objectTex.getSprite() + (row - objectY) * objectTex.getWidth() + (xStartGlobal - objectX);
                            int bufferOffset = (row - tileY) * tSize + (xStartGlobal - tileX);
                            int dataLoad = (xEndGlobal - xStartGlobal) * sizeof(uint16_t);

                            memcpy(buff + bufferOffset, startObject, dataLoad);
                        }
                    }
                    else
                    {
                        for (int row = yStartGlobal; row < yEndGlobal; row++)
                        {
                            for (int col = xStartGlobal; col < xEndGlobal; col++)
                            {
                                uint16_t bitMask = (objectTex.getBitMask() == nullptr || objectTex.getBitMaskValue(row - objectY, col - objectX) == 0xFFFF) ? 0xFFFF : 0x0000;
                                buff[(row - tileY) * tSize + (col - tileX)] = (buff[(row - tileY) * tSize + (col - tileX)] & (0xFFFF - bitMask)) | (objectTex.getPixel(row - objectY, col - objectX) & bitMask);
                            }
                        }
                    }
                }
            }

            bool tok = true;
            xQueueSendToBack(transferTokens, &tok, 0);
            xQueueSendToBack(bufferInUseQueue, &buff, 0);
            // drawBitMap
            esp_lcd_panel_draw_bitmap(panel_handle, t.getAbsoluteXPos() * t.getTileSize(), t.getAbsoluteYPos() * t.getTileSize(), t.getAbsoluteXPos() * t.getTileSize() + t.getTileSize(), t.getAbsoluteYPos() * t.getTileSize() + t.getTileSize(), buff);

            RedrawedTile redrawed;
            redrawed.row = t.getAbsoluteYPos();
            redrawed.col = t.getAbsoluteXPos();
            // tile was redrawn
            xQueueSendToBack(redrawedTilesQueue, &redrawed, 0);
        }

        renderStats[renderStatCurrentId] = esp_cpu_get_cycle_count();
        increamentCounter(&renderStatCurrentId);

        if (xTaskDelayUntil(&xLastWakeTime, xEngineFrequency) == pdFALSE)
        {
            bool missed = true;
            xQueueSendToBack(deadlineMissedSignal, &missed, 0);
        }
    }
}

void initTask(void *param)
{
    assetInit();
    gameWorld.init();
    snap2 = gameWorld.createSnapShot();
    // neuen snapshot an render task schicken
    xQueueOverwrite(snapShotQueue, &snap2);
    vTaskDelete(NULL);
}

void logicTask(void *param)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint64_t lastTime = esp_timer_get_time();
    uint64_t timeNow;

    for (;;)
    {

        logicStats[logicStatCurrentId] = esp_cpu_get_cycle_count();
        increamentCounter(&logicStatCurrentId);

        timeNow = esp_timer_get_time();
        uint64_t deltaTime = (timeNow - lastTime) / 1000.0f;
        // ESP_LOGI("BEWEGUNG","Deltatime %llu us", deltaTime);
        lastTime = timeNow;

        // schon gezeichnete Tiles resetten
        size_t tileCount = uxQueueMessagesWaiting(redrawedTilesQueue);
        while (tileCount > 0)
        {
            RedrawedTile redraw;
            xQueueReceive(redrawedTilesQueue, &redraw, 0);
            gameWorld.getTileMap().tileWasRedrawed(redraw.row, redraw.col);
            tileCount--;
        }

        // input verarbeiten
        size_t messageCount = uxQueueMessagesWaiting(inputQueue);
        while (messageCount > 0)
        {
            InputEvent ev = {};
            xQueueReceive(inputQueue, &ev, 0);
            service.handleInputEvent(ev);
            messageCount--;
        }

        // logic updaten
        gameWorld.update(deltaTime);

        service.resetInputAfterFrame();

        snap2 = gameWorld.createSnapShot();
        // neuen snapshot an render task schicken

        xQueueOverwrite(snapShotQueue, &snap2);

        if (audioService.newSoundSet())
        {
            Sound *effect = audioService.getSoundeffect();
            xQueueOverwrite(soundEffectQueue, &effect);
        }

        if (audioService.newBackgroundMusicSet())
        {
            Sound *effect = audioService.getBackgroundMusic();
            xQueueOverwrite(backgroundMusicQueue, &effect);
            ESP_LOGI("AUDIO", "Background losgeschickt");
        }

        audioService.reset();

        logicStats[logicStatCurrentId] = esp_cpu_get_cycle_count();
        increamentCounter(&logicStatCurrentId);

        if (xTaskDelayUntil(&xLastWakeTime, xEngineFrequency) == pdFALSE)
        {
            bool missed = true;
            xQueueSendToBack(deadlineMissedSignal, &missed, 0);
        }
    }
}

// mark a buffer as free after drawing
bool onTransfer(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    bool tmpForTok = false;
    xQueueReceiveFromISR(transferTokens, &tmpForTok, 0);

    uint16_t *buf;

    if (xQueueReceiveFromISR(bufferInUseQueue, &buf, 0) == pdTRUE)
    {
        xQueueSendFromISR(freeBufferQueue, &buf, 0);
    }

    return false;
}

// AUDIO

ledc_timer_config_t timerConfig = {};
ledc_channel_config_t channelConfig = {};

Sound *backgroundMus = nullptr;
Sound *soundeffect = nullptr;

// for audio
// for detailed implementation documentation, read my thesis
void audioTask(void *param)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint64_t lastTime = esp_timer_get_time();
    uint64_t timeNow;
    bool effectChanged, backChanged;
    // timeelapsed auch als float machen
    for (;;)
    {
        timeNow = esp_timer_get_time();
        // in microsecs
        uint64_t timeElapsed = timeNow - lastTime;
        // in Milliesecs
        timeElapsed = timeElapsed / 1000;

        lastTime = timeNow;

        effectChanged = false;
        backChanged = false;

        // checken, ob neue sound reinkommen
        Sound *newBackMus;
        Sound *newEffect;
        if (xQueueReceive(soundEffectQueue, &newEffect, 0) == pdTRUE)
        {
            soundeffect = newEffect;
            effectChanged = true;
        }
        if (xQueueReceive(backgroundMusicQueue, &newBackMus, 0) == pdTRUE)
        {
            backgroundMus = newBackMus;
            backChanged = true;
            ESP_LOGI("AUDIO", "Background angekommen");
        }

        unsigned int effectTone = 0;
        unsigned int backgroundTone = 0;
        if (effectChanged && soundeffect != nullptr)
        {
            effectTone = soundeffect->start();
        }
        else if (soundeffect != nullptr)
        {
            effectTone = soundeffect->update(timeElapsed);

            // prüfen, ob soundeffect vorbei ist
            if (soundeffect->isOver())
            {
                soundeffect = nullptr;
            }
        }

        if (backChanged && backgroundMus != nullptr)
        {
            backgroundTone = backgroundMus->start();
        }
        else if (backgroundMus != nullptr)
        {
            backgroundTone = backgroundMus->update(timeElapsed);
        }

        // wenn soundeffect da, dann wird soundeffect gespielt, sonst background

        unsigned int toneToPlay = 0;

        if (backgroundMus != nullptr)
            toneToPlay = backgroundTone;
        if (soundeffect != nullptr)
            toneToPlay = effectTone;

        if (toneToPlay == 0)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        }
        else
        {
            ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, toneToPlay);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 128);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(AUDIO_TASK_PERIOD_TIME_IN_MS));
    }
}

extern "C" void app_main(void)
{
    // PWM

    timerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConfig.duty_resolution = LEDC_TIMER_8_BIT;
    timerConfig.timer_num = LEDC_TIMER_0;
    timerConfig.freq_hz = 5000;
    timerConfig.clk_cfg = LEDC_AUTO_CLK;

    channelConfig.gpio_num = static_cast<gpio_num_t>(BUZZER_PIN);
    channelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    channelConfig.channel = LEDC_CHANNEL_0;
    channelConfig.timer_sel = LEDC_TIMER_0;

    channelConfig.duty = 0;
    channelConfig.hpoint = 0;
    channelConfig.sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD;
    channelConfig.deconfigure = false;
    channelConfig.flags = {false};

    ledc_timer_config(&timerConfig);
    ledc_channel_config(&channelConfig);

    // INPUT

    xEngineFrequency = pdMS_TO_TICKS(1000 / FPS);
    setButtonCongigs();
    configureGPIOs();

    soundEffectQueue = xQueueCreate(1, sizeof(Sound *));
    backgroundMusicQueue = xQueueCreate(1, sizeof(Sound *));
    inputQueue = xQueueCreate(20, sizeof(InputEvent));
    snapShotQueue = xQueueCreate(1, sizeof(WorldSnapshot));
    freeBufferQueue = xQueueCreate(BUFFER_COUNT, sizeof(uint16_t *));
    bufferInUseQueue = xQueueCreate(BUFFER_COUNT, sizeof(uint16_t *));
    redrawedTilesQueue = xQueueCreate(WIDTH / TILE_SIZE * HEIGTH / TILE_SIZE + 20, sizeof(RedrawedTile));
    deadlineMissedSignal = xQueueCreate(10, sizeof(bool));
    transferNotDoneBeforeNextRenderingSignal = xQueueCreate(1, sizeof(bool));
    transferTokens = xQueueCreate(WIDTH / TILE_SIZE * HEIGTH / TILE_SIZE + 20, sizeof(bool));

    uint16_t *buffer1Ptr = buff1;
    uint16_t *buffer2Ptr = buff2;
    uint16_t *buffer3Ptr = buff3;
    uint16_t *buffer4Ptr = buff4;
    uint16_t *buffer5Ptr = buff5;
    uint16_t *buffer6Ptr = buff6;

    xQueueSend(freeBufferQueue, &buffer1Ptr, 0);
    xQueueSend(freeBufferQueue, &buffer2Ptr, 0);
    xQueueSend(freeBufferQueue, &buffer3Ptr, 0);
    xQueueSend(freeBufferQueue, &buffer4Ptr, 0);
    xQueueSend(freeBufferQueue, &buffer5Ptr, 0);
    xQueueSend(freeBufferQueue, &buffer6Ptr, 0);

    gpio_install_isr_service(ESP_INTR_FLAG_EDGE);

    service.resetInput();

    // add handler
    for (size_t i = 0; i < buttonCount; i++)
    {
        gpio_isr_handler_add(static_cast<gpio_num_t>(buttonPins[i]), &InputHandler, &types[i]);
    }

    // display boilerplate code
    esp_lcd_i80_bus_handle_t i80_bus = NULL;

    esp_lcd_i80_bus_config_t bus_config = {
        .dc_gpio_num = static_cast<gpio_num_t>(LCD_RS),
        .wr_gpio_num = static_cast<gpio_num_t>(LCD_WR),
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .data_gpio_nums = {
            static_cast<gpio_num_t>(LCD_D0),
            static_cast<gpio_num_t>(LCD_D1),
            static_cast<gpio_num_t>(LCD_D2),
            static_cast<gpio_num_t>(LCD_D3),
            static_cast<gpio_num_t>(LCD_D4),
            static_cast<gpio_num_t>(LCD_D5),
            static_cast<gpio_num_t>(LCD_D6),
            static_cast<gpio_num_t>(LCD_D7),
        },
        .bus_width = 8,
        .max_transfer_bytes = 480 * 320 * sizeof(uint16_t),
        .dma_burst_size = 64};

    esp_lcd_new_i80_bus(&bus_config, &i80_bus);

    esp_lcd_panel_io_handle_t io_handle = nullptr;

    esp_lcd_panel_io_i80_config_t io_config = {};

    io_config.cs_gpio_num = static_cast<gpio_num_t>(LCD_CS),
    io_config.pclk_hz = 10000000,
    io_config.trans_queue_depth = 10,
    io_config.dc_levels = {
        .dc_idle_level = 0,
        .dc_cmd_level = 0,
        .dc_dummy_level = 0,
        .dc_data_level = 1,
    },
    io_config.lcd_cmd_bits = 8, io_config.lcd_param_bits = 8,

    esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle);

    esp_lcd_panel_dev_config_t panel_config = {};
    panel_config.reset_gpio_num = static_cast<gpio_num_t>(LCD_RST),
    panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
    panel_config.bits_per_pixel = 16,

    esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle);

    esp_lcd_panel_io_callbacks_t transferCallback = {};

    transferCallback.on_color_trans_done = onTransfer;

    esp_lcd_panel_io_register_event_callbacks(io_handle, &transferCallback, nullptr);

    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    // esp_lcd_panel_mirror(panel_handle, false, true);

    esp_lcd_panel_disp_on_off(panel_handle, true);

    TaskHandle_t logic;
    TaskHandle_t render;
    TaskHandle_t init;
    TaskHandle_t stats;
    TaskHandle_t audio;
    if (xTaskCreate(initTask, "INITTASK",
                    64000,
                    NULL, 20, &init) != pdPASS)
    {
        printf("Init task down");
    }

    if (xTaskCreate(logicTask, "LOGICTASK",
                    64000,
                    NULL, 4, &logic) != pdPASS)
    {
        printf("Logic Task Down");
    }

    if (xTaskCreate(renderTask, "RENDERTASK",
                    64000,
                    NULL, 3, &render) != pdPASS)
    {
        printf("Render task down");
    }
    if (DYNAMICFRAMERATE)
    {
        if (xTaskCreate(statisticsTask, "STATSTASK",
                        64000,
                        NULL, 5, &stats) != pdPASS)
        {
            printf("statistics task down");
        }
    }
    if (xTaskCreate(audioTask, "RENDERTASK",
                    64000,
                    NULL, 6, &audio) != pdPASS)
    {
        printf("Audio task down");
    }
}
