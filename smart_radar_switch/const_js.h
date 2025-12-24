const char reb_d[] PROGMEM = R"rawliteral(
   function rebootdev() {
        modal.style.display = "block";
     }
   var t = 60;
   var time = document.getElementsByClassName("clock")[0];
     function fun() {
         t--;
        time.innerHTML = "<b>Оставшееся время: "+ t +" сек.</b>"
       if(t <= 0) {
         location.href = "/";
         clearInterval(inter);
       }
   }
   
  function rebootdev_s() {
      var msg = document.getElementsByClassName("modal-body")[0];
      var req5 = new XMLHttpRequest();
            req5.open('GET','/?page=dev_reboot');
            req5.onreadystatechange = function(){
              if (req5.readyState === 4 && req5.status === 200) {
                     msg.innerHTML ="<br><b>Выполняется перезапуск устройства...</b><br>"
                     var inter = setInterval("fun()", 1000);
              }
            }
        req5.send();
}
//  График
        const ctx = document.getElementById('graphic').getContext('2d');
        const data_g = new Array(100).fill(0); // 100 значений сразу
        let counter = 0;

        function draw() {
            ctx.clearRect(0, 0, 500, 150);
            
            // Всегда 100 столбцов
            for (let i = 0; i < 100; i++) {
                const maxVal = Math.max(...data_g);
                const scale = maxVal ? 150 / maxVal : 1;
                const height = data_g[i] * scale;
                
                ctx.fillStyle = '#2196F3';
                ctx.fillRect(i * 5, 150 - height, 5, height);
            }
        }

// Отрисовываем сразу
draw();

// js
       setInterval(server_time, 1000);
       var last_st = "";
       var last_st2 = "";
      function server_time(){
         var worktime = new XMLHttpRequest();
             worktime.open("GET","?page=time",true);
             worktime.onreadystatechange = function(){
          if (worktime.readyState === 4 && worktime.status === 200) {
                const obj = JSON.parse(worktime.responseText);
                document.getElementById("timew").innerHTML      = obj.time;
                document.getElementById("distance").innerHTML   = obj.distance;
                data_g.shift(); // Удаляем самый старый (слева)
                // добавляем данные в график
                data_g.push(Number(obj.distance)); // Новое значение добавляется в конец
                draw(); // Отрисуем график
                if(last_st !== obj.chanel1){
                   last_st = obj.chanel1;
                   if(obj.chanel1 === "On"){
                      document.getElementById("pbut").innerHTML = "<center><a href=\"#\" class=\"btn_on\" onclick=\"on_off()\">Выключить</a></center>";
                   }else{
                      document.getElementById("pbut").innerHTML = "<center><a href=\"#\" class=\"btn_off\" onclick=\"on_off()\">Включить</a></center>";
                   }
                 }
                   if(last_st2 !== obj.auto_st){
                    last_st2 = obj.auto_st;
                    document.getElementById("pbsw").innerHTML = "<label class=\"switch\"><input onclick=\"switch_b(this.checked ? 'on' : 'off')\" type=\"checkbox\" id=\"switch_auto\" "+obj.auto_st+" ><span class=\"slider round\"></span></div></label>";
                 }
                }
              }
             worktime.send();
           }
         
      function on_off(){
         var power = new XMLHttpRequest();
             power.open("GET","?page=power",true);
             power.onreadystatechange = function(){
          if (power.readyState === 4 && power.status === 200) {
                var data = power.responseText;
                var myArray = data.split("0");
                document.getElementById("pbut").innerHTML    = myArray[0];
                //document.getElementById("radios").innerHTML  = myArray[1];
                 }
              }
             document.getElementById("pbut").innerHTML  = "<center>Секундочку...</center>";
             power.send();
           }
       
      function switch_b(radio){
            var value = radio;
              var xhr = new XMLHttpRequest();
                  xhr.open('POST', '?page=indata', true);
                  xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
                  xhr.onreadystatechange = function() {
                   if (xhr.readyState === 4 && xhr.status === 200) {
                          console.log('Данные успешно отправлены на сервер' + value);
                        }
                    };
                  xhr.send('switch_auto=' + value);
                }

