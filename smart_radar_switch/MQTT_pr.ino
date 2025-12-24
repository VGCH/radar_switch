// Функция отправки данных по протоколу MQTT
void MQTT_send(){
   client.loop();
   uint32_t nows = millis();
  if (nows - lastMsg > 10000 && settings.mqtt_en) {
    if(WiFi.status() == WL_CONNECTED) {
        MQTT_send_data("jsondata", JSON_DATA());
     }
   lastMsg = nows; 
  }
 }
void MQTT_send_data(String topic, String data){
         String root = settings.mqtt_topic;
         String top  = root +"/"+ topic;
         String subscr = root +"/control";
         String input = settings.mqtt_serv;
         int colonIndex = input.indexOf(':');
         String ipAddress;
         String port;
         IPAddress Remote_MQTT_IP;

        if (colonIndex != -1) {
             ipAddress = input.substring(0, colonIndex);
             port = input.substring(colonIndex + 1);
             WiFi.hostByName(ipAddress.c_str(), Remote_MQTT_IP);
          }
      
      client.setServer(Remote_MQTT_IP, port.toInt());
      client.setCallback(callback);
      client.setBufferSize(1024); 
           if(client.connected()){                                                                                    // Вторичная отправка данных при подключенном брокере 
          count_rf = 0;
          send_mqtt(top, data, subscr);
           }else{
              count_rf++;
              if (client.connect(ch_id.c_str(), settings.mqtt_user, settings.mqtt_passw)){                            // Первичная отправка данных, выполняестя попытка подключения к брокеру 
                    send_mqtt(top, data, subscr);          
                }else{
                  if(count_rf > 2){                                                                                    // Если были неудачные попытки подключения, то переподключаем Wi-fi
                     WiFi.disconnect();
                     WiFi.begin(settings.mySSID, settings.myPW);
                     count_rf = 0;
                }
            }
        }
     
}


