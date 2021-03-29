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

i.e. on my phone right now, I have `192.168.0.229:9292`