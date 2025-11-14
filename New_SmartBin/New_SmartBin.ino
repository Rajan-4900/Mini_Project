//Parteek
//www.justdoelectronics.com
//Smart Dustbin with SMS & GPS Pop-up on Web
//Arduino IDE for ESP-12E Module
//Board Manager URL: https://arduino.esp8266.com/stable/package_esp8266com_index.json

/*
 * ============================================================================
 * COMPONENT CONNECTIONS FOR ESP-12E MODULE
 * ============================================================================
 * 
 * ESP-12E PINOUT REFERENCE:
 * -------------------------
 * D0  (GPIO16) - Wake pin (not used)
 * D1  (GPIO5)  - I2C SCL (for LCD)
 * D2  (GPIO4)  - I2C SDA (for LCD)
 * D3  (GPIO0)  - SoftwareSerial RX (GSM TX)
 * D4  (GPIO2)  - SoftwareSerial TX (GSM RX)
 * D5  (GPIO14) - Echo Pin (Ultrasonic Sensor)
 * D6  (GPIO12) - Trig Pin (Ultrasonic Sensor)
 * D7  (GPIO13) - LED Pin
 * D8  (GPIO15) - Boot mode (not used)
 * 
 * 1. ULTRASONIC SENSOR (HC-SR04):
 *    -----------------------------
 *    VCC  -> 5V (or 3.3V if using 3.3V logic)
 *    GND  -> GND
 *    Trig -> D6 (GPIO12)
 *    Echo -> D5 (GPIO14)
 * 
 * 2. LCD DISPLAY (I2C 16x2):
 *    ------------------------
 *    VCC  -> 5V (or 3.3V)
 *    GND  -> GND
 *    SDA  -> D2 (GPIO4)
 *    SCL  -> D1 (GPIO5)
 *    Note: I2C address is 0x27 (change if different)
 * 
 *    TROUBLESHOOTING LCD BLACK SCREEN WITH RED LIGHT:
 *    -------------------------------------------------
 *    Problem: LCD shows black screen, I2C module has red light
 *    Solutions:
 *    1. Check I2C Address:
 *       - Common addresses: 0x27, 0x3F, 0x20, 0x38
 *       - Use I2C Scanner sketch (I2C_Scanner.ino) to find correct address
 *       - Update line 181: LiquidCrystal_I2C lcd(0x27, 16, 2);
 *    
 *    2. Check Wiring:
 *       - Verify SDA -> D2 (GPIO4) connection
 *       - Verify SCL -> D1 (GPIO5) connection
 *       - Ensure VCC is 5V (some modules need 5V, not 3.3V)
 *       - Check all GND connections are common
 *    
 *    3. Power Issues:
 *       - Red light on I2C module = power is OK
 *       - Try 5V instead of 3.3V for VCC
 *       - Check if I2C module has voltage regulator (some need 5V input)
 *    
 *    4. Contrast Adjustment:
 *       - Some I2C modules have contrast pot (potentiometer)
 *       - Adjust contrast if available
 *    
 *    5. I2C Scanner Code (upload separately to find address):
 *       #include <Wire.h>
 *       void setup() {
 *         Wire.begin();
 *         Serial.begin(9600);
 *         Serial.println("Scanning I2C...");
 *         for(byte addr=1; addr<127; addr++) {
 *           Wire.beginTransmission(addr);
 *           if(Wire.endTransmission()==0) {
 *             Serial.print("Found at 0x");
 *             Serial.println(addr,HEX);
 *           }
 *         }
 *       }
 *       void loop() {}
 * 
 * 3. GSM MODULE (SIM800L or similar):
 *    ---------------------------------
 *    VCC  -> 5V (or use external power supply for better stability)
 *    GND  -> GND
 *    TX   -> D3 (GPIO0) - ESP8266 RX
 *    RX   -> D4 (GPIO2) - ESP8266 TX
 *    Note: GSM modules may need external power supply (2A recommended)
 * 
 * 4. LED INDICATORS (Green and Red):
 *    --------------------------------
 *    OPTION 1: Single LED (Current Setup)
 *    - Green/Red LED Anode (+) -> D7 (GPIO13) through 220Ω-1kΩ resistor
 *    - LED Cathode (-) -> GND
 *    
 *    OPTION 2: Dual LED (Green + Red)
 *    - Green LED Anode (+) -> D7 (GPIO13) through 220Ω resistor -> GND
 *    - Red LED Anode (+) -> D8 (GPIO15) through 220Ω resistor -> GND
 *    - Both LED Cathodes (-) -> GND
 *    Note: Code currently uses D7 for single LED. Modify code for dual LED.
 * 
 * 5. POWER SUPPLY:
 *    -------------
 *    ESP-12E requires 3.3V (but can accept 5V on VIN with onboard regulator)
 *    Recommended: Use external 5V/2A power supply for stable operation
 *    Connect: 5V -> VIN, GND -> GND
 * 
 * IMPORTANT NOTES:
 * - Ensure all GND connections are common (shared ground)
 * - Use appropriate resistors for LED (220Ω-1kΩ)
 * - GSM module may require separate power supply for reliable operation
 * - Check I2C LCD address using I2C scanner if 0x27 doesn't work
 * - Ultrasonic sensor works best with 5V, but ESP8266 GPIOs are 3.3V tolerant
 * 
 * ============================================================================
 * STEP-BY-STEP SETUP GUIDE
 * ============================================================================
 * 
 * STEP 1: HARDWARE CONNECTIONS
 * -----------------------------
 * 1. Connect all GND pins together (common ground)
 * 2. Connect Ultrasonic Sensor:
 *    - VCC -> 5V, GND -> GND, Trig -> D6, Echo -> D5
 * 3. Connect LCD I2C:
 *    - VCC -> 5V, GND -> GND, SDA -> D2, SCL -> D1
 * 4. Connect GSM Module:
 *    - VCC -> 5V (or external supply), GND -> GND
 *    - TX -> D3, RX -> D4
 * 5. Connect LED:
 *    - Anode (+) -> D7 through 220Ω resistor
 *    - Cathode (-) -> GND
 * 6. Power ESP-12E:
 *    - Connect 5V to VIN, GND to GND
 * 
 * STEP 2: FIND LCD I2C ADDRESS (IF LCD NOT WORKING)
 * ---------------------------------------------------
 * 1. Upload I2C_Scanner.ino code (separate file provided)
 * 2. Open Serial Monitor (9600 baud)
 * 3. Note the address found (usually 0x27 or 0x3F)
 * 4. Update line 181 in this code with correct address
 * 
 * STEP 3: CONFIGURE CODE
 * -----------------------
 * 1. Update WiFi credentials (around lines 192-193):
 *    - ssid = "YourWiFiName"
 *    - password = "YourWiFiPassword"
 * 2. Update GPS location (around lines 201-202):
 *    - gpsLocation = "latitude,longitude" (get from Google Maps)
 *    - locationName = "Your Location Name"
 * 3. Update phone number (around line 205):
 *    - phoneNumber = "+91XXXXXXXXXX" (with country code)
 * 4. Update IFTTT key if needed (around line 198)
 * 
 * STEP 4: UPLOAD CODE
 * -------------------
 * 1. Select Board: Tools -> Board -> NodeMCU 1.0 (ESP-12E Module)
 * 2. Select Port: Tools -> Port -> (your COM port)
 * 3. Upload the code
 * 4. Open Serial Monitor (9600 baud) to see status
 * 
 * STEP 5: TEST
 * ------------
 * 1. Check Serial Monitor for WiFi connection and IP address
 * 2. Open web browser and go to: http://[IP_ADDRESS]
 * 3. Webpage should auto-refresh every 3 seconds
 * 4. When level >= 80%, pop-up will appear with GPS link
 * 5. SMS will be sent with GPS location link
 * 
 * STEP 6: TROUBLESHOOTING
 * -----------------------
 * - LCD Black Screen: Check I2C address, try 5V power, verify wiring
 * - WiFi Not Connecting: Check credentials, signal strength
 * - SMS Not Sending: Check GSM module power, SIM card, network
 * - Webpage Not Loading: Check IP address, ensure same WiFi network
 * 
 * ============================================================================
 */

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D3, D4);

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ctype.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = D6;
const int echoPin = D5;
const int led = D7;

