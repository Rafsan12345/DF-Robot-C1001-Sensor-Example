/**！
 * @file sleep.ino
 * @brief This is an example of sleep detection using human millimeter wave radar.
 * 
 * ---------------------------------------------------------------------------------------------------
 *    board   |             MCU                | Leonardo/Mega2560/M0 | ESP32 |
 *     VCC    |            3.3V/5V             |        VCC           |  VCC  |
 *     GND    |              GND               |        GND           |  GND  |
 *     RX     |              TX                |     Serial1 TX1      |   D2  |
 *     TX     |              RX                |     Serial1 RX1      |   D3  |
 * ---------------------------------------------------------------------------------------------------
 * 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [tangjie](jie.tang@dfrobot.com)
 * @version  V1.0
 * @date  2024-06-03
 * @url https://github.com/DFRobot/DFRobot_HumanDetection
 */

#include "DFRobot_HumanDetection.h"

// ========================= ADDITION =========================
#include <WiFi.h>
#include <WebServer.h>
// =============================================================


DFRobot_HumanDetection hu(&Serial1);


// ========================= ADDITION =========================
// ESP32 Access Point
const char* AP_SSID = "HumanMonitor";
const char* AP_PASSWORD = "12345678";

WebServer server(80);

// Webpage variables
int bedStatus = 0;
int sleepStatus = 0;
int awakeDuration = 0;
int deepSleepDuration = 0;
int sleepQuality = 0;

int presence = 0;
int averageRespiration = 0;
int averageHeartbeat = 0;
int turnoverNumber = 0;
int largeBodyMove = 0;
int minorBodyMove = 0;
int apneaEvents = 0;

int sleepDisturbances = 0;
int sleepQualityRating = 0;
int abnormalStruggle = 0;
// =============================================================



// ========================= ADDITION =========================
// Webpage
void handleRoot() {

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>Sleep Monitor</title>

<style>

body {
  font-family: Arial, sans-serif;
  background: #f2f2f2;
  margin: 0;
  padding: 20px;
}

h1 {
  text-align: center;
}

.container {
  max-width: 700px;
  margin: auto;
}

.card {
  background: white;
  padding: 15px;
  margin: 10px 0;
  border-radius: 10px;
  box-shadow: 0 2px 6px rgba(0,0,0,0.15);
}

.title {
  font-size: 14px;
  color: #666;
}

.value {
  font-size: 24px;
  font-weight: bold;
  margin-top: 5px;
}

.section {
  font-size: 20px;
  font-weight: bold;
  margin-top: 20px;
}

</style>

</head>

<body>

<div class="container">

<h1>🛏️ Sleep Monitor</h1>


<div class="section">Bed & Sleep Status</div>

<div class="card">
<div class="title">Bed Entry Status</div>
<div class="value" id="bed">--</div>
</div>

<div class="card">
<div class="title">Sleep Status</div>
<div class="value" id="sleep">--</div>
</div>

<div class="card">
<div class="title">Awake Duration</div>
<div class="value" id="awakeDuration">--</div>
</div>

<div class="card">
<div class="title">Deep Sleep Duration</div>
<div class="value" id="deepSleepDuration">--</div>
</div>

<div class="card">
<div class="title">Sleep Quality Score</div>
<div class="value" id="sleepQuality">--</div>
</div>


<div class="section">Human Information</div>

<div class="card">
<div class="title">Presence</div>
<div class="value" id="presence">--</div>
</div>

<div class="card">
<div class="title">Average Respiration</div>
<div class="value" id="respiration">--</div>
</div>

<div class="card">
<div class="title">Average Heart Rate</div>
<div class="value" id="heartbeat">--</div>
</div>

<div class="card">
<div class="title">Number of Turns</div>
<div class="value" id="turns">--</div>
</div>

<div class="card">
<div class="title">Significant Body Movement</div>
<div class="value" id="largeMove">--</div>
</div>

<div class="card">
<div class="title">Minor Body Movement</div>
<div class="value" id="minorMove">--</div>
</div>

<div class="card">
<div class="title">Apnea Events</div>
<div class="value" id="apnea">--</div>
</div>


<div class="section">Sleep Analysis</div>

<div class="card">
<div class="title">Sleep Abnormalities</div>
<div class="value" id="disturbance">--</div>
</div>

<div class="card">
<div class="title">Sleep Quality Rating</div>
<div class="value" id="rating">--</div>
</div>

<div class="card">
<div class="title">Abnormal Struggle</div>
<div class="value" id="struggle">--</div>
</div>

</div>


<script>

function updateData() {

  fetch('/data')
  .then(response => response.json())
  .then(data => {

    document.getElementById("bed").innerHTML =
      data.bed;

    document.getElementById("sleep").innerHTML =
      data.sleep;

    document.getElementById("awakeDuration").innerHTML =
      data.awakeDuration;

    document.getElementById("deepSleepDuration").innerHTML =
      data.deepSleepDuration;

    document.getElementById("sleepQuality").innerHTML =
      data.sleepQuality;


    document.getElementById("presence").innerHTML =
      data.presence;

    document.getElementById("respiration").innerHTML =
      data.respiration;

    document.getElementById("heartbeat").innerHTML =
      data.heartbeat;

    document.getElementById("turns").innerHTML =
      data.turns;

    document.getElementById("largeMove").innerHTML =
      data.largeMove;

    document.getElementById("minorMove").innerHTML =
      data.minorMove;

    document.getElementById("apnea").innerHTML =
      data.apnea;


    document.getElementById("disturbance").innerHTML =
      data.disturbance;

    document.getElementById("rating").innerHTML =
      data.rating;

    document.getElementById("struggle").innerHTML =
      data.struggle;

  });

}

setInterval(updateData, 1000);

updateData();

</script>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}


