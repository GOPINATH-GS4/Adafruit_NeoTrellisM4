#include <Audio.h>

// WAV files converted to code by wav2sketch
#include "AudioSampleSnare.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "AudioSampleTomtom.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "AudioSampleHihat.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "AudioSampleKick.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
#include "AudioSampleGong.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "AudioSampleCashregister.h" // http://www.freesound.org/people/kiddpark/sounds/201159/

#include "Adafruit_NeoTrellisM4.h"
#include <Adafruit_NeoPixel_ZeroDMA.h>

#define NEO_PIN 10
#define NUM_KEYS 32

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioPlayMemory    sound0;
AudioPlayMemory    sound1;  // six memory players, so we can play
AudioPlayMemory    sound2;  // all six sounds simultaneously
AudioPlayMemory    sound3;
AudioPlayMemory    sound4;
AudioPlayMemory    sound5;
AudioMixer4        mix1;    // two 4-channel mixers are needed in
AudioMixer4        mix2;    // tandem to combine 6 audio sources
AudioOutputAnalogStereo  audioOut;

// Create Audio connections between the components
//
AudioConnection c1(sound0, 0, mix1, 0);
AudioConnection c2(sound1, 0, mix1, 1);
AudioConnection c3(sound2, 0, mix1, 2);
AudioConnection c4(sound3, 0, mix1, 3);
AudioConnection c5(mix1, 0, mix2, 0);   // output of mix1 into 1st input on mix2
AudioConnection c6(sound4, 0, mix2, 1);
AudioConnection c7(sound5, 0, mix2, 2);
AudioConnection c8(mix2, 0, audioOut, 0);
AudioConnection c9(mix2, 0, audioOut, 1);

Adafruit_NeoPixel_ZeroDMA strip(NUM_KEYS, NEO_PIN, NEO_GRB);

void setup() {

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(255);

  trellisKeypad.begin();

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // by default the Teensy 3.1 DAC uses 3.3Vp-p output
  // if your 3.3V power has noise, switching to the
  // internal 1.2V reference can give you a clean signal
  //dac.analogReference(INTERNAL);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mix1.gain(0, 0.4);
  mix1.gain(1, 0.4);
  mix1.gain(2, 0.4);
  mix1.gain(3, 0.4);
  mix2.gain(1, 0.4);
  mix2.gain(2, 0.4);
}

void loop() {

  tick_trellis();
  
  strip.clear();
  while(trellisKeypad.available())
  {
    keypadEvent e = trellisKeypad.read();
    int keyindex = e.bit.KEY - 1;
    if(e.bit.EVENT == KEY_JUST_PRESSED){
      strip.setPixelColor(keyindex, 0x0000FF);
      switch(e.bit.KEY){
        case(1):
          sound0.play(AudioSampleSnare);
          break;
        case(2):
          sound1.play(AudioSampleTomtom);
          break;
        case(3):
          sound2.play(AudioSampleHihat);
          break;
        case(4):
          sound3.play(AudioSampleKick);
          break;
        case(5):
          sound4.play(AudioSampleGong);
          break;
        case(6):
          sound5.play(AudioSampleCashregister);
          break;
        default:
          break;
      }
    }
  }
  strip.show();
  delay(10);
}