long duration;
int distance;
float level;
bool alertSent = false;
bool whatsappAlertSent = false;

const char *ssid = "Hotspot Name";
const char *password = "Hotspot Password";

void send_event(const char *event);
bool sendWhatsAppAlert();

const char *host = "maker.ifttt.com";
const char *privateKey = "ifttt private key";

// GPS Location (Update with your actual coordinates)
const char *gpsLocation = "12.88xxxxx,77.55xxxxx"; // Format: "latitude,longitude"
const char *locationName = "DustBin Location"; // Your location name

// Phone number for SMS
const char *phoneNumber = "+91phoneNumber";

// Alert channel toggles
const bool enableGsmAlerts = false;
const bool enableWhatsAppAlerts = true;

// Twilio WhatsApp configuration (update with your credentials)
const char *twilioAccountSid = "YOUR_TWILIO_SID_HERE";
const char *twilioAuthToken = "YOUR_TWILIO_TOKEN_HERE";
const char *twilioFromNumber = "YOUR_TWILIO_WHATSAPP_NUMBER"; // Twilio sandbox or approved WhatsApp number
const char *twilioToNumber = "YOUR_PHONE_NUMBER"; // Recipient WhatsApp-enabled number

// Bin calibration (in centimeters)
const float binEmptyDistanceCm = 25.0; // Distance measured when bin is empty
const float binFullDistanceCm = 5.0;   // Distance measured when bin is completely full

