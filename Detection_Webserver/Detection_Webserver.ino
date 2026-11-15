#include <WiFi.h>
#include <WebServer.h>
#include <DFRobot_HumanDetection.h>


// =====================================================
// DFRobot Human Detection Sensor
// ESP32 RX = GPIO16
// ESP32 TX = GPIO17
// =====================================================

DFRobot_HumanDetection hu(&Serial1);


// =====================================================
// ESP32 Access Point
// =====================================================

const char* AP_SSID = "HumanMonitor";
const char* AP_PASSWORD = "12345678";

WebServer server(80);


// =====================================================
// Sensor Data
// =====================================================

int humanPresence = 0;
int humanMovement = 0;
int movementRange = 0;
int respirationRate = 0;
int heartRate = 0;


// =====================================================
// Web Page
// =====================================================

const char MAIN_page[] PROGMEM = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta name="viewport"
      content="width=device-width, initial-scale=1">

<title>Human Monitor</title>


<style>

* {
    box-sizing: border-box;
}


body {

    margin: 0;

    padding: 20px;

    font-family: Arial, sans-serif;

    background: #f2f2f2;

    text-align: center;
}


h1 {

    margin-bottom: 5px;

    color: #222;

}


.subtitle {

    color: #666;

    margin-bottom: 25px;

}


.container {

    max-width: 900px;

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

    margin-top: 0;

    font-size: 20px;

    color: #444;

}


.value {

    font-size: 34px;

    font-weight: bold;

    margin-top: 15px;

}


.unit {

    margin-top: 5px;

    color: #777;

    font-size: 15px;

}


#presence {

    color: green;

}


#movement {

    color: blue;

}


#range {

    color: orange;

}


#respiration {

    color: purple;

}


#heart {

    color: red;

}


.status {

    display: inline-block;

    padding: 8px 16px;

    border-radius: 20px;

    background: #dff5df;

    color: green;

    font-weight: bold;

    margin-bottom: 20px;

}


.footer {

    margin-top: 30px;

    color: #777;

    font-size: 14px;

}


</style>

</head>


<body>


<h1>Human Detection Monitor</h1>

<div class="subtitle">
    ESP32 + DFRobot Human Detection
</div>


<div class="status" id="status"> Connected
</div>


<div class="container">


    <!-- Human Presence -->

    <div class="card">

        <h2>Human Presence</h2>

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

        <div class="unit">
            Parameter
        </div>

    </div>


    <!-- Respiration -->

    <div class="card">

        <h2>Respiration Rate</h2>

        <div class="value"
             id="respiration">

            --

        </div>

        <div class="unit">
            breaths/min
        </div>

    </div>


    <!-- Heart Rate -->

    <div class="card">

        <h2>Heart Rate</h2>

        <div class="value"
             id="heart">

            --

        </div>

        <div class="unit">
            BPM
        </div>

    </div>


</div>


<div class="footer">

    ESP32 Access Point:
    <b>192.168.4.1</b>

</div>


<script>


// =====================================================
// Update Sensor Data
// =====================================================

function updateData()
{

    fetch("/data")

    .then(response => response.json())

    .then(data =>
    {

        // ---------------------------------------------
        // Presence
        // ---------------------------------------------

        document.getElementById("presence")
            .innerHTML = data.presence;


        // ---------------------------------------------
        // Movement
        // ---------------------------------------------

        document.getElementById("movement")
            .innerHTML = data.movement;


        // ---------------------------------------------
        // Movement Range
        // ---------------------------------------------

        document.getElementById("range")
            .innerHTML = data.range;


        // ---------------------------------------------
        // Respiration
        // ---------------------------------------------

        document.getElementById("respiration")
            .innerHTML = data.respiration;


        // ---------------------------------------------
        // Heart Rate
        // ---------------------------------------------

        document.getElementById("heart")
            .innerHTML = data.heart;


        // ---------------------------------------------
        // Connection status
        // ---------------------------------------------

        document.getElementById("status")
            .innerHTML = "Connected";

    })

    .catch(error =>
    {

        document.getElementById("status")
            .innerHTML = "Connection Error";

    });

}


// =====================================================
// Update every 1 second
// =====================================================

setInterval(updateData, 1000);


// Initial update

updateData();


</script>


</body>

</html>

)rawliteral";


// =====================================================
// Read Sensor Data
// =====================================================

void readSensor()
{

    // -------------------------------------------------
    // Human Presence
    // -------------------------------------------------

    humanPresence =
        hu.smHumanData(
            hu.eHumanPresence
        );


    // -------------------------------------------------
    // Human Movement
    // -------------------------------------------------

    humanMovement =
        hu.smHumanData(
            hu.eHumanMovement
        );


    // -------------------------------------------------
    // Movement Range
    // -------------------------------------------------

    movementRange =
        hu.smHumanData(
            hu.eHumanMovingRange
        );


    // -------------------------------------------------
    // Respiration Rate
    // -------------------------------------------------

    respirationRate =
        hu.getBreatheValue();


    // -------------------------------------------------
    // Heart Rate
    // -------------------------------------------------

    heartRate =
        hu.getHeartRate();


    // =================================================
    // Serial Monitor
    // =================================================

    Serial.println();

    Serial.println(
        "================================"
    );


    // Human Presence

    Serial.print(
        "Human Presence: "
    );

    switch (humanPresence)
    {

        case 0:

            Serial.println(
                "No one is present"
            );

            break;


        case 1:

            Serial.println(
                "Someone is present"
            );

            break;


        default:

            Serial.println(
                "Read error"
            );

            break;

    }


    // Human Movement

    Serial.print(
        "Movement: "
    );

    switch (humanMovement)
    {

        case 0:

            Serial.println(
                "None"
            );

            break;


        case 1:

            Serial.println(
                "Still"
            );

            break;


        case 2:

            Serial.println(
                "Active"
            );

            break;


        default:

            Serial.println(
                "Read error"
            );

            break;

    }


    // Movement Range

    Serial.print(
        "Movement Range: "
    );

    Serial.println(
        movementRange
    );


    // Respiration

    Serial.print(
        "Respiration Rate: "
    );

    Serial.println(
        respirationRate
    );


    // Heart Rate

    Serial.print(
        "Heart Rate: "
    );

    Serial.println(
        heartRate
    );


    Serial.println(
        "================================"
    );

}


