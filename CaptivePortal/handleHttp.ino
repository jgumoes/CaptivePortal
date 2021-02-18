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

boolean name_change = false;

void handleRoot() {
  Serial.println("handleRoot()");
  if (captivePortal()) { // If captive portal redirect instead of displaying the page.
    return;
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page = String(F("<!DOCTYPE html>"
"<html>"
  "<head>"
    
  "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<title>WiFi Config</title>"
  "<style>"
  "body {margin: 3%;background-color: lightblue;}"
  ".h {font-family: Helvetica;text-align: center;margin-bottom: -20px;}"
  ".p {font-family: Helvetica;}"
  ".wrong {margin-left: 10%;font-family: Helvetica;color:red;font-size: 20px;}"
  ".radios {display: block;position: relative;padding-left: 35px;"	/*padding for the radio button labels*/
  "margin-bottom: 12px;"	/* vertical padding between buttons */
  "margin-left: 5px;"	/* left padding */
  "cursor: pointer;font-family: Helvetica;font-size: 22px;}"
  ".radios input {position: absolute;opacity: 0;cursor: pointer;}"	/* Hide the browser's default radio button */
  ".checkmark {position: absolute;top: 0;left: 0;height: 25px;width: 25px;background-color: #eee;border-radius: 50%; }"	/* Create a custom radio button */
  ".radios:hover input ~ .checkmark {background-color: #ccc;}"	/* On mouse-over, add a grey background color */
  ".radios input:checked ~ .checkmark {background-color: #2196F3;}"	/* When the radio button is checked, add a blue background */
  ".checkmark:after {content: '';position: absolute;display: none;}"	/* Create the indicator (the dot/circle - hidden when not checked) */
  ".radios input:checked ~ .checkmark:after {display: block;}"	/* Show the indicator (dot/circle) when checked */
  ".radios .checkmark:after {top: 9px;left: 9px;width: 8px;height: 8px;border-radius: 50%;background: white;}"	/* Style the indicator (dot/circle) */
  ".connect {letter-spacing: 0.1em;font-size: 15px;margin-left: 12.5%;background-color: #4CAF50;border: none;color: white;padding: 13px 32px;text-decoration: none;text-align: center;width: 75%;cursor: pointer;}"
  /*Advanced CSS*/
  ".h3 {font-family: Helvetica;}"
  "input[type='text'] {width: 80%;margin-left: 10%;margin-top: 10px;}"
  ".SubName {margin-top: 10px;}"
  ".NameChange {letter-spacing: 0.1em;font-size: 15px;color: #259b29;font-family: Helvetica;margin-top: -5px;margin-bottom: -10px;}"
"</style>"
"</head>"
"<body>"

"<h1 class='h'>WiFi Config</h1>"
"<br>"
"<p class='p'>"));
  /* device name / device name change */
  if(name_change == false){
    Page += String(F("This device is called "));
    Page += String(myHostname);
    Page += String(F("<br>"));
  }
  else{
    Page += String(F("<b class='NameChange'>Device name changed to <em>"));
    Page += String(myHostname);
    Page += String(F("</em></b>"));
    name_change = false;
  }

  /* connection status */
  Page += String(F("You are "));
  if(status != 3){
    Page += String(F("not connected to a network"));
  }
  else{
    Page += String(F("connected to ")) + String(ssid);
  }

  Page += F(
    "</p>"
    "<form action='/wifisave'>"
      "<h3 class='h'>Available Networks:</h3>"
      "<h4 class='h'>(refresh if any are missing)</h4>"
    "<br><br>");

  // Serial.println("scan start");
  n_networks = WiFi.scanNetworks();  //TODO: add option to scan for hidden networks
  // Serial.println("scan done");
  if (n_networks > 0) {
    for (int i = 0; i < n_networks; i++) {
      // add the wifi ssids to the datalist.
      // should I rank SSIDs by signal strength? (yes if there's space)
      // should I show signal strength (optional in the advanced settings)?
      Page += String(F("<label class='radios'>")) + WiFi.SSID(i);
      Page += String(F("<input type='radio' name='radio' value='")) + WiFi.SSID(i) + String(F("'>"));
      Page += String(F("<span class='checkmark'></span>"));
      Page += String(F("</label>"));
    }
  } else {
    Page += String(F("<label class='radios'>No Networks Found"));
    Page += String(F("<input type='radio' name='n' value=''>"));
    Page += String(F("<span class='checkmark'></span>"));
    Page += String(F("</label>"));
  }
  Page += String(F(
    "<br>"
    "<div class='password'>"
      "<h3 class='h' for='pwd'>Enter Password:</h3><br>"));

  if(wrong_pass == true){
    Page += String(F("<label for='pwd' class=wrong>Wrong password</label><br>"));
  }

  Page += String(F("<input type='text' id='pwd' name='pwd'>"
    "</div>"
    "<br>"
    "<input type='submit' class='connect' value='Connect'>"
  "</form>"
  "<form action='/change_name'>"
  "<hr/>"
  "<p class='h3'>Change device name from <b>"));
  Page += String(myHostname);
  Page += String(F("</b>?</p>"
  "<label for='dev_name' class='p'>Enter new device name below (100 characters maximum):</label><br>"
  "<input type='text' id='dev_name' name='dev_name' maxlength='100' placeholder='Enter new device name'>"
  "<br>"
  "<input type='submit' class='SubName' value='Save New Name'>"
"</form>"
"<br><br>"

"</body>"
"</html>"));
  
  server.send(200, "text/html", Page);
  server.client().stop(); // Stop is needed because we sent no content length
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    Serial.println("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
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