WiFiServer server(80);
bool webAlertShown = false;

String urlEncode(const String &value);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  
  Serial.begin(9600);
  mySerial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Smart ");
  lcd.setCursor(0, 1);
  lcd.print("      Dustbin");
  delay(3000);
  lcd.clear();
  
  Serial.print("Connecting to Wifi Network: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("Successfully connected to WiFi.");
    Serial.print("IP address is : ");
    Serial.println(WiFi.localIP());
    server.begin();
    Serial.println("Server started");
  } else {
    Serial.println("");
    Serial.println("WiFi connection failed! Check credentials.");
  }
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000); // Add timeout (30ms)
  
  if (duration == 0) {
    Serial.println("Sensor timeout - check connections");
    delay(500);
    return;
  }
  
  distance = duration * 0.0340 / 2;
  Serial.println("Distance");
  Serial.println(distance);
  
  // Calculate fill level using calibrated distances
  float calibratedDistance = constrain((float)distance, binFullDistanceCm, binEmptyDistanceCm);
  float span = binEmptyDistanceCm - binFullDistanceCm;
  if (span <= 0) {
    span = 1; // Prevent division by zero
  }

  float filledHeight = binEmptyDistanceCm - calibratedDistance;
  level = (filledHeight / span) * 100.0;
  level = constrain(level, 0.0f, 100.0f);
  
  Serial.println("level");
  Serial.println(level);
  
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print(" Smart Dustbin");
  lcd.setCursor(0, 1);
  lcd.print(" Level");
  lcd.setCursor(8, 1);
  lcd.print((int)level);
  lcd.print("%");
  
  // LED control - turn on when level >= 80, off otherwise
  if (level >= 80) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  
  delay(200);
  
  WiFiClient client = server.available();
  if (client) {
    unsigned long timeout = millis() + 1000;
    while (!client.available() && millis() < timeout) {
      delay(1);
    }
    
    if (client.available()) {
      String request = client.readStringUntil('\r');
      client.flush();
      
      // Check if user acknowledged the alert
      if (request.indexOf("/alert-ack") != -1) {
        webAlertShown = true;
        Serial.println("Web alert acknowledged");
      }
      
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println("Refresh: 3"); // Auto-refresh every 3 seconds
      client.println();
      
      // Professional HTML with modern design
      client.println("<!DOCTYPE html>");
      client.println("<html lang='en'>");
      client.println("<head>");
      client.println("<meta charset='UTF-8'>");
      client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
      client.println("<meta http-equiv='refresh' content='3'>");
      client.println("<title>Smart Dustbin Monitor</title>");
      client.println("<style>");
      client.println("*{margin:0;padding:0;box-sizing:border-box}");
      client.println("body{font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);min-height:100vh;display:flex;justify-content:center;align-items:center;padding:20px}");
      client.println(".container{background:white;border-radius:20px;box-shadow:0 20px 60px rgba(0,0,0,0.3);padding:40px;max-width:600px;width:100%;text-align:center}");
      client.println("h1{color:#333;margin-bottom:30px;font-size:32px;font-weight:600}");
      client.println(".level-display{font-size:72px;font-weight:bold;margin:30px 0;transition:all 0.3s}");
      client.println(".level-green{color:#4CAF50}");
      client.println(".level-orange{color:#FF9800}");
      client.println(".level-red{color:#f44336;animation:pulse 1s infinite}");
      client.println("@keyframes pulse{0%,100%{transform:scale(1)}50%{transform:scale(1.05)}}");
      client.println(".progress-container{background:#e0e0e0;border-radius:25px;height:40px;margin:30px 0;overflow:hidden;box-shadow:inset 0 2px 5px rgba(0,0,0,0.1)}");
      client.println(".progress-bar{height:100%;border-radius:25px;transition:width 0.5s ease,background 0.3s;display:flex;align-items:center;justify-content:center;color:white;font-weight:bold;font-size:16px}");
      client.println(".progress-green{background:linear-gradient(90deg,#4CAF50,#45a049)}");
      client.println(".progress-orange{background:linear-gradient(90deg,#FF9800,#F57C00)}");
      client.println(".progress-red{background:linear-gradient(90deg,#f44336,#d32f2f)}");
      client.println(".status-box{padding:20px;border-radius:15px;margin:25px 0;font-size:20px;font-weight:600;box-shadow:0 4px 15px rgba(0,0,0,0.1)}");
      client.println(".status-ok{background:#E8F5E9;color:#2E7D32;border-left:5px solid #4CAF50}");
      client.println(".status-warning{background:#FFF3E0;color:#E65100;border-left:5px solid #FF9800}");
      client.println(".status-full{background:#FFEBEE;color:#C62828;border-left:5px solid #f44336}");
      client.println(".info-section{margin-top:30px;padding:20px;background:#f5f5f5;border-radius:15px;text-align:left}");
      client.println(".info-item{margin:10px 0;color:#666;font-size:14px}");
      client.println(".info-label{font-weight:600;color:#333}");
      client.println(".modal{display:none;position:fixed;z-index:1000;left:0;top:0;width:100%;height:100%;background:rgba(0,0,0,0.7);animation:fadeIn 0.3s}");
      client.println("@keyframes fadeIn{from{opacity:0}to{opacity:1}}");
      client.println(".modal-content{background:white;margin:5% auto;padding:0;border-radius:20px;width:90%;max-width:500px;box-shadow:0 10px 40px rgba(0,0,0,0.5);animation:slideDown 0.3s;overflow:hidden}");
      client.println("@keyframes slideDown{from{transform:translateY(-50px);opacity:0}to{transform:translateY(0);opacity:1}}");
      client.println(".modal-header{background:linear-gradient(135deg,#f44336,#d32f2f);color:white;padding:25px;text-align:center;font-size:24px;font-weight:bold}");
      client.println(".modal-body{padding:30px}");
      client.println(".modal-message{font-size:18px;color:#333;margin:20px 0;line-height:1.6;text-align:center}");
      client.println(".modal-location{background:#f5f5f5;padding:15px;border-radius:10px;margin:20px 0;border-left:4px solid #f44336}");
      client.println(".modal-button{background:#4CAF50;color:white;border:none;padding:15px 30px;font-size:16px;border-radius:8px;cursor:pointer;margin:10px;transition:all 0.3s;text-decoration:none;display:inline-block}");
      client.println(".modal-button:hover{background:#45a049;transform:translateY(-2px);box-shadow:0 5px 15px rgba(0,0,0,0.2)}");
      client.println(".modal-button.maps{background:#2196F3}");
      client.println(".modal-button.maps:hover{background:#0b7dda}");
      client.println(".auto-refresh{position:fixed;bottom:20px;right:20px;background:rgba(0,0,0,0.7);color:white;padding:10px 15px;border-radius:20px;font-size:12px}");
      client.println("</style>");
      client.println("</head>");
      client.println("<body>");
      
      client.println("<div class='container'>");
      client.println("<h1>🗑️ Smart Dustbin Monitor</h1>");
      
      // Level display with color coding
      client.print("<div class='level-display ");
      if (level >= 100) client.print("level-red");
      else if (level >= 80) client.print("level-orange");
      else client.print("level-green");
      client.print("'>");
      client.print((int)level);
      client.println("%</div>");
      
      // Progress bar
      client.println("<div class='progress-container'>");
      client.print("<div class='progress-bar ");
      if (level >= 100) client.print("progress-red");
      else if (level >= 80) client.print("progress-orange");
      else client.print("progress-green");
      client.print("' style='width:");
      client.print((int)level);
      client.print("%'>");
      client.print((int)level);
      client.println("%</div></div>");
      
      // Status message
      client.print("<div class='status-box ");
      if (level >= 100) {
        client.println("status-full'>🚨 DUSTBIN FULL! CLEAN IMMEDIATELY! 🚨</div>");
      } else if (level >= 80) {
        client.println("status-warning'>⚠️ WARNING: Almost Full! Clean Soon!</div>");
      } else {
        client.println("status-ok'>✓ Dustbin is Empty - All Good!</div>");
      }
      
      // Info section
      client.println("<div class='info-section'>");
      client.println("<div class='info-item'><span class='info-label'>📍 Location:</span> ");
      client.print(locationName);
      client.println("</div>");
      client.println("<div class='info-item'><span class='info-label'>🌐 IP Address:</span> ");
      client.print(WiFi.localIP());
      client.println("</div>");
      client.println("<div class='info-item'><span class='info-label'>🔄 Auto-refresh:</span> Every 3 seconds</div>");
      client.println("</div>");
      client.println("</div>");
      
      // Pop-up Modal for Alert
      if (level >= 80 && !webAlertShown) {
        client.println("<div id='alertModal' class='modal' style='display:block'>");
        client.println("<div class='modal-content'>");
        client.println("<div class='modal-header'>🚨 URGENT ALERT! 🚨</div>");
        client.println("<div class='modal-body'>");
        client.println("<div class='modal-message'>");
        client.print("<strong>Dustbin Level: ");
        client.print((int)level);
        client.println("%</strong><br><br>");
        if (level >= 100) {
          client.println("⚠️ Dustbin is COMPLETELY FULL!<br>Immediate cleaning required!");
        } else {
          client.println("⚠️ Dustbin is almost full!<br>Please clean soon!");
        }
        client.println("</div>");
        
        client.println("<div class='modal-location'>");
        client.println("<strong>📍 GPS Location:</strong><br>");
        client.print(locationName);
        client.println("<br>");
        client.print("Coordinates: ");
        client.print(gpsLocation);
        client.println("</div>");
        
        client.print("<a href='https://www.google.com/maps?q=");
        client.print(gpsLocation);
        client.println("' target='_blank'><button class='modal-button maps'>🗺️ Open in Google Maps</button></a>");
        
        client.println("<button class='modal-button' onclick='acknowledgeAlert()'>✓ I Got It!</button>");
        client.println("</div></div></div>");
        
        // JavaScript for modal
        client.println("<script>");
        client.println("function acknowledgeAlert(){");
        client.println("  document.getElementById('alertModal').style.display='none';");
        client.println("  fetch('/alert-ack');");
        client.println("}");
        client.println("</script>");
      }
      
      client.println("<div class='auto-refresh'>🔄 Auto-refreshing...</div>");
      client.println("</body></html>");
    }
    client.stop(); 
  }
  
  if (level >= 80 && !alertSent) {
    if (enableGsmAlerts) {
      SendMessage();
    }
    if (enableWhatsAppAlerts && !whatsappAlertSent) {
      if (sendWhatsAppAlert()) {
        whatsappAlertSent = true;
      }
    }
    send_event("jar_event");
    alertSent = true;
    webAlertShown = false; // Reset web alert to show pop-up
  }
  
  if (level < 75 && alertSent) {
    alertSent = false;
    whatsappAlertSent = false;
    webAlertShown = false;
    Serial.println("Alert reset - dustbin cleaned");
  }
}

