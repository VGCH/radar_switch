// Функция отправки данных по протоколу MQTT
void MQTT_send(){
   client.loop();
   uint32_t nows = millis();
  if (nows - lastMsg > 30000 && settings.mqtt_en) {
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
           if(client.connected()){                                                                                    // Вторичная отправка данных при подключенном брокере 
          count_rf = 0;
          send_mqtt(top, data, subscr);
           }else{
              count_rf++;
              if (client.connect(ch_id.c_str(), settings.mqtt_user, settings.mqtt_passw)){           // Первичная отправка данных, выполняестя попытка подключения к брокеру 
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

void send_mqtt(String tops, String data, String subscr){
     // Анонсируем датчики и переключатели для Home Assistant [auto-discovery ]
     // Анонсируем элементы один раз при успешном подуключении и при запуске устройства
    //if(!annonce_mqtt_discovery){
          String top      = String(settings.mqtt_topic) +"/jsondata";
          String control  = String(settings.mqtt_topic) +"/control";
          //Анонс датчика расстояния 
          String ditance = "{\"device_class\": \"distance\", \"name\": \"Dist\", \"state_topic\": \""+top+"\", \"unit_of_measurement\": \"cm\", \"value_template\": \"{{ value_json.hum}}\" }";
          String conf1 = "homeassistant/sensor/"+ch_id+"_d/config";
          client.publish(conf1.c_str(), ditance.c_str(), true);
          //Анонс уставки верхней границы 
          String conf2 = "homeassistant/sensor/"+ch_id+"_d_on/config";
          String ditance_up = "{\"device_class\": \"distance\", \"name\": \"D_up\", \"state_topic\": \""+top+"\", \"unit_of_measurement\": \"cm\", \"value_template\": \"{{ value_json.h_on}}\" }";
          client.publish(conf2.c_str(), ditance_up.c_str(), true);
          //Анонс уставки нижней границы
          String conf3 = "homeassistant/sensor/"+ch_id+"_d_off/config";
          String ditance_down = "{\"device_class\": \"distance\", \"name\": \"D_down\", \"state_topic\": \""+top+"\", \"unit_of_measurement\": \"cm\", \"value_template\": \"{{ value_json.h_off}}\" }";
          client.publish(conf3.c_str(), ditance_down.c_str(), true);
          //Анонс переключателя On/Off  
          String conf4 = "homeassistant/switch/"+ch_id+"_s_off/config";
          String Playload_on_off = "{\"name\": \"OnF\", \"command_topic\": \""+control+"\", \"state_topic\": \""+top+"\", \"payload_on\": \"0\", \"payload_off\": \"0\", \"state_on\": \"On\", \"state_off\": \"Off\", \"value_template\": \"{{ value_json.c }}\"}";
          client.publish(conf4.c_str(), Playload_on_off.c_str(), true);
          //Анонс переключателя авторежима 
          String conf5 = "homeassistant/switch/"+ch_id+"aut/config";
          String Playload_auto = "{\"name\": \"Auto\", \"command_topic\": \""+control+"\", \"state_topic\": \""+top+"\", \"payload_on\": \"1\", \"payload_off\": \"2\", \"state_on\": \"1\", \"state_off\": \"0\", \"value_template\": \"{{ value_json.a }}\"}";
          client.publish(conf5.c_str(), Playload_auto.c_str(), true);
          annonce_mqtt_discovery = true;
   // }
    // Отправляем данные 
    client.publish(tops.c_str(), data.c_str());
    client.subscribe(subscr.c_str());
}


String MQTT_status(){
    String response_message = "";
    if(client.connected()){
         response_message = "подключен";
      }else{
         response_message = "отключен";
    }
    return response_message;
} 
