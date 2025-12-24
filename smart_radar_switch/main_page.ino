void handleRoot() {
   if (captivePortal()) {  
    return;
  }
  //String header;
  if (!validateToken()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  String html = "<html lang=\"ru\"><head><meta charset=\"UTF-8\"><title>Управление модулем</title>";
  html += "<link href=\"style.css\" rel=\"stylesheet\" type=\"text/css\" />";
  html += "</head><body>";
  html += divcode;
  html += "<h2>Управление модулем</h2>";
  html += "<div class =\"frame\">";
  html += "<text>Время работы устройства:</text> <div class =\"live\" id=\"timew\">00:00:00:00</div>";
  html += "<text>Статус MQTT </text> <div class =\"live\" id=\"MQTT\">"+MQTT_status()+"</div>";
  html += "<text>Дистанция до объекта, см </text>  <div class =\"live\" id=\"distance\" >0.00</div>";
  html += "<div class=\"chart-container\"><canvas id=\"graphic\" width=\"500px\" ></canvas></div><br>";
  html += "<text>Автоматический режим </text><div class =\"live\"><div id=\"pbsw\" ><label class=\"switch\"><input onclick=\"switch_b(this.checked ? 'on' : 'off')\" type=\"checkbox\" id=\"switch_auto\" "+String(settings.auto_en ? "checked" : "")+" ><span class=\"slider round\"></span></div></label></div>";
  html += "<text>Задержка отключения, сек </text>\
  <div class =\"live\">\
  <input name=\"flevel\" id=\"flying\" type=\"range\"\
  min=\"1\" max=\"500\" value=\""+String(settings.off_time)+"\"\
  step=\"1\">\
  </div>";
  html += "<div class =\"live\"><div id=\"response\">"+String(settings.off_time)+"</div></div><br>";
  html += "<div id=\"pbut\" >";
  html += load_on ? "<center><a href=\"#\" class=\"btn_on\" onclick=\"on_off()\">Выключить</a></center>" : "<center><a href=\"#\" class=\"btn_off\" onclick=\"on_off()\">Включить</a></center>";
  html += "</div><br>";
  html += "<br></div></body>";
  html += "<center><br><a href=\"/?page=wlan_config\">Wi-Fi конфигурация</a><br>";
  html += "<a href=\"/?page=send_config\">Настройка передачи данных</a><br>";
  html += "<a href=\"/?page=a_config\">Настройка автоматического режима</a><br>";
  html += "<a href=\"/?page=changelp\">Изменение пароля устройства</a><br>";
  html += "<a href=\"/?page=update_fw\">Обновление прошивки</a><br>";
  html += "<a href=\"#\" onclick=\"rebootdev()\">Перезагрузить устройство</a><br>";
  html += "<a href=\"/login?DISCONNECT=YES\">Выход</a></center>";
  html += "<footer>© <b>CYBEREX TECH</b>, 2025. Версия микро ПО <b>"+version_code+"</b>.</footer>";
  html += "<script src=\"script.js?script=reb_d\"></script>"; 
  html += "</html>";
  server.send(200, "text/html", html);
}