// =====================================================
// Root Web Page
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
// JSON Sensor Data
// =====================================================

void handleData()
{

    String presenceText;
    String movementText;


    // -------------------------------------------------
    // Presence Text
    // -------------------------------------------------

    if (humanPresence == 0)
    {

        presenceText = "No one";

    }

    else if (humanPresence == 1)
    {

        presenceText = "Present";

    }

    else
    {

        presenceText = "Error";

    }


    // -------------------------------------------------
    // Movement Text
    // -------------------------------------------------

    if (humanMovement == 0)
    {

        movementText = "None";

    }

    else if (humanMovement == 1)
    {

        movementText = "Still";

    }

    else if (humanMovement == 2)
    {

        movementText = "Active";

    }

    else
    {

        movementText = "Error";

    }


    // -------------------------------------------------
    // Create JSON
    // -------------------------------------------------

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


    json += "\"respiration\":";
    json += respirationRate;
    json += ",";


    json += "\"heart\":";
    json += heartRate;


    json += "}";


    server.send(
        200,
        "application/json",
        json
    );

}


// =====================================================
// Setup
// =====================================================

void setup()
{

    // -------------------------------------------------
    // Serial Monitor
    // -------------------------------------------------

    Serial.begin(115200);


    delay(1000);


    Serial.println();
    Serial.println();
    Serial.println(
        "=========================================="
    );

    Serial.println(
        "DFRobot Human Detection"
    );

    Serial.println(
        "ESP32 Web Server - AP Mode"
    );

    Serial.println(
        "=========================================="
    );


    // -------------------------------------------------
    // Sensor UART
    //
    // RX = GPIO16
    // TX = GPIO17
    // -------------------------------------------------

    Serial1.begin(
        115200,
        SERIAL_8N1,
        16,
        17
    );


    // -------------------------------------------------
    // Sensor Initialization
    // -------------------------------------------------

    Serial.println();

    Serial.println(
        "Start sensor initialization..."
    );


    while (
        hu.begin() != 0
    )
    {

        Serial.println(
            "Sensor initialization error!"
        );

        delay(1000);

    }


    Serial.println(
        "Sensor initialization successful!"
    );


    // -------------------------------------------------
    // Sleep Detection Mode
    // -------------------------------------------------

    Serial.println();

    Serial.println(
        "Switching to Sleep Detection Mode..."
    );


    while (
        hu.configWorkMode(
            hu.eSleepMode
        ) != 0
    )
    {

        Serial.println(
            "Work mode switch error!"
        );

        delay(1000);

    }


    Serial.println(
        "Sleep Detection Mode enabled!"
    );


    // -------------------------------------------------
    // HP LED ON
    // -------------------------------------------------

    hu.configLEDLight(
        hu.eHPLed,
        1
    );


    // -------------------------------------------------
    // Reset Sensor
    // -------------------------------------------------

    hu.sensorRet();


    Serial.println(
        "Sensor reset complete!"
    );


    // -------------------------------------------------
    // ESP32 Access Point
    // -------------------------------------------------

    Serial.println();

    Serial.println(
        "Starting ESP32 Access Point..."
    );


    WiFi.mode(WIFI_AP);


    bool result = WiFi.softAP(
        AP_SSID,
        AP_PASSWORD
    );


    if (result)
    {

        Serial.println(
            "Access Point started successfully!"
        );

    }

    else
    {

        Serial.println(
            "Access Point start FAILED!"
        );

    }


    // -------------------------------------------------
    // Print WiFi Information
    // -------------------------------------------------

    Serial.println();

    Serial.println(
        "------------------------------------------"
    );

    Serial.print(
        "WiFi Name    : "
    );

    Serial.println(
        AP_SSID
    );


    Serial.print(
        "WiFi Password: "
    );

    Serial.println(
        AP_PASSWORD
    );


    Serial.print(
        "ESP32 IP     : "
    );

    Serial.println(
        WiFi.softAPIP()
    );


    Serial.println(
        "------------------------------------------"
    );


    // -------------------------------------------------
    // Web Server
    // -------------------------------------------------

    server.on(
        "/",
        handleRoot
    );


    server.on(
        "/data",
        handleData
    );


    server.begin();


    Serial.println();

    Serial.println(
        "Web server started!"
    );


    Serial.println();

    Serial.println(
        "Connect your phone/laptop to:"
    );

    Serial.println(
        "WiFi: HumanMonitor"
    );

    Serial.println(
        "Then open:"
    );

    Serial.println(
        "http://192.168.4.1"
    );

    Serial.println();

}


// =====================================================
// Main Loop
// =====================================================

void loop()
{

    // -------------------------------------------------
    // Read sensor
    // -------------------------------------------------

    readSensor();


    // -------------------------------------------------
    // Handle Web Client
    // -------------------------------------------------

    server.handleClient();


    // -------------------------------------------------
    // Update interval
    // -------------------------------------------------

    delay(1000);

}
