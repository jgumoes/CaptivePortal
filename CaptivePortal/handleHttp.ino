/** Handle root or redirect to captive portal */

/*
 * 
 * when connected to AP, handleRoot() is called, and request is redirected to captive portal.
 * when device attempts to "sign in", handleRoot is called, redirected to captive portal,then
 * handleRoot() called again witout redirect.
 * 
 * TODO:
 *  - turn handleRoot() into the wifi config page
 *  - turn SSID list into html Radio Buttons
 *  - add an html checkbox that lets you see the password 
 *  
 *  - add an advanced tab for stuff like: 
 *      * device name
 *      * timeout time
 *      * large font
 *      * forget current network
 *      * show password
 *      * etc.
 */

String css_head = 
"<!DOCTYPE html>"
"<html>"
  "<head>"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<title>WiFi Config</title>"
  "<style>"
  "body {margin: 3%;background-color: lightblue;}"
  ".h {font-family: Helvetica;text-align: center;margin-bottom: -20px;}"
  ".p {font-family: Helvetica;}"
  ".wrong {color: red;font-size: 20px;}"
  ".radios {display: block;position: relative;padding-left: 35px;"/*padding for the radio button labels*/
  "margin-bottom: 12px;"  /* vertical padding between buttons */
  "margin-left: 5px;"   /* left padding */
  "cursor: pointer;font-family: Helvetica;font-size: 22px;}"
  ".radios input {position: absolute;opacity: 0;cursor: pointer;}" /* Hide the browser's default radio button */
  ".checkmark {position: absolute;top: 0;left: 0;height: 25px;width: 25px;background-color: #eee;border-radius: 50%; }"   /* Create a custom radio button */
  ".radios:hover input ~ .checkmark {background-color: #ccc;}" /* On mouse-over, add a grey background color */
  ".radios input:checked ~ .checkmark {background-color: #2196F3;}" /* When the radio button is checked, add a blue background */
  ".checkmark:after {content: '';position: absolute;display: none;}" /* Create the indicator (the dot/circle - hidden when not checked) */
  ".radios input:checked ~ .checkmark:after {display: block;}"  /* Show the indicator (dot/circle) when checked */
  ".radios .checkmark:after {top: 9px;left: 9px;width: 8px;height: 8px;border-radius: 50%;background: white;}"  /* Style the indicator (dot/circle) */
  ".connect {background-color: #4CAF50;border: none;color: white;padding: 16px 32px;text-decoration: none;text-align: center;width: 80%;margin: 4px 2%;cursor: pointer;}"
  /*Advanced CSS*/
  ".h3 {font-family: Helvetica;}"
  ".slider-wrapper input, input[type='text'] {width: 80%;margin-left: 10%;margin-top: 10px;}"
"</style>"
"</head>"
"<body>";

void handleAdvanced() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += css_head;
  Page += advanced_css;
  Page += advanced_body;
  server.send(200, "text/html", Page);
  server.client().stop(); // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  Serial.println("wifi save");
  char test_ssid[33] = "";
  char test_password[65] = "";
  server.arg("n").toCharArray(test_ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(test_password, sizeof(password) - 1);
  if (connectWifi(test_ssid, test_password)){
    server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
    saveCredentials();
  }
  else{
    wrong_pass = true;
  }
  server.sendHeader("Location", "/", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

void handleNotFound() {
  Serial.println("handleNotFound");
  if (captivePortal()) { // If captive portal redirect instead of displaying the error page.
    return;
  }
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++) {
    message += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(404, "text/plain", message);
}
