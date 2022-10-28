char page[] PROGMEM = R"(
  
  <!DOCTYPE html><html><head>
  <title>Local network</title>
  <style>
  body {margin-top:50px; font-family:Arial}
  body {font-size:20px; text-align:center}
  .btn {display:block; width:280px; margin:auto; padding:30px}
  .btn {font-size:30px; color:black; text-decoration:none}
  .on {background-color:green}
  .off {background-color:red}
  .values {background-color:Thistle}
  td {font-size:30px; margin-top:50px; margin-bottom:5px}
  p {font-size:30px; margin-top:50px; margin-bottom:5px}
  </style></head>
  <body>
  <h1> Hoseah and Fabrice Manual controller</h1>
  <table style='width:100%'><tr>
  </tr></table>
  <table style='width:80%'><tr>
  <td><button class = 'btn off' id='Motor'
       onclick = 'sendData(id)'>Turn Motor ON
       </button></td>
       <td><button class = 'btn off' id='Manual State'
     onclick = 'sendData(id)'>Turn Manual State ON
     </button></td>
</tr></table>
<p>Water level <span id='counter'>0</span> now</p>
<button class = 'btn values' id = 'values'
     onclick = 'sendData(id)'>See values</button>
<script>
function sendData(butn)
{
  var URL, variab, text;
  if(butn == 'Manual State') //changing to Manual Mode
  {
    URL = 'Manualurl';
    variab = 'MOTOR';
  }
  else if(butn == 'Motor')  // turning Motor on or off
  {
    URL = 'Motorurl';
    variab = 'MANUAL';
  }
  else if(butn == 'values')  // Probably use it to display current water level reading
  {
    URL = 'valuesurl';
    variab = 'counter';
  }
  if(butn == 'Manual State' || butn == 'Motor')  //Finding the state of the motor and automatic mode
{ // change button class and text 
  var state = document.getElementById(butn).className;
  state = (state == 'btn on' ? 'btn off' : 'btn on');
  text = (state == 'btn on' ? butn + ' OFF' : butn + ' ON');
    document.getElementById(butn).className = state;
    document.getElementById(butn).innerHTML = 'Turn ' + text;
  }
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function(butn)
  {
    if (this.readyState == 4 && this.status == 200)
    document.getElementById(variab).innerHTML = this.
    responseText;
  };
  xhr.open('GET', URL, true);
  xhr.send();
}
 
</script>
</body></html>
  
  
)";
