#include <Arduino.h>
#include <libraries/DFPlayer_Mini_Mp3_by_Makuna/src/DFMiniMp3.h>
#include <libraries/OneButton/src/OneButton.h>
#include <SoftwareSerial.h>

#define button1Pin A0
#define button2Pin A1
#define button3Pin A2
#define button4Pin A3
#define button5Pin A4

// Setup a some new OneButton
OneButton button1(button1Pin, true);
OneButton button2(button2Pin, true);
OneButton button3(button3Pin, true);
OneButton button4(button4Pin, true);
OneButton button5(button5Pin, true);

//Stores the current playing folder and track
uint8_t currentFolder = 1;
uint8_t currentTrack = 1;

//method definitions
void nextTrack();
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
class Mp3Notify {
public:
    static void OnError(uint16_t errorCode) {
        // see DfMp3_Error for code meaning
        Serial.println();
        Serial.print("Com Error ");
        Serial.println(errorCode);
    }
    static void OnPlayFinished(uint16_t track) {
        Serial.print("Track beendet");
        Serial.println(track);
        delay(100);
        nextTrack();
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

    Serial.println(F("TonUINO Version 2.0"));
    Serial.println(F("(c) Thorsten Voß"));

    // link the button functions.
    button1.attachClick(changeFolder1);
    button1.attachDoubleClick(increaseVolume);
    button2.attachClick(changeFolder2);
    button2.attachDoubleClick(decreaseVolume);
    button3.attachClick(changeFolder3);
    button4.attachClick(changeFolder4);
    button5.attachClick(changeFolder5);

    // Busy Pin
    pinMode(4, INPUT);

    // DFPlayer Mini initialisieren
    mp3.begin();
    mp3.setVolume(15);
}

void nextTrack() {
    uint16_t numTracksInFolder = mp3.getFolderTrackCount(currentFolder);
    uint16_t oldTrack = currentTrack;
    currentTrack = random(1, numTracksInFolder + 1);
    if (currentTrack == oldTrack)
        currentTrack = currentTrack == numTracksInFolder ? 1 : currentTrack+1;
    Serial.print(F("Party Modus ist aktiv -> zufälligen Track spielen: "));
    Serial.println(currentTrack);
    mp3.playFolderTrack(currentFolder, currentTrack);
}

void changeFolder(uint16_t folder) {
    currentFolder=folder;
    nextTrack();
}

void changeFolder1() {
    Serial.println("Change to Folder 1");
    changeFolder(1);
}

void changeFolder2() {
    Serial.println("Change to Folder 2");
    changeFolder(2);
}

void changeFolder3() {
    Serial.println("Change to Folder 3");
    changeFolder(3);
}

void changeFolder4() {
    Serial.println("Change to Folder 4");
    changeFolder(4);
}

void changeFolder5() {
    Serial.println("Change to Folder 5");
    changeFolder(5);
}

void increaseVolume() {
    Serial.println("IncreaseVolume Volume");
    mp3.increaseVolume();
}

void decreaseVolume() {
    Serial.println("DecreaseVolume Volume");
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