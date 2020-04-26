#include <MIDI.h>
#include "Timer.h"
#include <QueueArray.h>

Timer t;

MIDI_CREATE_DEFAULT_INSTANCE();

// -----------------------------------------------------------------------------

// This function will be automatically called when a NoteOn is received.
// It must be a void-returning function with the correct parameters,
// see documentation here:
// http://arduinomidilib.fortyseveneffects.com/a00022.html

int notes[] = {22, 38, 24, 37, 26, 29, 44, 31, 45, 33, 32, 47, 34, 41, 36, 43, 28, 30, 40, 27, 39, 25, 35, 46, 23};
QueueArray <int> queue;

int base = 67;

void note_off()
{
  digitalWrite(queue.dequeue(), LOW);
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    // Do whatever you want when a note is pressed.

    // Try to keep your callbacks short (no delays ect)
    // otherwise it would slow down the loop() and have a bad impact
    // on real-time performance.
    if ((pitch >= base) && (pitch < base + 25)){
      digitalWrite(notes[pitch-base], HIGH);   // turn the LED on (HIGH is the voltage level)
      queue.enqueue ( notes[pitch-base]);
      t.after(20, note_off);
    }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    //digitalWrite(notes[pitch-base], LOW);
    // Do something when the note is released.
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
}

// -----------------------------------------------------------------------------

void setup()
{
    // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.

    int i;
    for(i = 0; i < 25; i ++){
      pinMode(notes[i], OUTPUT); 
    }     

//    for(i = 0; i < 25; i ++){
//      digitalWrite(notes[i], HIGH);
//      delay(25);
//      digitalWrite(notes[i], LOW);
//      delay(1000); 
//    }  
    
    MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

    // Do the same for NoteOffs
    MIDI.setHandleNoteOff(handleNoteOff);

    // Initiate MIDI communications, listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);

    Serial.begin(115200);
}

void loop()
{
    // Call MIDI.read the fastest you can for real-time performance.
    MIDI.read();
    t.update();

    // There is no need to check if there are messages incoming
    // if they are bound to a Callback function.
    // The attached method will be called automatically
    // when the corresponding message has been received.
}
