#include <WiFi.h>
#include <WebServer.h>
#include "DFRobot_HumanDetection.h"

//#define m1
//#define mm
#define m6
DFRobot_HumanDetection hu(&Serial1);
WebServer server(80);

// ===============================
// ESP32 AP
// ===============================

const char* AP_SSID = "HumanMonitor";
const char* AP_PASSWORD = "12345678";

// ===============================
// Sensor data
// ===============================

int humanPresence = 0;
int humanMovement = 0;
int movementRange = 0;
int fallState = 0;
int stationaryDwell = 0;
int bm ;
int count=0;
int count_second=0;
int count_first=0;

int da[3] = {0};
// ===============================
// Webpage
// git-------- ** check git diff fall_detection_webserver.ino **
// ===============================

const char MAIN_page[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>

<head>
 
<meta name="viewport"
      content="width=device-width, initial-scale=1">

<title>DFRobot Fall Detection</title>

<style>

body {
    font-family: Arial;
    background: #f2f2f2;
    text-align: center;
    margin: 0;
    padding: 20px;
}

h1 {
    color: #222;
}

.subtitle {
    color: #666;
    margin-bottom: 25px;
}

.container {
    max-width: 1000px;
    margin: auto;

    display: grid;

    grid-template-columns:
        repeat(auto-fit, minmax(220px, 1fr));

    gap: 18px;
}

.card {

    background: white;

    padding: 25px;

    border-radius: 15px;

    box-shadow:
        0 4px 12px rgba(0,0,0,0.15);
}

.card h2 {

    font-size: 19px;

    color: #444;
}

.value {

    font-size: 30px;

    font-weight: bold;

    margin-top: 15px;
}

.unit {

    color: #777;

    margin-top: 5px;
}

.footer {

    margin-top: 30px;

    color: #777;
}

</style>

</head>

<body>

<h1>DFRobot Human Detection</h1>

<div class="subtitle">
ESP32 Fall Detection Monitor
</div>

<div class="container">

<!-- Presence -->

<div class="card">

<h2> Human Presence</h2>

<div class="value"
     id="presence">

--

</div>

</div>


<!-- Movement -->

<div class="card">

<h2>Movement</h2>

<div class="value"
     id="movement">

--

</div>

</div>


<!-- Movement Range -->

<div class="card">

<h2> Movement Range</h2>

<div class="value"
     id="range">

--

</div>

</div>


<!-- Fall -->

<div class="card">

<h2> Fall Status</h2>

<div class="value"
     id="fall">

--

</div>

</div>


<!-- Stationary Dwell -->

<div class="card">

<h2> Stationary Dwell</h2>

<div class="value"
     id="dwell">

--

</div>

</div>

</div>


<div class="footer">

ESP32 AP:
<b>192.168.4.1</b>

</div>


<script>

function updateData()
{

    fetch("/data")

    .then(response => response.json())

    .then(data => {

        document.getElementById("presence")
            .innerHTML = data.presence;

        document.getElementById("movement")
            .innerHTML = data.movement;

        document.getElementById("range")
            .innerHTML = data.range;

        document.getElementById("fall")
            .innerHTML = data.fall;

        document.getElementById("dwell")
            .innerHTML = data.dwell;

    })

    .catch(error => {

        console.log(error);

    });

}

setInterval(updateData, 1000);

updateData();

</script>

</body>

</html>

)rawliteral";


// =====================================================
// READ SENSOR
// =====================================================