void send_event(const char *event) {
  Serial.print("Connecting to ");
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  // We now create a URI for the request
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += privateKey;
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  
  unsigned long timeout = millis() + 5000; // 5 second timeout
  while (client.connected() && millis() < timeout) {
    if (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    } else {
      delay(10);
    }
  }
  Serial.println();
  Serial.println("closing connection");
  client.stop();
}

void SendMessage() {
  Serial.println("Sending SMS with GPS location...");
  mySerial.println("AT+CMGF=1"); // Set SMS to text mode
  delay(1000);
  
  mySerial.print("AT+CMGS=\"");
  mySerial.print(phoneNumber);
  mySerial.println("\"");
  delay(1000);
  
  // SMS message with GPS location
  mySerial.print("ALERT! Dustbin1 is ");
  mySerial.print((int)level);
  mySerial.print("% Full. Please clean immediately! Location: ");
  mySerial.print(locationName);
  mySerial.print(" - https://www.google.com/maps?q=");
  mySerial.print(gpsLocation);
  
  delay(200);
  mySerial.println((char)26); // Send Ctrl+Z to end message
  delay(2000);
  
  Serial.println("SMS sent with GPS location!");
}

String urlEncode(const String &value) {
  String encoded = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < value.length(); i++) {
    c = value.charAt(i);
    if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
      encoded += c;
    } else if (c == ' ') {
      encoded += '+';
    } else {
      code1 = (c & 0x0F) + '0';
      if ((c & 0x0F) > 9) {
        code1 = (c & 0x0F) - 10 + 'A';
      }
      code0 = (c >> 4) + '0';
      if ((c >> 4) > 9) {
        code0 = (c >> 4) - 10 + 'A';
      }
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}

