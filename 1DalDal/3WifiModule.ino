class WebServer {
public:

  const char* ssid     = "Bublifuk";
  const char* password = "775116855";

  void connectToWifi() {
     WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Place this IP address into a browser window");
  }

  void processClients(WiFiServer& server) {
    WiFiClient client = server.available();   // listen for incoming clients

    if (client) {                             
      Serial.println("new client");          
      String currentLine = "";                   // make a String to hold incoming data from the client
      while (client.connected()) {            
        if (client.available()) {                // if there's client data
          char c = client.read();                // read a byte
            if (c == '\n') {                     // check for newline character, 
            if (currentLine.length() == 0) {     // if line is blank it means its the end of the client HTTP request
              client.println("<!DOCTYPE html>"); // open wrap the web page
              client.print("<html><head><meta name='viewport' content='initial-scale=1.0'><meta charset='utf-8'><style>#map {height: 100%;}html, body {height: 100%;margin: 0;padding: 0;}</style></head>");
              client.print("<body><h1>ESP32 WebServer Stepper Motor's</h1>");
          // Gui buttons start here
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonBlueLong.png'onmousedown=location.href='/dec1' >");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/nixiesmall1.png'>");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonOrangeLong.png'onmousedown=location.href='/inc1' >");
                     
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonPinkLong.png'onmousedown=location.href='/dec2' >");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/nixiesmall2.png'>");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonGreenLong.png'onmousedown=location.href='/inc2' >"); 
  
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonTurqLong.png'onmousedown=location.href='/dec3' >");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/nixiesmall3.png'>");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonPurpleLong.png'onmousedown=location.href='/inc3' >"); 
                  
                  client.print("<input type=image style=width:100%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonRedLong.png'onmousedown=location.href='/stopall' >");
                  client.print("</body></html>"); // close wrap the web page
              
              client.println(); // The HTTP response ends with an extra blank line:
            
              break;  // break out of the while loop:
            } else {    // if you got a newline, then clear currentLine:
              currentLine = "";
            }
           } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;       // add it to the end of the currentLine
          }
  
          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("GET /dec1")) {digitalWrite(16, HIGH);
                for (int i=0; i <= 43; i++){ digitalWrite(17, HIGH);delay(10);digitalWrite(17,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /inc1")) {digitalWrite(16, LOW);
                for (int i=0; i <= 43; i++){ digitalWrite(17, HIGH);delay(10);digitalWrite(17,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /dec2")) {digitalWrite(16, HIGH);
                for (int i=0; i <= 43; i++){ digitalWrite(18, HIGH);delay(10);digitalWrite(18,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /inc2")) {digitalWrite(16, LOW);
                for (int i=0; i <= 43; i++){ digitalWrite(18, HIGH);delay(10);digitalWrite(18,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /dec3")) {digitalWrite(16, HIGH);
                for (int i=0; i <= 43; i++){ digitalWrite(19, HIGH);delay(10);digitalWrite(19,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /inc3")) {digitalWrite(16, LOW);
                for (int i=0; i <= 43; i++){ digitalWrite(19, HIGH);delay(10);digitalWrite(19,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /stopall")) {digitalWrite(16, HIGH);
            }
          }
        }
      }
  }
  
};