void readSensor()
{

    humanPresence =
        hu.smHumanData(
            hu.eHumanPresence
        );


    humanMovement =
        hu.smHumanData(
            hu.eHumanMovement
        );


    movementRange =
        hu.smHumanData(
            hu.eHumanMovingRange
        );


    fallState =
        hu.getFallData(
            hu.eFallState
        );


    stationaryDwell =
        hu.getFallData(
            hu.estaticResidencyState
        );


    // ==========================================
    // Serial Monitor
    // ==========================================

    //Serial.print("Existing information:");

    switch (humanPresence)
    {

        case 0:
            //Serial.println("No one is present");
            break;

        case 1:
            //Serial.println("Someone is present");
            break;

        default:
            //Serial.println("Read error");
            break;

    }


    //Serial.print("Motion information:");

    switch (humanMovement)
    {

        case 0:
            //Serial.println("None");
            break;

        case 1:
            //Serial.println("Still");
            break;

        case 2:
            //Serial.println("Active");
            break;

        default:
            //Serial.println("Read error");
            break;

    }


    //Serial.printf(
    //     "Body movement parameters:%d\n",
    //     movementRange
    // );


    //Serial.print("Fall status:");

    switch (fallState)
    {

        case 0:
            #ifdef m1
            //Serial.println("Not fallen");
            bm = movementRange;

             if(bm>80 && count ==0) 
             {
                count_first = bm;
                count++;

             }
             else if(count>0)
             {

            //  if(delay(400),readSensor(),count>0)
            //  {
                count_second = movementRange;
                
           if( abs(count_first-count_second)>=50) 

           {

Serial.println("=========****=========");

Serial.print("First data : ");
Serial.println(count_first);

Serial.print(" second data : ");
Serial.println(count_second);

Serial.print(" Diffrence : ");
Serial.println(abs(count_first-count_second));

Serial.println("=========****=========");
Serial.println("");
count_first =0;
count_second = 0;

            //  for(int i=0 ;i<6;i++)
            //  {
            //   digitalWrite(18,0);
            //   delay(500);
            //   digitalWrite(18,1);
            //   delay(500);
            // }
            digitalWrite(18,0);
             delay(2500);
             digitalWrite(18,1);
            count = 0;
count_first =0;
count_second = 0;
}


             }
            //  }
            //  if(bm>80 && count) ;
             ;//for(int i=0 ;i<6;i++)
            // {
            //  digitalWrite(18,0);
            //  delay(500);
            //  digitalWrite(18,1);
            //  delay(500);
            //};
            
            #endif





            #ifdef m6
            //Serial.println("Not fallen");
            bm = movementRange;

             if(bm>80 && count ==0) 
             {
                da[count] = bm;
                count++;

             }
             else if(count>0)
             {
             
             da[count] = bm;
             

            if(count==2)
            {

Serial.print("First data : ");
Serial.println(da[0]);

Serial.print(" second data : ");
Serial.println(da[1]);


Serial.print(" Third data : ");
Serial.println(da[2]);
count = 0;
}}
             count++;
            #endif
            


#ifdef m2 
            //Serial.println("Not fallen");
            bm = movementRange;

             if(count ==0) 
             {
                count_first = bm;
                count++;

             }
             
             else if(count>0)
             {
                count_second = bm;
                
           if(50 >= abs(count_first-count_second)) 

           {
             //Serial.print("First data : ");
//Serial.print(count_first);

Serial.print(" second data : ");
Serial.println(count_second);

             for(int i=0 ;i<6;i++)
             {
              digitalWrite(18,0);
              delay(500);
              digitalWrite(18,1);
              delay(500);
            }

            count = 0;

}


             }

            //  if(bm>80 && count) ;
             ;//for(int i=0 ;i<6;i++)
            // {
            //  digitalWrite(18,0);
            //  delay(500);
            //  digitalWrite(18,1);
            //  delay(500);
            //};
            
            #endif















            //digitalWrite(18,0);
            break;

        case 1:


          #ifdef mm  

            //Serial.println("Fallen");
            digitalWrite(18,0);
             delay(2500);
             digitalWrite(18,1);
Serial.println("=========****====fallen=====");
Serial.println("");
             Serial.println(movementRange);
             Serial.println("=========****====fallen=====");

                         #endif
            break;


 
        default:
            //Serial.println("Read error");
            break;

    }


    ////Serial.print("Stationary dwell status:");

    switch (stationaryDwell)
    {

        case 0:
            //Serial.println("No stationary dwell");
            break;

        case 1:
            //Serial.println("Stationary dwell present");
            break;

        default:
            //Serial.println("Read error");
            break;

    }

    //Serial.println();

}


// =====================================================
// WEB ROOT
// =====================================================

void handleRoot()
{

    server.send(
        200,
        "text/html",
        MAIN_page
    );

}


// =====================================================
// JSON DATA
// =====================================================

void handleData()
{

    String presenceText;
    String movementText;
    String fallText;
    String dwellText;


    // Presence

    if (humanPresence == 0)
        presenceText = "No one";

    else if (humanPresence == 1)
        presenceText = "Present";

    else
        presenceText = "Error";


    // Movement

    if (humanMovement == 0)
        movementText = "None";

    else if (humanMovement == 1)
        movementText = "Still";

    else if (humanMovement == 2)
        movementText = "Active";

    else
        movementText = "Error";


    // Fall

    if (fallState == 0)
        fallText = "Not fallen";

    else if (fallState == 1)
        fallText = "Fallen";

    else
        fallText = "Error";


    // Dwell

    if (stationaryDwell == 0)
        dwellText = "No stationary dwell";

    else if (stationaryDwell == 1)
        dwellText = "Stationary dwell present";

    else
        dwellText = "Error";


    String json = "{";

    json += "\"presence\":\"";
    json += presenceText;
    json += "\",";

    json += "\"movement\":\"";
    json += movementText;
    json += "\",";

    json += "\"range\":";
    json += movementRange;
    json += ",";

    json += "\"fall\":\"";
    json += fallText;
    json += "\",";

    json += "\"dwell\":\"";
    json += dwellText;

    json += "\"}";


    server.send(
        200,
        "application/json",
        json
    );

}


