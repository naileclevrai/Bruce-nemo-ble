#include "record_audio_sd.h"

bool recordAudioToSD() {
    if (!setupSdCard()) {
        Serial.println("SD card not found.");
        return false;
    }

    File audioFile = SD.open("/audio.raw", FILE_WRITE);
    if (!audioFile) {
        Serial.println("Failed to open file for writing.");
        return false;
    }

    // Configuration I2S
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 16000, // Réduction de la fréquence pour stabilité
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 3,
        .dma_buf_len = 512
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_PIN_NO_CHANGE,
        .ws_io_num = I2S_PIN_NO_CHANGE,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = PIN_DATA
    };

    if (i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL) != ESP_OK) {
        Serial.println("Failed to install I2S driver.");
        audioFile.close();
        return false;
    }

    if (i2s_set_pin(I2S_NUM_0, &pin_config) != ESP_OK) {
        Serial.println("Failed to set I2S pin configuration.");
        i2s_driver_uninstall(I2S_NUM_0);
        audioFile.close();
        return false;
    }

    uint8_t buffer[1024];
    size_t bytes_written;
    unsigned long start_time = millis();

    Serial.println("Recording started.");
    while (millis() - start_time < RECORDING_DURATION_MS) {
        int bytes_read = i2s_read(I2S_NUM_0, buffer, sizeof(buffer), &bytes_written, portMAX_DELAY);
        if (bytes_read > 0) {
            audioFile.write(buffer, bytes_read);
        }
    }

    Serial.println("Recording finished.");

    audioFile.close();
    i2s_driver_uninstall(I2S_NUM_0);
    closeSdCard();

    return true;
}
