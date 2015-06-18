var j5 = require("johnny-five");
var board = new j5.Board();
 
var PROBE1LED = 8;
var PROBE1 = "A0";
var PROBE2LED = 9;
var PROBE2 = "A1";

board.on("ready", function(){
  var led1 = new j5.Led(PROBE1LED);
  var probe1 = new j5.Sensor({ pin: PROBE1, freq: 5000 });
 
  var led2 = new j5.Led(PROBE2LED);
  var probe2 = new j5.Sensor({ pin: PROBE2, freq: 5000 });

  var alertTempProbe1 = 85;
  var currentTempProbe1;
 
  var alertTempProbe2 = 85;
  var currentTempProbe2;

  probe1.on("change",  function(err, thmVoltage){
    currentTempProbe1 = convertVoltToTemp(thmVoltage);
 
    if (currentTempProbe1.tempF >= alertTempProbe1) {
      led1.on();
    } else {
      led1.off();
    }
 
    console.log("Current TempF: ", currentTempProbe1.tempF);
  });

  probe2.on("change",  function(err, thmVoltage){
    currentTempProbe2 = convertVoltToTemp(thmVoltage);

    if (currentTempProbe2.tempF >= alertTempProbe2) {
      led2.on();
    } else {
      led2.off();
    }

    console.log("Current TempF: ", currentTempProbe2.tempF);
  });
 
}); 

function convertVoltToTemp(volt){
  var tempK, tempC, tempF;
 
  // get the Kelvin temperature
  tempK = Math.log(((10240000/volt) - 10000));
  tempK = 1 / (0.001129148 + (0.000234125 * tempK) + (0.0000000876741 * 
      tempK * tempK * tempK));
 
  // convert to Celsius and round to 1 decimal place
  tempC = tempK - 273.15;
  tempC = Math.round(tempC*10)/10;
 
  // get the Fahrenheit temperature, rounded
  tempF = (tempC * 1.8) + 32;
  tempF = Math.round(tempF*10)/10;
 
  // return all three temperature scales
  return {
    tempK: tempK,
    tempC: tempC,
    tempF: tempF
  };
}
