// Micro Shawarma
// July 2022
// Roni Bandini @RoniBandini
// Buenos Aires, Argentina
// MIT License

#include <WiFiNINA.h>

char ssid[] = "Micro Shawarma";         
char pass[] = "1234567890";    
int keyIndex = 0;                

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

int relayPin1=6; 
int relayPin2=7;  

int wheel=0;
int heat=0;

String header;

void wheelOn(){
    digitalWrite(relayPin1, LOW);    
    Serial.println("Wheel on");
    wheel=1;  
}


void heatOn(){
    digitalWrite(relayPin2, LOW);
    Serial.println("Heat on");
    heat=1;        
}

void machineOff(){
    digitalWrite(relayPin1, HIGH);  
    digitalWrite(relayPin2, HIGH);        
    wheel=0;
    heat=0;
    Serial.println("Machine off");            
}

void setup() {
  
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);   
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);  
  
  Serial.begin(9600);
  
  Serial.println("Micro Shawarma");
  Serial.println("Roni Bandini - Argentina");
  Serial.println("Starting server");

  pinMode(led, OUTPUT);       

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  Serial.print("Creating AP");  

  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}

void loop() {
  // compare the previous status to the current status
  if(status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if(status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

   WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             
    Serial.println("new client");            
    String currentLine = "";                 
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                   
        if (c == '\n') {                    


          if (currentLine.length() == 0) {

            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Cairo; display: inline; margin: 0px auto; text-align: center; background-color: #FFFFFF;}");
            client.println(".button { background-color: #00FF00; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 35px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #FF0000;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Micro Shawarma</h1>");            
            
            if (wheel==0){
                client.println("<p><a href=\"/WHEEL/on\"><button class=\"button\">Wheel ON</button></a></p>");              
              }
            if (heat==0){
                client.println("<p><a href=\"/HEAT/on\"><button class=\"button\">Heat ON</button></a></p>");  
              }

            if (wheel==1 or heat==1){
                client.println("<p><a href=\"/ALL/off\"><button class=\"button button2\">OFF</button></a></p>");              
              }
            
            client.println("<i>Made in Argentina - Roni Bandini - July 2022</i>");
            client.println("</body></html>");
                       
            client.println();
            // break out of the while loop:
            break;
          }
          else {      
            currentLine = "";
          }
        }
        else if (c != '\r') {     
          currentLine += c;       
        }

        
         if (currentLine.endsWith("GET /WHEEL/on")) {
              Serial.println("Wheel on");              
              wheelOn();
            } 

             if (currentLine.endsWith("GET /HEAT/on")) {
              Serial.println("Heat on");              
              heatOn();
            }  

            if (currentLine.endsWith("GET /ALL/off")) {
              Serial.println("Machine off");              
              machineOff();
            }             
        
      }
    }
    
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWiFiStatus() {
 
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

 
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