void send_mqtt(String tops, String data, String subscr) {
    if(!annonce_mqtt_discovery) {
        String device_id = "radar_light_switch_" + ch_id;
        String configuration_url = "http://" + WiFi.localIP().toString();
        String top = String(settings.mqtt_topic) + "/jsondata";
        String control = String(settings.mqtt_topic) + "/control";

        // Создаем конфигурацию устройства
        DynamicJsonDocument deviceDoc(512);
        deviceDoc["ids"][0] = device_id;
        deviceDoc["name"] = "Радарный модуль УО";
        deviceDoc["mdl"] = version_code;
        deviceDoc["sw"] = "1.0.2";
        deviceDoc["mf"] = "CYBEREX TECH";
        deviceDoc["configuration_url"] = configuration_url;

        // Вспомогательная функция для публикации конфигурации
        auto publishConfig = [&](const String& type, const String& entity_id, JsonDocument& doc) {
            doc["device"] = deviceDoc.as<JsonObject>();
            String payload;
            serializeJson(doc, payload);
            client.publish(("homeassistant/" + type + "/" + entity_id + "/config").c_str(), 
                          payload.c_str(), true);
        };

        //  Датчик расстояния
        {
            DynamicJsonDocument doc(384);
            doc["device_class"] = "distance";
            doc["name"] = "Датчик присутствия";
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "cm";
            doc["value_template"] = "{{ value_json.distance }}";
            doc["unique_id"] = ch_id + "_d";
            publishConfig("sensor", ch_id + "_d", doc);
        }

        //  Верхний порог (сенсор)
        {
            DynamicJsonDocument doc(384);
            doc["device_class"] = "distance";
            doc["name"] = "Порог срабатывания верхний";
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "cm";
            doc["value_template"] = "{{ value_json.h_on }}";
            doc["unique_id"] = ch_id + "_d_on";
            publishConfig("sensor", ch_id + "_d_on", doc);
        }

        //  Нижний порог (сенсор)
        {
            DynamicJsonDocument doc(384);
            doc["device_class"] = "distance";
            doc["name"] = "Порог срабатывания нижний";
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "cm";
            doc["value_template"] = "{{ value_json.h_off }}";
            doc["unique_id"] = ch_id + "_d_off";
            publishConfig("sensor", ch_id + "_d_off", doc);
        }

        //  Задержка выключения (сенсор)
        {
            DynamicJsonDocument doc(512);
            doc["device_class"] = "duration";
            doc["name"] = "Задержка отключения";
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "s";
            doc["value_template"] = "{{ value_json.d_time }}";
            doc["unique_id"] = ch_id + "_d_off_time";
            publishConfig("sensor", ch_id + "_d_off_time", doc);
        }

        //  Минимальный порог сработки
        {
            DynamicJsonDocument doc(512);
            doc["name"] = "Мин порог";
            doc["command_topic"] = control;
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "см";
            doc["value_template"] = "{{ value_json.h_off }}";
            doc["command_template"] = "{\"min_threshold\": {{ value }}}";
            doc["min"] = 0;
            doc["max"] = 300;
            doc["step"] = 1;
            doc["mode"] = "slider";
            doc["unique_id"] = ch_id + "_min";
            publishConfig("number", ch_id + "_min", doc);
        }

        //  Максимальный порог сработки
        {
            DynamicJsonDocument doc(512);
            doc["name"] = "Макс порог";
            doc["command_topic"] = control;
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "см";
            doc["value_template"] = "{{ value_json.h_on }}";
            doc["command_template"] = "{\"max_threshold\": {{ value }}}";
            doc["min"] = 0;
            doc["max"] = 300;
            doc["step"] = 1;
            doc["mode"] = "slider";
            doc["unique_id"] = ch_id + "_max";
            publishConfig("number", ch_id + "_max", doc);
        }

        //  Время задержки отключения
        {
            DynamicJsonDocument doc(512);
            doc["name"] = "Задержка отключения";
            doc["command_topic"] = control;
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "сек";
            doc["value_template"] = "{{ value_json.d_time }}";
            doc["command_template"] = "{\"delaytime_threshold\": {{ value }}}";
            doc["min"] = 0;
            doc["max"] = 500;
            doc["step"] = 1;
            doc["mode"] = "slider";
            doc["unique_id"] = ch_id + "t_delay";
            publishConfig("number", ch_id + "t_delay", doc);
        }
         //  Фильтр коэффициент R
         {
            DynamicJsonDocument doc(512);
            doc["name"] = "Фильтр R";
            doc["command_topic"] = control;
            doc["state_topic"] = top;
            //doc["unit_of_measurement"] = "";
            doc["value_template"] = "{{ value_json.filter_r }}";
            doc["command_template"] = "{\"filter_r_threshold\": {{ value }}}";
            doc["min"] = 0;
            doc["max"] = 6;
            doc["step"] = 0.1;
            doc["mode"] = "slider";
            doc["unique_id"] = ch_id + "r_filter";
            publishConfig("number", ch_id + "r_filter", doc);
        }

        //  Фильтр коэффициент Q
        {
            DynamicJsonDocument doc(512);
            doc["name"] = "Фильтр Q";
            doc["command_topic"] = control;
            doc["state_topic"] = top;
            //doc["unit_of_measurement"] = "";
            doc["value_template"] = "{{ value_json.filter_q }}";
            doc["command_template"] = "{\"filter_q_threshold\": {{ value }}}";
            doc["min"] = 0.001;
            doc["max"] = 0.1;
            doc["step"] = 0.001;
            doc["mode"] = "slider";
            doc["unique_id"] = ch_id + "q_filter";
            publishConfig("number", ch_id + "q_filter", doc);
        }

        //  Переключатель освещения
        {
            DynamicJsonDocument doc(384);
            doc["name"] = "Управление освещением";
            doc["command_topic"] = control;
            doc["state_topic"] = top;
            doc["payload_on"] = "0";
            doc["payload_off"] = "0";
            doc["state_on"] = "On";
            doc["state_off"] = "Off";
            doc["value_template"] = "{{ value_json.c }}";
            doc["unique_id"] = ch_id + "_s_off";
            publishConfig("switch", ch_id + "_s_off", doc);
        }

        //  Переключатель авторежима
        {
            DynamicJsonDocument doc(384);
            doc["name"] = "Автоматический режим";
            doc["command_topic"] = control;
            doc["state_topic"] = top;
            doc["payload_on"] = "1";
            doc["payload_off"] = "2";
            doc["state_on"] = "1";
            doc["state_off"] = "0";
            doc["value_template"] = "{{ value_json.a }}";
            doc["unique_id"] = ch_id + "aut";
            publishConfig("switch", ch_id + "aut", doc);
        }

        annonce_mqtt_discovery = true;
    }
    
    // Отправляем данные 
    client.publish(tops.c_str(), data.c_str());
    client.subscribe(subscr.c_str());
}

String MQTT_status(){
    return client.connected() ? "подключен" : "отключен";
} 
