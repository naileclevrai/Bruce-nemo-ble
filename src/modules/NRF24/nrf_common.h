#ifndef NRF_COMMON_H
#define NRF_COMMON_H

#include <RF24.h>
#include <SPI.h>
#include <string>
#include <functional>

// Définir les broches pour NRF si elles ne sont pas définies
#ifndef NRF24_CE_PIN
#define NRF24_CE_PIN -1
#endif
#ifndef NRF24_SS_PIN
#define NRF24_SS_PIN -1
#endif

// Déclarations externes
extern RF24 NRFradio;

// Fonctions pour démarrer NRF et afficher les infos
bool nrf_start();
void nrf_info();

// Fonctions pour les scripts Ducky
void mj_process_ducky_script(SPIClass* spi, uint8_t* addr, uint8_t addr_size, uint8_t rate, const std::string& script);

// Fonctions utilitaires
std::string keyboard(const std::string& prompt, int maxLength, const std::string& errorMsg);
std::string chooseFile(FS& fs, const std::string& extension);

#endif // NRF_COMMON_H
