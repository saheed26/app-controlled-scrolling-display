#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include  <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 16 // No of dot matrix display
#define CLK_PIN   D5 // or SCK
#define DATA_PIN  D7 // or MOSI
#define CS_PIN    D8 // or SS

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char  *ap_name = "AkoredeAP"; //  The name  of  the Wi-Fi network that  will  be  created
const char  *ap_pass = "ak12345";     //  The password  required  to  connect to  it, leave blank for an  open  network

char msg[500] = { "SCROLLING DISPLAY SYSTEM" };

const char PROGMEM indexPage[] = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <style> @media only screen and (max-width: 600px) {.wrapper{width: 90%;}} @media only screen and (min-width: 600px) {.wrapper{width: 70%;}} @media only screen and (min-width: 992px) {.wrapper{width: 30%;}}</style><title>Scrolling Display</title></head><body style=\"background: #227279; padding: 1em 0px;font-size:18px;\"><div style=\"margin: .5em auto; padding: 0.6em 1em; border: 5px solid skyblue;border-radius: 20px;box-shadow: 5px 5px 5px black; color: white;\"><h2 style=\"text-align:center;\">Scrolling Display</h2><form method=\"post\" action=\"/sent\" id=\"scrollingF\"> <center><textarea name=\"message\" placeholder=\"Type your message here..\" rows=\"6\" cols=\"30\" maxlength=\"500\" style=\"font-size: 17px; width: 95%; border-radius: 10px;\"></textarea></center><br>   <div><center><input type=\"submit\" value=\"Send\" style=\"background: skyblue; padding: 1em; color: black; border-color:skyblue; border-radius: 7px; font-weight:bold;\"></center></div><br> </form>  </center><hr><p>Display power</p><center><a href=\"/poweron\" style=\"background: green; padding: .6em; color: white; border-color:#1A82C3; border-radius: 7px; text-decoration: none; border: 1px solid black;\">Power On</a><a href=\"/poweroff\" style=\"background: red; padding: .6em; color: white; border-color:#1A82C3; border-radius: 7px; margin-left: 10px; text-decoration: none;  border: 1px solid black;\">Power Off</a><center><br> </div><br></body></html>";

void  handleRoot()  {                                                   //  When  URI / is  requested,  send  the homepage
server.send(200,  "text/html", indexPage);
}

void  handleLED() {      //  If  a POST  request is  made  to  URI /sent
  server.send(200, "text/html",  "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <style> @media only screen and (max-width: 600px) {.wrapper{width: 90%;}} @media only screen and (min-width: 600px) {.wrapper{width: 70%;}} @media only screen and (min-width: 992px) {.wrapper{width: 30%;}}</style><title>Message sent</title></head><body style=\"background: #227279; padding: 30px 0px;\"><div style=\"margin: 1em auto; padding: 1em; border: 5px solid skyblue;border-radius: 20px;box-shadow: 5px 5px 5px black; color: white;\"><center><h4>Message sent successfully</h4><br> <a href=\"/\" id=\"w\" style=\"background: skyblue; padding: .8em; margin-bottom: 1em; color: black; border-color:#1A82C3; border-radius: 7px;\">Go to homepage</a> </center></div></body></html>");
  
  String message = server.arg("message");
 // Serial.println(message);
 
  //Formating display Text
  int arraySz = message.length();
  arraySz += 1;
  char displayMsg[arraySz];
  message.toCharArray(displayMsg, arraySz);
  //Saving into array
  strcpy(msg, displayMsg);
  P.displayClear();
  P.displayText(msg, PA_CENTER, 25, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

//Power up the display
  void  handlePowerOn()  {        
    P.displayShutdown(false);
    P.displaySuspend(false);
    server.send(200,  "text/html", "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <style> @media only screen and (max-width: 600px) {.wrapper{width: 90%;}} @media only screen and (min-width: 600px) {.wrapper{width: 70%;}} @media only screen and (min-width: 992px) {.wrapper{width: 30%;}}</style><title>Display powered on</title></head><body style=\"background: #227279; padding: 30px 0px;\"><div style=\"margin-right: auto; margin-left: auto;\"><center><h4>Display powered on</h4><br> <a href=\"/\" style=\"background: #1A82C3; padding: .7em; color: white; border-color:#1A82C3; border-radius: 4px;\">Go to homepage</a> </center></div></body></html>");
}

//Power down the dispaly
 void  handlePowerOff()  {  
    P.displayShutdown(true);
    P.displaySuspend(true);
    server.send(200,  "text/html", "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <style> @media only screen and (max-width: 600px) {.wrapper{width: 90%;}} @media only screen and (min-width: 600px) {.wrapper{width: 70%;}} @media only screen and (min-width: 992px) {.wrapper{width: 30%;}}</style><title>Display powered down</title></head><body style=\"background: #227279; padding: 30px 0px;\"><div style=\"margin-right: auto; margin-left: auto;\"><center><h4>Display powered down</h4><br> <a href=\"/\" style=\"background: #1A82C3; padding: .7em; color: white; border-color:#1A82C3; border-radius: 4px;\">Go to homepage</a> </center></div></body></html>");
}

void  handleNotFound(){
  server.send(200,  "text/html", "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <style> @media only screen and (max-width: 600px) {.wrapper{width: 90%;}} @media only screen and (min-width: 600px) {.wrapper{width: 70%;}} @media only screen and (min-width: 992px) {.wrapper{width: 30%;}}</style><title>Page not found</title></head><body style=\"background: #227279; padding: 30px 0px;\"><div style=\"margin: 1em auto; padding: 1em; border: 5px solid skyblue;border-radius: 20px;box-shadow: 5px 5px 5px black; color: white;\"><center><h4>Page NOT found!</h4><br> <a href=\"/\" style=\"background: skyblue; padding: .8em; margin-bottom: 1em; color: red; border-color:#1A82C3; border-radius: 7px;\">Go to homepage</a> </center></div></body></html>");
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println();

  P.begin();
  P.setInvert(false);
  P.setIntensity(4);
  P.displayShutdown(true);
  delay(50);
  P.displayShutdown(false);
  P.displayClear();
  P.displaySuspend(false);
  P.displayText(msg, PA_CENTER, 25, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  WiFi.disconnect();
  WiFi.softAP(ap_name, ap_pass);

  //Start Web server
  server.on("/",  HTTP_GET, handleRoot);  //  Call  the 'handleRoot'  function  when  a client  requests  URI "/"
  server.on("/sent", HTTP_POST,  handleLED);
  server.on("/poweron",  HTTP_GET, handlePowerOn);
  server.on("/poweroff",  HTTP_GET, handlePowerOff);
  server.onNotFound(handleNotFound);     //  When  a client  requests  an  unknown URI (i.e. something other than  "/"), call  function "handleNotFound"
  server.begin();
}

void loop() {
  yield();
  server.handleClient();
  if (P.displayAnimate()){
    P.displayClear();
    P.displayText(msg, PA_CENTER, 25, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
}