// JSON data
void handleData() {

  String json = "{";

  json += "\"bed\":\"";
  json += (bedStatus == 1 ? "In bed" : "Out of bed");
  json += "\",";

  json += "\"sleep\":\"";

  switch (sleepStatus) {
    case 0:
      json += "Deep sleep";
      break;

    case 1:
      json += "Light sleep";
      break;

    case 2:
      json += "Awake";
      break;

    case 3:
      json += "None";
      break;

    default:
      json += "Read error";
  }

  json += "\",";


  json += "\"awakeDuration\":\"";
  json += awakeDuration;
  json += "\",";

  json += "\"deepSleepDuration\":\"";
  json += deepSleepDuration;
  json += "\",";

  json += "\"sleepQuality\":\"";
  json += sleepQuality;
  json += "\",";


  json += "\"presence\":\"";
  json += (presence == 1 ? "Someone is present" : "No one");
  json += "\",";

  json += "\"respiration\":\"";
  json += averageRespiration;
  json += "\",";

  json += "\"heartbeat\":\"";
  json += averageHeartbeat;
  json += "\",";

  json += "\"turns\":\"";
  json += turnoverNumber;
  json += "\",";

  json += "\"largeMove\":\"";
  json += largeBodyMove;
  json += "\",";

  json += "\"minorMove\":\"";
  json += minorBodyMove;
  json += "\",";

  json += "\"apnea\":\"";
  json += apneaEvents;
  json += "\",";


  json += "\"disturbance\":\"";

  switch (sleepDisturbances) {

    case 0:
      json += "Sleep duration less than 4 hours";
      break;

    case 1:
      json += "Sleep duration more than 12 hours";
      break;

    case 2:
      json += "Long time abnormal absence";
      break;

    case 3:
      json += "None";
      break;

    default:
      json += "Read error";
  }

  json += "\",";


  json += "\"rating\":\"";

  switch (sleepQualityRating) {

    case 0:
      json += "None";
      break;

    case 1:
      json += "Good sleep quality";
      break;

    case 2:
      json += "Average sleep quality";
      break;

    case 3:
      json += "Poor sleep quality";
      break;

    default:
      json += "Read error";
  }

  json += "\",";


  json += "\"struggle\":\"";

  switch (abnormalStruggle) {

    case 0:
      json += "None";
      break;

    case 1:
      json += "Normal status";
      break;

    case 2:
      json += "Abnormal struggle status";
      break;

    default:
      json += "Read error";
  }

  json += "\"";

  json += "}";

  server.send(200, "application/json", json);
}
// =============================================================



