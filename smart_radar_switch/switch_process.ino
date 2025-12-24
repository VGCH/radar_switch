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

void auto_process(){                                                     // Автоматический режим 
  if (millis() - auto_check_time >= 1000 && settings.auto_en ) {
     if(settings.auto_on != settings.auto_off){
        if(distance > (settings.auto_on * 0.0003) && (settings.auto_off * 0.0003) > distance){ 
          if(!load_on){ powerS(); }
              off_status = false;
              //level_couts++;
          }
        if(distance < (settings.auto_on * 0.0003) && load_on || (settings.auto_off * 0.0003) < distance && load_on){ 
              if(!off_status){
                 off_status = true;
                 impOnDelay  = millis(); 
              }else{
                 if(millis() - impOnDelay >= settings.off_time * 1000){     // Задержка на отключение (по умолчанию 10 секунд)
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
// Фильтрация сигнала с датчика
float filter_distance(float raw_distance) {
    Pc = P + Q;
    G = Pc/(Pc + R);
    P = (1-G)*Pc;
    Xp = Xe;
    Zp = Xp;
    Xe = G*(raw_distance-Zp)+Xp; // фильтрованное значение
    return(Xe);
}

void processString(String str) {
  int startIndex = str.indexOf(':') + 1;                                // Находим позицию после ':'
  if (startIndex > 0 && startIndex < str.length()) {
    String numberString = str.substring(startIndex);                    // Берем подстроку после ':'
    float get_data = numberString.toFloat();                            // Преобразуем строку в число
     if(get_data != 105.00){                                            // Исключаем кривые данные
        distance = filter_distance(get_data); 
     }
                                          
  }
}


               

void callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) {
        message = message + (char)payload[i];
    }
    
    // Пробуем распарсить как JSON
    if (message.startsWith("{")) {
        DynamicJsonDocument doc(512);
        DeserializationError error = deserializeJson(doc, message);
        
        if (!error) {
            // Обработка минимального порога (auto_off)
            if (doc.containsKey("min_threshold")) {
                float min_val = doc["min_threshold"];
                // Преобразуем обратно: делим на 0.0003 (или умножаем на 3333.33)
                settings.auto_off = min_val / 0.0003;  // Обратное преобразование
                save_config();
                
                if(settings.mqtt_en) { 
                    MQTT_send_data("jsondata", JSON_DATA());
                }
                return;
            }
            
            // Обработка максимального порога (auto_on)
            if (doc.containsKey("max_threshold")) {
                float max_val = doc["max_threshold"];
                // Преобразуем обратно: делим на 0.0003
                settings.auto_on = max_val / 0.0003;  // Обратное преобразование
                save_config();
                
                if(settings.mqtt_en) { 
                    MQTT_send_data("jsondata", JSON_DATA());
                }
                return;
            }

            // Обработка порога задержки отключения
            if (doc.containsKey("delaytime_threshold")) {
                float time_val = doc["delaytime_threshold"];
                settings.off_time = time_val;  // Сохраняем время задержки в сек
                save_config();
                
                if(settings.mqtt_en) { 
                    MQTT_send_data("jsondata", JSON_DATA());
                }
                return;
            }

            // Обработка порога коэфф фильтра R 
            if (doc.containsKey("filter_r_threshold")) {
                float r_val = doc["filter_r_threshold"];
                settings.r_filter = r_val;  // Сохраняем коэффициент R в память
                R = r_val; 
                save_config();
                
                if(settings.mqtt_en) { 
                    MQTT_send_data("jsondata", JSON_DATA());
                }
                return;
            }

            // Обработка порога коэфф фильтра Q 
            if (doc.containsKey("filter_q_threshold")) {
                float q_val = doc["filter_q_threshold"];
                settings.q_filter = q_val;  // Сохраняем коэффициент Q в память
                Q = q_val;
                save_config();
                
                if(settings.mqtt_en) { 
                    MQTT_send_data("jsondata", JSON_DATA());
                }
                return;
            }
        }
    }
    
    // Обработка команд 0, 1, 2
    if(message != "On" && message != "Off"){  
        int data = message.toInt();
        if(data == 0){
            powerS();
        }
        if(data == 1){
            settings.auto_en = true;
            save_config();
            if(settings.mqtt_en){ 
                MQTT_send_data("jsondata", JSON_DATA());
            }
        }
        if(data == 2){
            settings.auto_en = false;
            save_config();
            if(settings.mqtt_en){ 
                MQTT_send_data("jsondata", JSON_DATA());
            }
        }
    }
}