//flying
          var flying = document.getElementById('flying');
          flying.addEventListener('input', function() {
                  response.innerHTML = flying.value;
            });
          flying.addEventListener('change', function() {
            var value = flying.value;
            var xhr = new XMLHttpRequest();
            xhr.open('POST', '?page=indata', true);
            xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
            xhr.onreadystatechange = function() {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    document.getElementById('response').innerHTML = xhr.responseText;
                }
            };
            xhr.send('flevel=' + encodeURIComponent(value));
        });

)rawliteral";

//=========================================
const char js_wifi[] PROGMEM = R"rawliteral(
document.addEventListener('DOMContentLoaded', async function() {
    try {
        const response = await fetch('?page=scan_wifi');
        const data = await response.text(); 
        document.getElementById('response').innerHTML = data;
        
    } catch (error) {
        console.error('Ошибка загрузки:', error);
        document.getElementById('response').innerHTML = 
            `<div class="error">Ошибка загрузки ${error.message}</div>`;
    }
});
  )rawliteral";
//=========================================

const char html_style[] PROGMEM = R"rawliteral(
                  body {
                  background: linear-gradient(to bottom, #060345, #023f78, #540110, #013a42, #1f3d6e);
                    }
                  form, div.frame {
                       margin: 20px auto 0;
                       max-width: 500px;
                       padding: 10px;
                       border-radius: 10px;
                       background-color: #fff;
                    }
                  div#response2, div#response21 {
                       margin: 10px auto 0;
                       max-width: 500px;
                       padding: 20px;
                       border-radius: 10px;
                       background: linear-gradient(to bottom right, #9df3f5, #e6e8e7);
                    }
                    label {
                        display: block;
                        color: #5769a1;
                        margin: 10px;
                    }
                     input[type=text], input[type=password], input[type=number], input[type=file], select {
                        width: 100%;
                        padding: 10px;
                        margin-bottom: 15px;
                        border: 1px solid #ccc;
                        border-radius: 4px;
                        box-sizing: border-box;
                      }
                     input[type=submit] {
                        background-color: #3482f7;
                        color: white;
                        padding: 12px 20px;
                        border: none;
                        border-radius: 4px;
                        cursor: pointer;
                     }
                    h2 {
                        text-align: center;
                        margin-top: 10px;
                        font-size: 30px;
                        color: white;
                    }
                    text {
                       text-align: left;
                       margin-top: 5px;
                       font-size: 18px;
                       font-family: Noto Sans;
                       color: #023c52;
                     }
                    div.live {
                       text-align: right;
                       margin-top: 0px;
                       font-size: 30px;
                       font-family: Arial;
                       color: #066094;
                     }
                     div.livePM {
                       text-align: right;
                       margin-top: 0px;
                       font-size: 35px;
                       font-family: Monotype Corsiva;
                       color: #800404;
                     }
                    div.modal {
                       text-align: left;
                       margin-top: 0px;
                       font-size: 30px;
                       color: #066094;
                     }
                      a {
                       text-align: center;
                       margin-top: 0px;
                       font-size: 16px;
                       text-decoration: none;
                       color: #9ee8e8;
                     }
                     a.mod {
                       text-align: center;
                       margin-top: 0px;
                       font-size: 18px;
                       text-decoration: none;
                       color: #3e5d80;
                     }
                    .spoiler input, .spoiler div  { 
                        display: none; 
                     }
                    .spoiler :checked ~ div {
                     display: block;
                     }
                     footer {
                       text-align: center;
                       margin-top: 10px;
                       font-size: 15px;
                       color: #cbd6f2;
                     }
                    #myBtn {                     
                      background-color: #9C27B0;  
                      color: white; 
                      padding: 12px 16px;
                      font-size: 18px;   
                      font-weight: bold;   
                      border: none;   
                      cursor: pointer;   
                      width: 180px;        
                     }
                      .btn_off {
                       display: inline-block; /* Строчно-блочный элемент */
                       background: #0e8038; /* Серый цвет фона */
                       color: #fff; /* Белый цвет текста */
                       padding: 1rem 1.5rem; /* Поля вокруг текста */
                       text-decoration: none; /* Убираем подчёркивание */
                       border-radius: 3px; /* Скругляем уголки */
                      }  
                       .btn_on {
                       display: inline-block; /* Строчно-блочный элемент */
                       background: #800e1b; /* Серый цвет фона */
                       color: #fff; /* Белый цвет текста */
                       padding: 1rem 1.5rem; /* Поля вокруг текста */
                       text-decoration: none; /* Убираем подчёркивание */
                       border-radius: 3px; /* Скругляем уголки */
                      }                        
                      .modal {
                        display: none; /* Hidden by default */
                        position: fixed; /* Stay in place */
                        z-index: 1; /* Sit on top */
                        padding-top: 250px; /* Location of the box */
                        left: 0;
                        top: 0;
                        width: 100%; /* Full width */
                        height: 100%; /* Full height */
                        overflow: auto; /* Enable scroll if needed */
                        background-color: rgb(0,0,0); /* Fallback color */
                        background-color: rgba(0,0,0,0.4); /* Black w/ opacity */
                        border-radius: 10px;
                    }
                    .modal-content {
                        position: relative;
                        background-color: #97e1f7;
                        margin: auto;
                        padding: 0;
                        border: 0px solid #cdeef7;
                        width: fit-content;
                        border-radius: 10px;
                        box-shadow: 0 4px 8px 0 rgba(0,0,0,0.2),0 6px 20px 0 rgba(0,0,0,0.19);
                        -webkit-animation-name: animatetop;
                        -webkit-animation-duration: 0.4s;
                        animation-name: animatetop;
                        animation-duration: 0.4s;
                    }
                   @-webkit-keyframes animatetop {
                        from {top:-300px; opacity:0}
                        to {top:0; opacity:1}
                    }
                   @keyframes animatetop {
                        from {top:-300px; opacity:0}
                        to {top:0; opacity:1}
                    }
                     .close {
                        color: white;
                        float: right;
                        font-size: 28px;
                        font-weight: bold;
                    }
                    .close:hover,
                    .close:focus {
                        color: #000;
                        text-decoration: none;
                        cursor: pointer;
                    }
                    .modal-header {
                        padding: 5px 16px;
                        color: white;
                        font-size: 12px;
                        text-align: left;
                        border-radius: 10px;
                    }
                    .modal-body {
                        padding: 12px 16px;
                        font-size: 18px;
                        text-align: center;
                    }
                    .modal-footer {
                        padding: 12px 16px;
                        color: #702457;
                        font-size: 16px;
                        text-align: right;
                        border-radius: 10px;
                    }
                    .input-file {
                        position: relative;
                        display: inline-block;
                        padding: 10px 20px;
                        border: none;
                        border-radius: 4px;
                    }
                   .input-file span {
                       position: relative;
                       display: inline-block;
                       cursor: pointer;
                       outline: none;
                       text-decoration: none;
                       font-size: 14px;
                       vertical-align: middle;
                       color: rgb(255 255 255);
                       text-align: center;
                       border-radius: 4px;
                       background-color: #419152;
                       line-height: 22px;
                       height: 40px;
                       padding: 10px 20px;
                       box-sizing: border-box;
                       border: none;
                       margin: 0;
                       transition: background-color 0.2s;
                  }
                  .input-file input[type=file] {
                       position: absolute;
                       margin: 10;
                       z-index: -1;
                       opacity: 0;
                       display: block;
                       width: 0;
                       height: 0;
                    }
                   .hidden {
                      display: none; /* Скрыть элемент */
                    }
                  /* Focus */
                  .input-file input[type=file]:focus + span {
                      box-shadow: 0 0 0 0.2rem rgba(0,123,255,.25);
                    }
 
                  /* Hover/active */
                  .input-file:hover span {
                      background-color: #59be6e;
                   }
                  .input-file:active span {
                      background-color: #2E703A;
                    }
 
                  /* Disabled */
                  .input-file input[type=file]:disabled + span {
                      background-color: #eee;
                  }
                  
                  /* Переключатели*/
                  .switch {
                     position: relative;
                     display: inline-block;
                     width: 60px;
                     height: 34px;
                  }
                  
                  .switch input {display:none;}
                  
                  .slider {
                      position: absolute;
                      cursor: pointer;
                      top: 0;
                      left: 0;
                      right: 0;
                      bottom: 0;
                      background-color: #ccc;
                      -webkit-transition: .4s;
                      transition: .4s;
                  }
                  
                  .slider:before {
                      position: absolute;
                      content: "";
                      height: 26px;
                      width: 26px;
                      left: 4px;
                      bottom: 4px;
                      background-color: white;
                      -webkit-transition: .4s;
                      transition: .4s;
                  }
                  
                  input:checked + .slider {
                      background-color: #2196F3;
                  }
                  
                  input:focus + .slider {
                      box-shadow: 0 0 1px #2196F3;
                  }
                  
                  input:checked + .slider:before {
                     -webkit-transform: translateX(26px);
                     -ms-transform: translateX(26px);
                     transform: translateX(26px);
                  }
                  
                  /* Rounded sliders */
                  .slider.round {
                     border-radius: 34px;
                  }
                  
                  .slider.round:before {
                     border-radius: 50%;
                  }
                  
                   .slider2 {
                     display: flex;
                     justify-content: center;
                     align-items: center;
                     margin: 0 auto;
                     margin-top: 50px;
                     height: 30px;
                     max-width: 450px;
                     padding: 15px 10px;
                     border-radius: 10px;
                     background: #58b4fc;
                  }

                  .slider2--body {
                     background: rgb(250, 250, 250);
                     height: 4px;
                     width: 100%;
                     position: relative;
                     border-radius: 5px;
                  }

                  .slider2--progress {
                      position: absolute;
                      left: 100%;
                      right: 100%;
                      background: #163a94;
                      height: 4px;
                      border-radius: 10px;
                  }

                  .slider2--inputs {
                     position: relative;
                  }

                  .slider2--inputs > input {
                     pointer-events: none;
                  }

                  .slider2--input {
                      position: absolute;
                      top: -2.4px;
                      left: -3px;
                      height: 5px;
                      width: calc(100% + 2px);
                      background: none;
                      -webkit-appearance: none;
                  }

                  .slider2--input::-webkit-slider-thumb {
                    width: 15px;
                    height: 15px;
                    border-radius: 50%;
                    border: 1px solid #021033;
                    pointer-events: auto;
                    -webkit-appearance: none;
                    background: #021647;
                  }
                  #prg {
                    width:0;
                    color:white;
                    margin-bottom: 10px;
                    padding-top: 0px;
                    padding-bottom: 0px;
                    text-align:center;
                    border-radius: 3px;
                    border: none;
                  }
                 .chart-container {
                   max-width: 500px; /* Ограничиваем максимальную ширину */
                   width: 100%; /* Занимает всю доступную ширину до max-width */
                   margin: 0 auto;
                  }
        
                 #graphic {
                   width: 100%; /* Растягивается на весь контейнер */
                   height: 150px;
                   border: none;
                   background: #ebe6e6;
                   display: block; /* Важно для корректного отображения */
                   border-radius: 10px;
                  }
                  )rawliteral";

