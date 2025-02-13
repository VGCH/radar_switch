 String millis2time(){ // преобразование миллисекунд в вид д/ч/м/с
  
         int times =(millis()/1000);
         int days =  (times/3600)/24;
         int timehour =(((times)  % 86400L) / 3600); // часы
        if ( ((times % 3600) / 60) < 10 ) {
         int timehour = 0;
               }
         int timeminuts=((times  % 3600) / 60); // минуты
         if ( (times % 60) < 10 ) {
         int timeminuts = 0;
             }
         int timeseconds=(times % 60); // секунды
       String Time= String(days)+":"+String(twoDigits(timehour))+":"+String(twoDigits(timeminuts))+":"+String(twoDigits(timeseconds));
       return Time;
     }

 String twoDigits(int digits){
             if(digits < 10) {
          String i = '0'+String(digits);
          return i;
         }
          else {
        return String(digits);
         }
      }

void time_work(){
   if (captivePortal()) {  
    return;
  }
  String header;
  if (validateToken()){
   String outjson  ="{";
          outjson += "\"time\":\""+millis2time()+"\",";
          outjson += "\"chanel1\":";
  if(load_on){
          outjson  += "\"On\",";
     }else{
          outjson  += "\"Off\",";
           }
          outjson += "\"hum\":";
          outjson += "\""+String(hum)+"\",";
          outjson += "\"level1\":";
          outjson += "\""+String(load_on)+"\",";
          outjson += "\"auto_st\":";
          outjson += "\""+get_switch()+"\",";
          outjson += "\"MQTT\":\""+MQTT_status()+"\"";
          outjson += "}";
     server.send(200, "text", outjson);   
  }   
}



String JSON_DATA(){
     String outjson  = "{";
            outjson += "\"c\":";
     if(load_on){
            outjson  += "\"On\",";
        }else{
            outjson  += "\"Off\",";
           }
          outjson += "\"hum\":";
          outjson += "\""+String(hum)+"\",";
          outjson += "\"a\":";
          outjson += "\""+String(settings.auto_en)+"\",";
          outjson += "\"h_on\":";
          outjson += "\""+String(settings.auto_on * 0.0003)+"\",";
          outjson += "\"h_off\":";
          outjson += "\""+String(settings.auto_off * 0.0003)+"\"";
          outjson += "}";  
     return outjson;
}
