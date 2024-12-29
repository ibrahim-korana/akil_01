#ifndef _WIFI_H_
#define _WIFI_H_

#include "../core.h"
#include "esp_event.h"

typedef struct {
    uint32_t ip;      /**< Interface IPV4 address */
    uint32_t netmask; /**< Interface IPV4 netmask */
    uint32_t gw;      /**< Interface IPV4 gateway address */
} ip_t;

ESP_EVENT_DECLARE_BASE(LWIFI_EVENTS);
enum {
    EVENT_LWIFI_TIME_SYNC,
    EVENT_LWIFI_CONNECTED,
    EVENT_LWIFI_DISCONNECTED,
    EVENT_LWIFI_GOTIP,
};

void initialise_wifi(status_t *cnf);
bool wifi_apsta();
bool wifi_sta();
bool wifi_ap(void);

#endif