//================================================
const char divcode[] PROGMEM = R"rawliteral(
    <div id="myModal" class="modal">
       <div class="modal-content">
       <div class="modal-header">
          <span class="close">&times;</span>
          <text>Перезагрузка устройства</text>
       </div>
       <div class="modal-body"><br><a class ="btn_on" href="#" onclick="rebootdev_s()"><b>Перезагрузить устройство</b></a><br><br></div>
     <div class="modal-footer">
        <span class = "clock"> <br></span>
     </div>
   </div>
 </div>
 <script>
      var modal = document.getElementById("myModal");
      var span = document.getElementsByClassName('close')[0];
      span.onclick = function() {
          modal.style.display = "none";
        }
       window.onclick = function(event) {
          if(event.target == modal) {
            modal.style.display = "none";
          }
       }
    </script>)rawliteral";
 //==============================================
 // Скрипт для формы конфигурации
const char *config_js PROGMEM = R"rawliteral(
        const form = document.getElementById('config-form');
              form.addEventListener('submit', (event) => {
                    event.preventDefault();
                    const formData = new FormData(form);
                    const xhr = new XMLHttpRequest(); 
                          xhr.open('POST', '/?page=indata');  
                          xhr.addEventListener('load', () => {
                           if (xhr.status === 200) {
                                  document.getElementById('response').innerHTML = xhr.responseText;
                                    }else{
                                       document.getElementById('response').innerHTML = 'Ошибка отправки';
                                          }
                                    });
                          xhr.send(formData);
                       });

          document.getElementById('switch').addEventListener('change', function() {
                  var status_s = this.checked ? 'on' : 'off';
                  var xhr = new XMLHttpRequest();
                  xhr.open('POST', '?page=indata', true);
                  xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
                  xhr.onreadystatechange = function() {
                   if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
                            document.getElementById('response2').innerHTML = xhr.responseText;
                          //console.log('Данные успешно отправлены на сервер' + status_s);
                        }
                    };
                  xhr.send('switch=' + status_s);
                });
  )rawliteral";
 //========================================
 //JS смены пароля
 const char pass_js[] PROGMEM = R"rawliteral(
       const form = document.getElementById('config-form');
             form.addEventListener('submit', (event) => {
                  event.preventDefault();
                  const formData = new FormData(form);
                  const xhr = new XMLHttpRequest();
                        xhr.open('POST', '?page=indata');
                        xhr.addEventListener('load', () => {
                              if (xhr.status === 200) { 
                                    document.getElementById('response').innerHTML = xhr.responseText; 
                                   }else{
                                    document.getElementById('response').innerHTML = 'Ошибка отправки';
                                 }
                            });
                   var newPassword = document.getElementById("password").value;
                   var confirmNewPassword = document.getElementById("newpassword").value;
                      if (newPassword != confirmNewPassword) {
                               document.getElementById('response').innerHTML = 'Пароли не совпадают!';
                               return false;
                             }else{
                               xhr.send(formData);
                              }
                          });
  )rawliteral";
