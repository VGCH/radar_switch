void indata(){
     if (captivePortal()) { 
    return;
  }
  if (!validateToken()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
    if (server.hasArg("switch")){  
    String msg;   
      if(server.arg("switch")=="on"){
        settings.mqtt_en = true;
        save_config();
        msg +="<text>Передача данных включена!</text>";
         String input = settings.mqtt_serv;
         int colonIndex = input.indexOf(':');
         String ipAddress;
         String port;

        if (colonIndex != -1) {
             ipAddress = input.substring(0, colonIndex);
             port = input.substring(colonIndex + 1);
          }
        client.setServer(ipAddress.c_str(), port.toInt());
        }else if(server.arg("switch")=="off"){
        settings.mqtt_en = false;
        save_config();
        msg +="<text>Передача данных выключена!</text>";
    } 
    server.send(200, "text/html", msg);
  }
   if (server.hasArg("switch_auto")){  
    String msg;   
      if(server.arg("switch_auto")=="on"){
        settings.auto_en = true;
        save_config();
        msg +="<text>Передача данных включена!</text>";
        }else if(server.arg("switch_auto")=="off"){
        settings.auto_en = false;
        save_config();
        msg +="<text>Передача данных выключена!</text>";
    } 
    server.send(200, "text/html", msg);
    if(settings.mqtt_en){ MQTT_send_data("jsondata", JSON_DATA());}
  }

 

 if(server.hasArg("IPMQTT") && server.hasArg("PASSWORD")){  
    strncpy(settings.mqtt_serv, server.arg("IPMQTT").c_str(), MAX_STRING_LENGTH);
    strncpy(settings.mqtt_user, server.arg("USERNAME").c_str(), MAX_STRING_LENGTH);                       // Логин mqtt
    strncpy(settings.mqtt_passw, server.arg("PASSWORD").c_str(), MAX_STRING_LENGTH);                       // Пароль mqtt
    strncpy(settings.mqtt_topic, server.arg("topicname").c_str(), MAX_STRING_LENGTH);                       // Корень mqtt 
    save_config();
    
    String data = "";
    data += "<text> IP адрес сервера MQTT: <b>"+String(settings.mqtt_serv)+"</b></text><br>";
    data += "<text> Логин: <b>"+String(settings.mqtt_user)+"</b></text><br>";
    data += "<text> Идентификатор устройства: <b>"+String(ESP.getChipId())+"</b></text><br>";
    data += "<text> Топик данных: <b>"+String(settings.mqtt_topic)+"</b></text><br>";
    data += "<text><b>Данные успешно сохранены!</b></text>";
    server.send(200, "text/html", data);
  }
  
   if(server.hasArg("NEWPASSWORD")){  
       if(server.arg("NEWPASSWORD") != ""){
           strncpy(settings.passwd, server.arg("NEWPASSWORD").c_str(), MAX_STRING_LENGTH);
           save_config();
    }
   
    String data = "";
    data += "<text><b>Новый пароль успешно сохранён!</b></text>";
    server.send(200, "text/html", data);
  }
     if(server.hasArg("on_range") && server.hasArg("off_range")){  
       if(server.arg("on_range") != "" && server.arg("off_range") != "" ){
           settings.auto_off = server.arg("off_range").toFloat(); 
           settings.auto_on  = server.arg("on_range").toFloat();
           save_config();
    }
   
    String data = "";
    data += "<text><b>Данные автоматизации успешно сохранены!</b></text>";
    server.send(200, "text/html", data);
  }
 /* if (server.hasArg("mode")){     
     String moder = server.arg("mode");
     settings.counter_coeff = moder.toInt();
     fan_level = moder.toInt();
     level = map (fan_level, 0, 100, 1, set_delay);
     save_config();
     String data  = "<text><b>Данные успешно сохранены!</b></text><br>";
     
     server.send(200, "text/html", data);
  }*/
}