// =====================================================
// SETUP
// =====================================================

void setup()
{

    Serial.begin(115200);
    pinMode(18, OUTPUT);
    digitalWrite(18,1);
    //digitalWrite(18,1);
    Serial1.begin(
        115200,
        SERIAL_8N1,
        16,
        17
    );


    //Serial.println(
    //     "Start initialization"
    // );


    while (hu.begin() != 0)
    {

        Serial.println(
            "init error!!!"
        );

        delay(1000);

    }


    Serial.println(
        "Initialization successful"
    );


    //Serial.println(
    //     "Start switching work mode"
    // );

//     pinMode(18, OUTPUT);
 
    // ==========================================
    // তোমার ORIGINAL parameter
    // ==========================================

    while (
        hu.configWorkMode(
            hu.eFallingMode
        ) != 0
    )
    {

        //Serial.println(
        //     "error!!!"
        // );

        delay(1000);

    }


    //Serial.println(
    //     "Work mode switch successful"
    // );


    // ==========================================
    // তোমার ORIGINAL settings
    // ==========================================

    hu.configLEDLight(
        hu.eFALLLed,
        1
    );


    hu.configLEDLight(
        hu.eHPLed,
        1
    );

    data:

    hu.dmInstallHeight(270);


    hu.dmFallTime(1);


    hu.dmUnmannedTime(1);


    hu.dmFallConfig(
        hu.eResidenceTime,
        200
    );


    hu.dmFallConfig(
        hu.eFallSensitivityC,
        2
    );


    hu.sensorRet();


    // ==========================================
    // Print configuration
    // ==========================================

    //Serial.print(
    //     "Current work mode:"
    // );

    switch (hu.getWorkMode())
    {

        case 1:
            Serial.println(
                "Fall detection mode"
            );
            break;

        case 2:
            Serial.println(
                "Sleep detection mode"
            );
            break;

        default:
            Serial.println(
                "Read error"
            );
            break;

    }


    //Serial.print(
    //     "HP LED status:"
    // );

    switch (
        hu.getLEDLightState(
            hu.eHPLed
        )
    )
    {

        case 0:
            Serial.println("Off");
            break;

        case 1:
            Serial.println("On");
            break;

        default:
            Serial.println("Read error");
            break;

    }


    // //Serial.print(
    //     "FALL status:"
    // );

    switch (
        hu.getLEDLightState(
            hu.eFALLLed
        )
    )
    {

        case 0:
            Serial.println("Off");
            break;

        case 1:
            Serial.println("On");
            break;

        default:
            Serial.println("Read error");
            break;

    }


    Serial.printf(
        "Radar installation height: %d cm\n",
        hu.dmGetInstallHeight()
    );


    Serial.printf(
        "Fall duration: %d seconds\n",
        hu.getFallTime()
    );


    //Serial.printf(
    //     "Unattended duration: %d seconds\n",
    //     hu.getUnmannedTime()
    // );


    //Serial.printf(
    //     "Dwell duration: %d seconds\n",
    //     hu.getStaticResidencyTime()
    // );


    //Serial.printf(
        // "Fall sensitivity: %d\n",
        // hu.getFallData(
        //     hu.eFallSensitivity
        // )
    //);


    //Serial.println();
    //Serial.println(
    //     "==============================="
    // );


    // ==========================================
    // ESP32 ACCESS POINT
    // ==========================================

    WiFi.mode(WIFI_AP);


    WiFi.softAP(
        AP_SSID,
        AP_PASSWORD
    );


    //Serial.println();
    //Serial.println(
    //     "ESP32 Access Point started"
    // );


    //Serial.print(
    //     "WiFi Name: "
    // );

    //Serial.println(
    //     AP_SSID
    // );


    //Serial.print(
    //     "Password: "
    // );

    //Serial.println(
    //     AP_PASSWORD
    // );


    //Serial.print(
    //     "IP Address: "
    // );

    //Serial.println(
    //     WiFi.softAPIP()
    // );


    // ==========================================
    // Web Server
    // ==========================================

    server.on(
        "/",
        handleRoot
    );


    server.on(
        "/data",
        handleData
    );


    server.begin();


    //Serial.println(
    //     "Web server started"
    // );

}


// =====================================================
// LOOP
// =====================================================

void loop()
{

    readSensor();


    server.handleClient();


    delay(500);

}