void setup() {

  Serial.begin(115200);

  
  #if defined(ESP32) 
  Serial1.begin(115200, SERIAL_8N1, 16, 17); 
  #else 
  Serial1.begin(115200); 
  #endif 
 
  Serial.println("Start initialization"); 
  while (hu.begin() != 0) { 
    Serial.println("init error!!!"); 
    delay(1000); 
  } 
  Serial.println("Initialization successful"); 
 
  Serial.println("Start switching work mode"); 
  while (hu.configWorkMode(hu.eSleepMode) != 0) { 
    Serial.println("error!!!"); 
    delay(1000); 
  } 
  Serial.println("Work mode switch successful"); 
 
  Serial.print("Current work mode:"); 
  switch (hu.getWorkMode()) { 
    case 1: 
      Serial.println("Fall detection mode"); 
      break; 
    case 2: 
      Serial.println("Sleep detection mode"); 
      break; 
    default: 
      Serial.println("Read error"); 
  } 
 
  hu.configLEDLight(hu.eHPLed, 1);  // Set HP LED switch 
  hu.sensorRet();                   // Module reset, must perform sensorRet after setting data, otherwise the sensor may not be usable 
 
  Serial.print("HP LED status:"); 
  switch (hu.getLEDLightState(hu.eHPLed)) { 
    case 0: 
      Serial.println("Off"); 
      break; 
    case 1: 
      Serial.println("On"); 
      break; 
    default: 
      Serial.println("Read error"); 
  } 
 
  Serial.println(); 
  Serial.println(); 


  // ========================= ADDITION =========================
  // Start ESP32 Access Point

  WiFi.mode(WIFI_AP);

  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.println("================================");
  Serial.println("ESP32 Access Point started");
  Serial.print("WiFi Name: ");
  Serial.println(AP_SSID);

  Serial.print("Password: ");
  Serial.println(AP_PASSWORD);

  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/data", handleData);

  server.begin();

  Serial.println("Web server started");
  Serial.println("================================");
  // =============================================================
}