bool sendWhatsAppAlert() {
  if (!enableWhatsAppAlerts) {
    return false;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WhatsApp alert skipped - WiFi not connected");
    return false;
  }

  if (String(twilioAccountSid).startsWith("ACXXXXXXXXXXXXXXXX") ||
      String(twilioAuthToken) == "your_auth_token") {
    Serial.println("WhatsApp alert skipped - update Twilio credentials");
    return false;
  }

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  String url = "https://api.twilio.com/2010-04-01/Accounts/";
  url += twilioAccountSid;
  url += "/Messages.json";

  if (!https.begin(client, url)) {
    Serial.println("Unable to connect to Twilio API");
    return false;
  }

  https.setAuthorization(twilioAccountSid, twilioAuthToken);
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String message = "ALERT! Dustbin1 is ";
  message += String((int)level);
  message += "% full at ";
  message += locationName;
  message += ". Clean immediately! Location: https://www.google.com/maps?q=";
  message += gpsLocation;

  String postData = "To=";
  postData += urlEncode(String("whatsapp:") + twilioToNumber);
  postData += "&From=";
  postData += urlEncode(String("whatsapp:") + twilioFromNumber);
  postData += "&Body=";
  postData += urlEncode(message);

  int httpCode = https.POST(postData);
  bool success = httpCode >= 200 && httpCode < 300;

  if (success) {
    Serial.println("WhatsApp alert sent via Twilio");
  } else {
    Serial.print("Failed to send WhatsApp alert, HTTP code: ");
    Serial.println(httpCode);
    if (httpCode > 0) {
      Serial.println(https.getString());
    }
  }

  https.end();
  return success;
}
