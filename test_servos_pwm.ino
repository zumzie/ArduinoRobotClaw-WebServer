/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

#include <WiFi.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ESPmDNS.h>


// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;
uint8_t servonumtwo = servonum  + 1;
uint8_t servonumthree = servonumtwo + 1;

// Replace with your network credentials
const char* ssid     = "NETGEAR25";
const char* password = "strongiris114";

// Name local host
const char* host = "robotarm";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
String header_two;

// Decode HTTP GET value
String valueString = String(5);
String valueStringTwo = String(5);
String valueStringThree = String(5);

int pos1 = 0;
int pos2 = 0;

int pos1_two = 0;
int pos2_two = 0;

int pos1_three = 0;
int pos2_three = 0;


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  Serial.println("8 channel Servo test!");
  pwm.begin();

  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Set up MDNS host
   if (!MDNS.begin(host)) { //http://<hostname>.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
    delay(1000);
    }
    }
    Serial.println("mDNS responder started");

    
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  double pulselength;
  double pulselength_two;
  double pulselength_three;
  
  if (client) {                             // If a new client connects,
    currentTime = millis();

    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        header_two += header;
   
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.write("HTTP/1.1 200 OK\r\n");
            client.write("Content-type:text/html\r\n");
            client.write("Connection: close\r\n");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");

            //Embedded Video/Image
            client.println("<head><img src=\"http://127.0.0.1:8000/\" type=video/jpeg></video>");
             
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
           

            // Web Page - Servo 1 Slider
            client.println("</head><body><h1>ESP32 with Servo #1</h1>");
            client.println("<p>Position: <span id=\"servoPos\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider_servo_one\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\"" + valueString + "\"/>");

            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.write("$.ajaxSetup({timeout:5000}); function servo(pos) { \r\n");
            client.write("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>\r\n");
            

            // Web Page - Servo 2 Slider
            client.println("</head><body><h1>ESP32 with Servo #2</h1>");
            client.println("<p>Position: <span id=\"servoPos_Two\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider_servo_two\" id=\"servoSlider_Two\" onchange=\"servo_two(this.value)\" value=\"" + valueStringTwo + "\"/>");

            client.println("<script>var slider_Two = document.getElementById(\"servoSlider_Two\");");
            client.println("var servoP_Two = document.getElementById(\"servoPos_Two\"); servoP_Two.innerHTML = slider_Two.value;");
            client.println("slider_Two.oninput = function() { slider_Two.value = this.value; servoP_Two.innerHTML = this.value; }");
            client.write("$.ajaxSetup({timeout:5000}); function servo_two(pos_two) { \r\n");
            client.write("$.get(\"/?valuetwo=\" + pos_two + \"&\"); {Connection: close};}</script>\r\n");

            // Web Page - Servo 3 Slider
            client.println("</head><body><h1>ESP32 with Servo #3</h1>");
            client.println("<p>Position: <span id=\"servoPos_Three\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider_servo_three\" id=\"servoSlider_Three\" onchange=\"servo_three(this.value)\" value=\"" + valueStringThree + "\"/>");

            client.println("<script>var slider_Three = document.getElementById(\"servoSlider_Three\");");
            client.println("var servoP_Three = document.getElementById(\"servoPos_Three\"); servoP_Three.innerHTML = slider_Three.value;");
            client.println("slider_Three.oninput = function() { slider_Three.value = this.value; servoP_Three.innerHTML = this.value; }");
            client.write("$.ajaxSetup({timeout:5000}); function servo_three(pos_three) { \r\n");
            client.write("$.get(\"/?valuethree=\" + pos_three + \"&\"); {Connection: close};}</script>\r\n");
            
             
            //GET /?value=180& HTTP/1.1
            if (header.indexOf("GET /?value=") >= 0) {
              //Rotate the servo #1
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1 + 1, pos2);

              pulselength = map(valueString.toInt(), 0, 180, SERVOMIN, SERVOMAX);
              pwm.setPWM(servonum, 0, pulselength);
             
              Serial.println("ValueString: ");
              Serial.println(valueString);
              Serial.println("servonum: ");
              Serial.println(servonum);

             
              }
           
            if (header.indexOf("GET /?valuetwo=") >= 0){
              pos1_two = header.indexOf('=');
              pos2_two = header.indexOf('&');
              valueStringTwo = header.substring(pos1_two + 1, pos2_two);
              
              pulselength_two = map(valueStringTwo.toInt(), 0, 180, SERVOMIN, SERVOMAX);
              pwm.setPWM(1, 0, pulselength_two);

              Serial.println("ValueStringTwo: ");
              Serial.println(valueStringTwo);
              Serial.println("servonumtwo: ");
              Serial.println(servonumtwo);
              
            
            }

            if (header.indexOf("GET /?valuethree=") >= 0){
              pos1_three = header.indexOf('=');
              pos2_three = header.indexOf('&');
              valueStringThree = header.substring(pos1_three + 1, pos2_three);
              
              pulselength_three = map(valueStringThree.toInt(), 0, 180, SERVOMIN, SERVOMAX);
              pwm.setPWM(2, 0, pulselength_three);

              Serial.println("ValueStringThree: ");
              Serial.println(valueStringThree);
              Serial.println("servonumthree: ");
              Serial.println(servonumthree);
              
            
            }

        

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }

    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
