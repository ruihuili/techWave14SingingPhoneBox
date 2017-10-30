//music-----------------------(begin)
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioPlaySdWav           playSdWav1;
AudioOutputI2S           audioOutput;
AudioConnection          patchCord1(playSdWav1, 0, audioOutput, 0);
AudioConnection          patchCord2(playSdWav1, 1, audioOutput, 1);
AudioControlSGTL5000     sgtl5000_1;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
//music-----------------------(end)

const byte motionPin = 2; 
const byte buttonPin = 4;
const byte ledPin = 13;
unsigned long previousMillis = 0; 
const long blinkInterval = 1000; 
boolean ledBlink, ledState, buttonState,buttonLatch,motionState,alarmEnable,detectionLatch,ledLatch;

void setup() {
  Serial.begin(9600); 

  //music-----------------------(begin)
  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
 //music-----------------------(end)
  pinMode(buttonPin,INPUT);
  pinMode(motionPin,INPUT);
  pinMode(ledPin,OUTPUT);

  randomSeed(3);
  
  delay(1000);
}

void loop() {
  buttonState = !digitalRead(buttonPin);
  
  motionState = digitalRead(motionPin);
  
  if(motionState && !detectionLatch && !buttonState){
    Serial.println("Human motion detected");
    detectionLatch = true;
    //Serial.println(detectionLatch);
    ledLatch= true;
    ledBlink = true;   
    Serial.println("LED blink");
    
    if (playSdWav1.isPlaying() == false) {
      int randNumber = random(1, 100); //(min, max) - min inclusive, max exclusive
      const char * fileName;
      if (randNumber%3 == 1){
         fileName = "MML.wav";
      }
      else if (randNumber%3 == 2){           
         fileName = "EMD.wav";
        }
      else{
         fileName = "IML.wav";
        }
        
      playSdWav1.play(fileName);
       
      Serial.print("Start playing ");
      Serial.println(fileName);

      delay(10); 
     }
    }

  if(buttonState == true && !buttonLatch){
    Serial.println("Phone picked up -- User starts interaction with tablet");
    buttonLatch = true;
    if(playSdWav1.isPlaying() == true){
        ledBlink = false;
        digitalWrite(ledPin,HIGH);
        Serial.println("LED on");
        playSdWav1.stop(); 
        Serial.println("Sound off");
    }
  }

    if(!buttonState && buttonLatch && detectionLatch){
    Serial.println("Button pushed -- user leaing");
    buttonLatch = true;
    motionState = false;
    if(playSdWav1.isPlaying() == false && ledLatch==true){
       Serial.println("Wait 3s to turn light off"); 
       Serial.println(">>>>>");
       delay(1000);
       Serial.println(">>>>>"); 
       delay(1000);
       Serial.println(">>>>>");       
       delay(1000);
       
       ledLatch= false;      
       digitalWrite(ledPin,LOW);
       Serial.println("Lights off"); 
       detectionLatch = false;
       //Serial.println(detectionLatch);
    }
    }
    
  if(!buttonState && buttonLatch){
    buttonLatch = false;
  }

  if (ledBlink){
    blink();
  }
    
}

//LED Blink
void blink(){
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= blinkInterval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
     digitalWrite(ledPin,ledState);
  }
}


