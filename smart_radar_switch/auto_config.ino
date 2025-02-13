void auto_config_page(){
 if (!validateToken()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  String html = "<html><head><meta charset=\"UTF-8\"><title>Настройка автоматического режима</title>";
  html += "<link href=\"style.css\" rel=\"stylesheet\" type=\"text/css\" />";
  html += "</head><body>";
  html += "<h2>Настройка автоматического режима</h2>";
  html += "<form id=\"config-form\" >";
  html += "<div id=\"response\">";
  html += "<div id=\"res_data\"><text> Диапазон активации: " + String(settings.auto_on * 0.0003) + " - " + String(settings.auto_off * 0.0003) + " см </text></div>";
  html += "   <div class=\"slider2\">";
  html += "    <div class=\"slider2--body\">";
  html += "      <div class=\"slider2--progress\"></div>";
  html += "      <div class=\"slider2--inputs\">";
  html += "        <input type=\"range\" class=\"slider2--input input--min\" name=\"on_range\" id=\"on_range\" min=\"0\" , max=\"999999\" step=\"10\" value=\"" + String(settings.auto_on) + "\" />";
  html += "        <input type=\"range\" class=\"slider2--input input--max\" name=\"off_range\" id=\"off_range\" min=\"0\" , max=\"999999\" step=\"10\" value=\"" + String(settings.auto_off) + "\" />";
  html += "      </div>";
  html += "    </div>";
  html += "  </div><br><br><br>";
  html += "<input type=\"submit\" value=\"Сохранить\">";
  html += "</div>";
  html += "</form><br>";
  html += "<center><br><a href=\"/\">Вернуться назад</a><br></center>";
  html += "<footer>© <b>CYBEREX TECH</b>, 2025. Версия микро ПО <b>"+version_code+"</b>.</footer></html>";
  html += aconf_js;
  html += "</body></html>";
  server.send(200, "text/html", html);
}
