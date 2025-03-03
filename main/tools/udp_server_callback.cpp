

static void on_udp_recv(void *handler_arg, esp_event_base_t base, int32_t id,
                    void *event_data)
{
    if (event_data == NULL) {
        return;
    }
    udp_msg_t *recv = (udp_msg_t *)event_data;
    char *txt = (char*)calloc(1,recv->len+1);
    memcpy(txt,recv->payload,recv->len);
    
   // ESP_LOGI("udp_server", "Received %d bytes from %s:%d", recv->len,
   //          udp_server.remote_ipstr(recv->remote), udp_server.remote_port(recv->remote));
   // ESP_LOG_BUFFER_HEXDUMP(TAG, recv->payload, recv->len, ESP_LOG_INFO);
   // ESP_LOGI("udp_server <<", "%s",txt);

    strcpy((char *)GlobalConfig.current_server_ip, udp_server.remote_ipstr(recv->remote));
    GlobalConfig.have_current_server_ip = true;

    rs485_callback(txt,253,TR_SERIAL);
    free(txt);

}