void loop() { 

  // ========================= ADDITION =========================
  // Read values for webpage

  bedStatus = hu.smSleepData(hu.eInOrNotInBed);

  sleepStatus = hu.smSleepData(hu.eSleepState);

  awakeDuration = hu.smSleepData(hu.eWakeDuration);

  deepSleepDuration = hu.smSleepData(hu.eDeepSleepDuration);

  sleepQuality = hu.smSleepData(hu.eSleepQuality);


  sSleepComposite comprehensiveState = hu.getSleepComposite();

  presence = comprehensiveState.presence;

  averageRespiration = comprehensiveState.averageRespiration;

  averageHeartbeat = comprehensiveState.averageHeartbeat;

  turnoverNumber = comprehensiveState.turnoverNumber;

  largeBodyMove = comprehensiveState.largeBodyMove;

  minorBodyMove = comprehensiveState.minorBodyMove;

  apneaEvents = comprehensiveState.apneaEvents;


  sleepDisturbances = hu.smSleepData(hu.eSleepDisturbances);

  sleepQualityRating = hu.smSleepData(hu.eSleepQualityRating);

  abnormalStruggle = hu.smSleepData(hu.eAbnormalStruggle);

  server.handleClient();
  // =============================================================


  // ================= ORIGINAL CODE =================

  Serial.print("Bed entry status:"); 
  switch (hu.smSleepData(hu.eInOrNotInBed)) { 
    case 0: 
      Serial.println("Out of bed"); 
      break; 
    case 1: 
      Serial.println("In bed"); 
      break; 
    default: 
      Serial.println("Read error"); 
  } 
 
  Serial.print("Sleep status:"); 
  switch (hu.smSleepData(hu.eSleepState)) { 
    case 0: 
      Serial.println("Deep sleep"); 
      break; 
    case 1: 
      Serial.println("Light sleep"); 
      break; 
    case 2: 
      Serial.println("Awake"); 
      break; 
    case 3: 
      Serial.println("None"); 
      break; 
    default: 
      Serial.println("Read error"); 
  } 

  Serial.print("Awake duration: "); 
  Serial.println(hu.smSleepData(hu.eWakeDuration)); 

  Serial.print("Deep sleep duration: "); 
  Serial.println(hu.smSleepData(hu.eDeepSleepDuration)); 

  Serial.print("Sleep quality score: "); 
  Serial.println( hu.smSleepData(hu.eSleepQuality)); 
 
  sSleepComposite comprehensiveState2 = hu.getSleepComposite(); 

  Serial.println("Comprehensive sleep status:{"); 
 
  Serial.print("\tExistence status: "); 
  switch (comprehensiveState2.presence) { 
    case 0: 
      Serial.println("No one"); 
      break; 
    case 1: 
      Serial.println("Someone is present"); 
      break; 
    default: 
      Serial.println("Read error"); 
  } 
 
  Serial.print("\tSleep status:"); 
  switch (comprehensiveState2.sleepState) { 
    case 0: 
      Serial.println("Deep sleep"); 
      break; 
    case 1: 
      Serial.println("Light sleep"); 
      break; 
    case 2: 
      Serial.println("Awake"); 
      break; 
    case 3: 
      Serial.println("None"); 
      break; 
    default: 
      Serial.println("Read error"); 
  } 
 
  Serial.print("\tAverage respiration rate: "); 
  Serial.println(comprehensiveState2.averageRespiration); 

  Serial.print("\tAverage heart rate: "); 
  Serial.println(comprehensiveState2.averageHeartbeat); 

  Serial.print("\tNumber of turns: "); 
  Serial.println(comprehensiveState2.turnoverNumber); 

  Serial.print("\tProportion of significant body movement: "); 
  Serial.println(comprehensiveState2.largeBodyMove); 

  Serial.print("\tProportion of minor body movement: "); 
  Serial.println(comprehensiveState2.minorBodyMove); 

  Serial.print("\tNumber of apneas: "); 
  Serial.println(comprehensiveState2.apneaEvents); 

  Serial.println("}"); 
 
  Serial.print("Sleep abnormalities:"); 
  switch (hu.smSleepData(hu.eSleepDisturbances)) { 
    case 0: 
      Serial.println("Sleep duration less than 4 hours"); 
      break; 
    case 1: 
      Serial.println("Sleep duration more than 12 hours"); 
      break; 
    case 2: 
      Serial.println("Long time abnormal absence of person"); 
      break; 
    case 3: 
      Serial.println("None"); 
      break; 
    default: 
      Serial.println("Read error"); 
  } 
 
  sSleepStatistics statistics = hu.getSleepStatistics(); 
  Serial.println("Sleep statistics:{"); 

  Serial.print("\tSleep quality score: "); 
  Serial.println(statistics.sleepQualityScore); 

  Serial.print("\tProportion of awake time: "); 
  Serial.println(statistics.sleepTime); 

  Serial.print("\tProportion of light sleep time: "); 
  Serial.println(statistics.wakeDuration); 

  Serial.print("\tProportion of light sleep time: "); 
  Serial.println(statistics.shallowSleepPercentage); 

  Serial.print("\tProportion of deep sleep time: "); 
  Serial.println(statistics.deepSleepPercentage); 

  Serial.print("\tOut of bed duration: "); 
  Serial.println(statistics.timeOutOfBed); 

  Serial.print("\tNumber of times out of bed: "); 
  Serial.println(statistics.exitCount); 

  Serial.print("\tNumber of turns: "); 
  Serial.println(statistics.turnOverCount); 

  Serial.print("\tAverage respiration: "); 
  Serial.println(statistics.averageRespiration); 

  Serial.print("\tAverage heartbeat: "); 
  Serial.println(statistics.averageHeartbeat); 

  Serial.println("}"); 
 
  Serial.print("Sleep quality rating: "); 
  switch (hu.smSleepData(hu.eSleepQualityRating)) { 
    case 0: 
      Serial.println("None"); 
      break; 
    case 1: 
      Serial.println("Good sleep quality"); 
      break; 
    case 2: 
      Serial.println("Average sleep quality"); 
      break; 
    case 3: 
      Serial.println("Poor sleep quality"); 
      break; 
    default: 
      Serial.println("Read error"); 
  } 
 
  Serial.print("Abnormal struggle status: "); 
  switch (hu.smSleepData(hu.eAbnormalStruggle)) { 
    case 0: 
      Serial.println("None"); 
      break; 
    case 1: 
      Serial.println("Normal status"); 
      break; 
    case 2: 
      Serial.println("Abnormal struggle status"); 
      break; 
    default: 
      Serial.println("Read error"); 
  } 
 
  Serial.println(); 
  Serial.println(); 
  delay(1000); 
}