//=================================================
   //JS смены конфигурации авто режима
 const char aconf_js[] PROGMEM = R"rawliteral(
       const form = document.getElementById('config-form');
             form.addEventListener('submit', (event) => {
                  event.preventDefault();
                  const formData = new FormData(form);
                  const xhr = new XMLHttpRequest();
                        xhr.open('POST', '?page=indata');
                        xhr.addEventListener('load', () => {
                              if (xhr.status === 200) { 
                                    document.getElementById('response').innerHTML = xhr.responseText; 
                                   }else{
                                    document.getElementById('response').innerHTML = 'Ошибка отправки';
                                 }
                            });
                   var on_r = document.getElementById("on_range").value;
                   var off_r = document.getElementById("off_range").value;
                      if (off_r == on_r) {
                               console.log(on_r + '--' + off_r);
                               document.getElementById('response').innerHTML = 'Значения не должны равняться!';
                               return false;
                             }else{
                               xhr.send(formData);
                              }
                          });

const progressBar = document.querySelector('.slider2--progress');
const inputRange = document.querySelectorAll('.slider2--input');

let rangeGap = 50000;

inputRange.forEach(function (el) {
  el.addEventListener('input', function (e) {
    let minValue = parseInt(inputRange[0].value);
    let maxValue = parseInt(inputRange[1].value);
    if (maxValue - minValue < rangeGap) {
      if (e.target.classList.contains('input--min')) {
        inputRange[0].value = maxValue - rangeGap;
      } else if (e.target.classList.contains('input--max')) {
        inputRange[1].value = minValue + rangeGap;
      }
    } else {
      progressBar.style.left = (minValue / inputRange[0].max) * 100 + '%';
      progressBar.style.right = 100 - (maxValue / inputRange[1].max) * 100 + '%';
      document.getElementById('res_data').innerHTML = '<text>Диапазон активации: ' + (minValue * 0.0003).toFixed(2) + ' - ' + (maxValue * 0.0003).toFixed(2) +' см</text>';
     }
  });
}); )rawliteral";
  //====================================
  //Update js
  const char update_js[] PROGMEM = R"rawliteral(
         const form = document.getElementById('config-form');
               form.addEventListener('submit', (event) => {
               prg.style.backgroundColor = 'blue';
               event.preventDefault();
               const formData = new FormData(form);
               const xhr = new XMLHttpRequest();
                     xhr.open('POST', '/update');
                     xhr.upload.addEventListener('progress', p=>{
                      let w = Math.round(p.loaded/p.total*100) + '%';
                        if(p.lengthComputable){
                             prg.innerHTML = w;
                             prg.style.width = w;
                             prg.style.paddingTop = '10px';
                             prg.style.paddingBottom = '10px';
                          }
                        if(w == '100%') prg.style.backgroundColor = 'green';
                        });
                     xhr.addEventListener('load', () => {
                  if (xhr.status === 200) {
                         document.getElementById('response').innerHTML = xhr.responseText.replace('Update Success! Rebooting...','<text>Обновление выполнено успешно! Выполняется перезагрузка устройства, подождите...</text>'); 
                      }else{
                         document.getElementById('response').innerHTML = '<text>Ошибка отправки</text>'; 
                       }
               });
                     document.getElementById('response').innerHTML = '<text>Выполняется процесс обновления прошивки... Пожалуйста, не покидайте страницу в процессе обновления!</text>';
                     xhr.send(formData);
            }); )rawliteral";
//==============================================