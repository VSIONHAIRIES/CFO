#define MIDI_CHANNEL 1

#include <spi4teensy3.h>
#include <EEPROM.h>
#include <uCFO.h>

// sequence ID
int s1, s2;
// sequence step index
int indx1 = 0;
int indx2 = 0;
const int nbr_notes1 = 16;
const int nbr_steps2 = 8;
const int notes1[] = {12, 24, 7, 12, 36, 12, 24, 15, 0, 12, 48, 36, 19, 24, 3, 36};
const int midiCC[] =     {8,  8, 11, 21, 21, 11, 21, 31, 8};
const int midiValue[] =  {0, 80, 68, 68, 68, 72, 72, 72, 0};

void setup() {

  // We initialise the sound engine by calling Music.init() which outputs a tone
  Music.init();
  
  Music.enableEnvelope1();
  Music.enableEnvelope2();
  
// These guys just have to be here...
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleControlChange(OnControlChange);
  
//  delay(5000);
  
    // this is the sequencer code
  Sequencer.init(120);

  //Sequencer.newSequence(CALLBACK, SUBDIV);
  // create new sequence and ID (s1)
  s1 = Sequencer.newSequence(&s1cb, NOTE_16);
  s2 = Sequencer.newSequence(&s2cb, NOTE_32);

  // start sequence 1
  Sequencer.startSequence(s1);
  Sequencer.startSequence(s2);
  
// Loading a preset from EEPROM
  Music.getPreset(20);
}

void loop() {
  
// Checking for incoming MIDI to use dashboard
  usbMIDI.read();
  Sequencer.update();
}

// callback function for the step sequencer

void s1cb() {
  Music.noteOn(notes1[indx1++] + 24, 127);
  if(indx1 >= nbr_notes1) indx1 = 0;
}

void s2cb() {
  Midi.controller(MIDI_CHANNEL - 1, midiCC[indx2], midiValue[indx2]);
  indx2++;
  if(indx2 >= nbr_steps2) indx2 = 0;
}
