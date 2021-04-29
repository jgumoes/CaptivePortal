# Test Portal
this portal uses a fetch request to get the network ssids. it's probably not necessary, because the ESPAsyncWebServer docs say the loops should be possible in the templates, but it's nice to know that it works for the alarm clock.
## running the test server
to run the test server:
```sh
cd "ruby server test"
rackup -o 0.0.0.0
```
find your computers ip address (mine is 192.168.0.229)

on your phone, go to `{ip}:9292`

i.e. on my phone right now, I have `http://192.168.0.229:9292`

# Operation

## stuff to look in to

 * [Respond with content coming from a File containing templates](https://github.com/me-no-dev/ESPAsyncWebServer#respond-with-content-coming-from-a-file-containing-templates)
 * [chunked response](https://github.com/me-no-dev/ESPAsyncWebServer#chunked-response)
 * [Param Rewrite With Matching](https://github.com/me-no-dev/ESPAsyncWebServer#param-rewrite-with-matching)
 * [Respond with content using a callback containing templates](https://github.com/me-no-dev/ESPAsyncWebServer#respond-with-content-using-a-callback-containing-templates)
 * [Chunked Response containing templates](https://github.com/me-no-dev/ESPAsyncWebServer#chunked-response-containing-templates)

## Static or Dynamic?

There are two ways to fill out the server flags on the config page:
  1. fill them out server-side
  2. fill them out dynamically using javascript functions

### 1. static
it might be possible to fill in the webpage on the server side while reading from program memory. the section in the README of  ESPAsyncWebServer called [Specifying Template Processor callback](https://github.com/me-no-dev/ESPAsyncWebServer#specifying-template-processor-callback) shows a way to serve a static webpage from a file containing templates, that uses a function specifically optimised for SPIFFS (though I'm not sure that it won't work with littleFS).

### 2. dynamic

This would be javascript functions filling out the flags on the client side. The flags would either be served by putting them in the url as params, or having a javascript function that will make a get request to the server, then extract the flags from the response object. This should only be done if I can't crack the static webpage.

## Network List

I would like if the webpage updated the wifi networks every 10 or so seconds. To do this, the webpage should make periodic GET requests to the server, that will then serve the wifi networks. A JS function will fill out the page elements.

To keep the list current, the wifiscan should be performed every 10 seconds, and should only wipe the old network list after the scan is complete (incase the client requests mid-scan).

## Server Flags

If a user submits a wrong password, the wrong_password flag should be raised, and a message should be displayed on the screen informing the user of their erronous assertion. In this instance, the page should redirect to a url containing `wrong_pwd=true` in the url. The when the page loads, it will scan the url for params, it adjust the page content accordingly.

## Diagrams

```
# esp should start network scan immediately
# if it can't connect to a network, and should
# rescan every 30 seconds until it is connected
esp->esp: "starts the network scan"
client->esp: "get '/config'"
esp->esp: "starts the network scan"
esp->client: "serves config.html,
config.css, config.js"
client->esp: "post '/network_list'"
esp->client: "responds with 'not ready'"
client->esp: "post '/network_list'"
esp->client: "serves networks.json"
```

### Saving Credentials Flow

Below is the flowchart for saving a POSTed ssid and password. There could eventually be a check for if the wifi ssid has already been saved, so if the user enters a wrong (or no) password for a given network, it will try to connect using the stored password instead. This gives the user freedom to select a prefered network (i.e. if they have multiple routers at home, and the device has connected to the one with poor connection). This could of course be reduntant by having the device pick strongest availlable network stored, and re-assessing networks if the connection drops. I think it still could be nice to have though.
```https://playground.diagram.codes/d/flowchart
"WiFi details are POSTed"
IF "do they work?"
 "save new credentials"
ELSE
 IF "ssid is stored
 in memory?"
 "use stored password"
  IF "does it work?"
   "save credentials"
  ELSE
   "send 'bad password' message"
  END IF
 ELSE
    "send 'bad password' message"
 END IF
END IF
```