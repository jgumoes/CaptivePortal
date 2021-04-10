# Test Portal
to run the test portal:
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