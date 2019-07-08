//Includes for IntelliJ CLion
#include <Arduino.h>
#include <libraries/DFPlayer_Mini_Mp3_by_Makuna/src/DFMiniMp3.h>
#include <libraries/OneButton/src/OneButton.h>
#include <SoftwareSerial.h>

//Includes for Adroino IDE
/*#include <DFMiniMp3.h>
#include "OneButton.h"
#include <SoftwareSerial.h>*/

#define button1Pin A0
#define button2Pin A1
#define button3Pin A2
#define button4Pin A3
#define button5Pin A4
#define busyPin 4

// Setup a some new OneButton
OneButton button1(button1Pin, true);
OneButton button2(button2Pin, true);
OneButton button3(button3Pin, true);
OneButton button4(button4Pin, true);
OneButton button5(button5Pin, true);

uint8_t currentVolume = 15;
const uint8_t MAX_VOLUME = 22;
const uint8_t MIN_VOLUME = 1;

//Stores the current playing folder and track
uint8_t currentFolder = 0;
uint8_t currentTrack = 1;

//method definitions
void nextTrack(uint8_t folder);
void increaseVolume();
void decreaseVolume();
void changeFolder1();
void changeFolder2();
void changeFolder3();
void changeFolder4();
void changeFolder5();

// DFPlayer Mini
SoftwareSerial mySoftwareSerial(2, 3); // RX, TX

// DFPlayer Mini Callback Methods
unsigned long millisLastFinishCall=0;
class Mp3Notify {
public:
    static void OnError(uint16_t errorCode) {
        // see DfMp3_Error for code meaning
        Serial.println();
        Serial.print("Com Error ");
        Serial.println(errorCode);
    }
    static void OnPlayFinished(uint16_t track) {
        Serial.println(millisLastFinishCall);
        unsigned long currentMillis = millis();
        if(currentMillis - millisLastFinishCall < 1000){
            Serial.println("[OnPlayFinished] Prevent double call");
            return;
        }
        millisLastFinishCall = currentMillis;
        Serial.print("Track beendet ");
        Serial.println(track);
        delay(100);
        nextTrack(currentFolder);
    }
    static void OnCardOnline(uint16_t code) {
        Serial.println(F("SD Karte online "));
    }
    static void OnCardInserted(uint16_t code) {
        Serial.println(F("SD Karte bereit "));
    }
    static void OnCardRemoved(uint16_t code) {
        Serial.println(F("SD Karte entfernt "));
    }
    static void OnUsbOnline(uint16_t code) {
        Serial.println(F("USB online "));
    }
    static void OnUsbInserted(uint16_t code) {
        Serial.println(F("USB bereit "));
    }
    static void OnUsbRemoved(uint16_t code) {
        Serial.println(F("USB entfernt "));
    }
};

//Instance of MP3 Player Module
static DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mySoftwareSerial);

void setup() {

    Serial.begin(115200); // Es gibt ein paar Debug Ausgaben über die serielle
    // Schnittstelle
    randomSeed(analogRead(A0)); // Zufallsgenerator initialisieren

    Serial.println(F("AntonUINO Version 1.0"));
    Serial.println(F("(c) Andre Morgenthal"));

    // link the button functions.
    button1.attachClick(changeFolder1);
    button1.attachDoubleClick(decreaseVolume);
    button2.attachClick(changeFolder2);
    button2.attachDoubleClick(increaseVolume);
    button3.attachClick(changeFolder3);
    button4.attachClick(changeFolder4);
    button5.attachClick(changeFolder5);

    // Busy Pin
    pinMode(busyPin, INPUT);

    // DFPlayer Mini initialisieren
    mp3.begin();
    mp3.setVolume(currentVolume);
}

bool isPlaying() {
    return !digitalRead(busyPin);
}

void nextTrack(uint8_t folder) {

    uint8_t oldFolder = currentFolder;
    currentFolder = folder;
    uint8_t numTracksInFolder = mp3.getFolderTrackCount(currentFolder);
    Serial.print(F("Number Tracks in Folder: "));
    Serial.println(numTracksInFolder);

    if(oldFolder != folder){
        //Start with first song in folder when another button is clicked.
        currentTrack = 1;
    }else{
        currentTrack = currentTrack + 1;
    }

    if (currentTrack > numTracksInFolder){
        currentTrack = 1;
    }

    Serial.print(F("Spiele naechsten Track: "));
    Serial.print(currentTrack);
    Serial.print(F(" aus Ordner: "));
    Serial.println(currentFolder);
    mp3.playFolderTrack(currentFolder, currentTrack);
}

void changeFolder(uint16_t folder) {
    if(isPlaying()){
        Serial.println(F("Pause"));
        mp3.pause();
    }else{
        nextTrack(folder);
    }
}

void changeFolder1() {
    changeFolder(1);
}

void changeFolder2() {
    changeFolder(2);
}

void changeFolder3() {
    changeFolder(3);
}

void changeFolder4() {
    changeFolder(4);
}

void changeFolder5() {
    changeFolder(5);
}

void increaseVolume() {
    if(currentVolume >= MAX_VOLUME){
        Serial.println("Reached Max Volume");
        return;
    }
    currentVolume++;
    Serial.print("IncreaseVolume Volume: ");
    Serial.println(currentVolume);
    mp3.increaseVolume();
}

void decreaseVolume() {
    if(currentVolume <= MIN_VOLUME){
        Serial.println("Reached Min Volume");
        return;
    }
    currentVolume--;
    Serial.print("DecreaseVolume Volume: ");
    Serial.println(currentVolume);
    mp3.decreaseVolume();
}

void loop() {
    mp3.loop();
    button1.tick();
    button2.tick();
    button3.tick();
    button4.tick();
    button5.tick();
}