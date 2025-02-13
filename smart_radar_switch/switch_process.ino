void powerS(){
  String response_message ="";
  if(load_on){
      digitalWrite(on_off_pin, LOW);
      digitalWrite(on_off_ind, LOW);
      response_message += "<center><a href=\"#\" class=\"btn_off\" onclick=\"on_off()\">Включить</a></center>";
      load_on = false;
   }else{
      digitalWrite(on_off_pin, HIGH);
      digitalWrite(on_off_ind, HIGH);
      response_message  += "<center><a href=\"#\" class=\"btn_on\" onclick=\"on_off()\">Выключить</a></center>";
      load_on = true;
  }
      
    // server.send(200, "text/html", response_message + "0" +radio_b());
     server.send(200, "text/html", response_message + "0 ytn" );
     if(settings.mqtt_en){ MQTT_send_data("jsondata", JSON_DATA());}
}

void auto_process(){                                                     // Автоматический режим управления вентилятором
  if (millis() - auto_check_time >= 1000 && settings.auto_en ) {
     if(settings.auto_on != settings.auto_off){
        if(hum > (settings.auto_on * 0.0003) && (settings.auto_off * 0.0003) > hum && !load_on){ 
             powerS(); 
             off_status = false;
          }
        else if(hum < (settings.auto_on * 0.0003) && load_on || (settings.auto_off * 0.0003) < hum && load_on){ 
              if(!off_status){
                 off_status = true;
                 impOnDelay  = millis(); 
              }else{
                 if(millis() - impOnDelay >= 5000){     // Задержка на отключение  5 секунд
                    powerS(); 
                    off_status = false;
                 }
              }
          }
      }
   auto_check_time = millis();
  }
     
}

void radar_process(){                                                   // Чтение и обработка данных с радара
      if(Serial.available() > 0) {
          char inChar = Serial.read();
          if (inChar == '\n') {
            processString(inputString);
            inputString = "";
           }else{
          inputString += inChar;
         }
       }
}

void processString(String str) {
  int startIndex = str.indexOf(':') + 1;                                // Находим позицию после ':'
  if (startIndex > 0 && startIndex < str.length()) {
    String numberString = str.substring(startIndex);                    // Берем подстроку после ':'
    float get_data = numberString.toFloat();                            // Преобразуем строку в число
     if(get_data != 105.00){                                            // Исключаем кривые данные
        hum = get_data; 
     }
                                          
  }
}


               

void callback(char* topic, byte* payload, unsigned int length) {
           String message;
         for (int i = 0; i < length; i++) {
             message = message + (char)payload[i];
          }
          if(message != "On" || message != "Off"){
             int data = message.toInt();
             if(data == 0){
                 powerS();
             }
             if(data == 1){
                 settings.auto_en = true;
                 save_config();
                 if(settings.mqtt_en){ MQTT_send_data("jsondata", JSON_DATA());}
             }
             if(data == 2){
                 settings.auto_en = false;
                 save_config();
                 if(settings.mqtt_en){ MQTT_send_data("jsondata", JSON_DATA());}
             }
             
           }
 }
