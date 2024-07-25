#include <Arduino.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <Wire.h>
#include <LCD_I2C.h>
#include <ESPmDNS.h>

LCD_I2C lcd(0x27, 20, 4);

// Domain <mdns_name>.local
const char* mdns_name = "-------";
// WiFi SSID
char* SSID = "-------";
// WiFi Password
const char* PASSWORD = "-------";
// Spotify Dev app client id
const char* CLIENT_ID = "-------";
// Spotify Dev app client secret
const char* CLIENT_SECRET = "-------";

Spotify sp(CLIENT_ID, CLIENT_SECRET);

void connect_to_wifi(){
    WiFi.begin(SSID, PASSWORD);
    Serial.print("Connecting to WiFi...");
    lcd.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.printf("\nConnected!\n");
    lcd.clear();
    lcd.print("Connected!");

}

void setup() {
    lcd.begin();
    lcd.backlight();
    Serial.begin(115200);
    connect_to_wifi();
    delay(1000);

    if(!MDNS.begin(mdns_name))
      Serial.println("Error setting up MDNS responder!");
    else
      Serial.printf("mDNS responder started: %s\n", mdns_name);

    lcd.clear();
    lcd.print("Please auth.");
    lcd.setCursor(0,1);
    lcd.print("http://carbruh.local/");
    sp.begin();
    while(!sp.is_auth()){
        sp.handle_client();
    }
    Serial.println("Authenticated");
    lcd.clear();
    lcd.print("Spotify connected!");
    delay(1000);
}

void loop() {
    static String lastArtist;
    static String lastTrackname;
    
    String currentArtist = sp.current_artist_names();
    String currentTrackname = sp.current_track_name();
    
    if (lastArtist != currentArtist && currentArtist != "Something went wrong" && !currentArtist.isEmpty()) {
        lastArtist = currentArtist;
        lcd.setCursor(0,0);
        Serial.println("Artist: " + lastArtist);
        lcd.setCursor(0,0);
        lcd.print(lastArtist);


    }
    
    if (lastTrackname != currentTrackname && currentTrackname != "Something went wrong" && currentTrackname != "null") {
        lastTrackname = currentTrackname;
        lcd.setCursor(0,1);
        Serial.println("Track: " + lastTrackname);
        lcd.setCursor(0,1);
        lcd.print(lastTrackname);
    }
}

