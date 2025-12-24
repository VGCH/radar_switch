String millis2time() {
    unsigned long totalSeconds = millis() / 1000;
    
    int days = totalSeconds / 86400;           // 86400 = 24 * 3600
    int hours = (totalSeconds % 86400) / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    
    String timeStr = String(days) + ":" + 
                     twoDigits(hours) + ":" + 
                     twoDigits(minutes) + ":" + 
                     twoDigits(seconds);
    return timeStr;
}

 String twoDigits(int digits){
        if(digits < 10) {
          String i = '0'+String(digits);
          return i;
         }else{
          return String(digits);
         }
      }

void time_work(){
   if (captivePortal()) {  
    return;
  }

  if (validateToken()){
    StaticJsonDocument<200> doc;

     doc["time"]      = millis2time();
     doc["chanel1"]   = load_on ? "On" : "Off";
     doc["distance"]  = String(distance);
     doc["level1"]    = String(load_on);
     doc["auto_st"]   = settings.auto_en ? "checked" : "" ;
     doc["MQTT"]      = MQTT_status();

     String outjson;
     serializeJson(doc, outjson);
     server.send(200, "text", outjson);    
  }   
}



String JSON_DATA() {
    StaticJsonDocument<256> doc;  
    
    doc["c"]        = load_on ? "On" : "Off";
    doc["distance"] = String(distance, 1);  // 1 знак после запятой вместо 2
    doc["a"]        = String(settings.auto_en);
    doc["h_on"]     = String(settings.auto_on * 0.0003, 1);
    doc["h_off"]    = String(settings.auto_off * 0.0003, 1);
    doc["d_time"]   = String(settings.off_time);
    doc["filter_r"] = String(settings.r_filter);
    doc["filter_q"] = String(settings.q_filter);
    
    String outjson;
    serializeJson(doc, outjson);
    return outjson;
}
