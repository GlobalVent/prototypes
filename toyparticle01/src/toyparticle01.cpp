/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "x:/github/prototypes/toyparticle01/src/toyparticle01.ino"

// ------------
// Blink an LED
// ------------

#include <chrono>
/*-------------

We've heavily commented this code for you. If you're a pro, feel free to ignore it.

Comments start with two slashes or are blocked off by a slash and a star.
You can read them, but your device can't.
It's like a secret message just for you.

Every program based on Wiring (programming language used by Arduino, and Particle devices) has two essential parts:
setup - runs once at the beginning of your program
loop - runs continuously over and over

You'll see how we use these in a second.

This program will blink an led on and off every second.
It blinks the D7 LED on your Particle device. If you have an LED wired to D0, it will blink that LED as well.

-------------*/


// First, we're going to make some variables.
// This is our "shorthand" that we'll use throughout the program:

void setup();
double getTimeNow();
void loop();
#line 30 "x:/github/prototypes/toyparticle01/src/toyparticle01.ino"
int led1 = D6; // Instead of writing D0 over and over again, we'll write led1
// You'll need to wire an LED to this one to see it blink.

int led2 = D7; // Instead of writing D7 over and over again, we'll write led2
// This one is the little blue LED on your board. On the Photon it is next to D7.

// Having declared these variables, let's move on to the setup function.
// The setup function is a standard part of any microcontroller program.
// It runs only once when the device boots up or is reset.

void setup() {
    Serial.begin(9600);

    // We are going to tell our device that D0 and D7 (which we named led1 and led2 respectively) are going to be output
    // (That means that we will be sending voltage to them, rather than monitoring voltage that comes from them)

    // It's important you do this here, inside the setup() function rather than outside it or in the loop function.

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);

}

// Next we have the loop function, the other essential part of a microcontroller program.
// This routine gets repeated over and over, as quickly as possible and as many times as possible, after the setup function is called.
// Note: Code that blocks for too long (like more than 5 seconds), can make weird things happen (like dropping the network connection).  The built-in delay function shown below safely interleaves required background activity, so arbitrarily long delays can safely be done if you need them.
double getTimeNow() {
    #if PARTICLE==1
        // millis rolls over after 49 days, so handle the roll over...
        static unsigned long lastMillis = 0;
        static unsigned long long totalTime = 0;
        unsigned long long deltaTime;
        unsigned long currMillis = millis();
        if (currMillis >= lastMillis)    
            deltaTime = currMillis-lastMillis;
        else   // we rolled over...
            deltaTime = ((unsigned long long)currMillis+0x100000000ULL)-lastMillis;
        lastMillis=currMillis;      // remember this.
        totalTime+= deltaTime;
        return(1e-3*totalTime);
    #else

        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = std::chrono::time_point<Clock>;
        // using Ms = std::chrono::milliseconds;
        // using Ns = std::chrono::nanoseconds;

        TimePoint point = Clock::now();
        const Clock::duration since_epoch = point.time_since_epoch();
        double ns = std::chrono::duration<double>(since_epoch).count();
        return(ns);
    #endif

}

static unsigned cnt =0;
void loop() {
    unsigned dly=(((cnt++ % 4)+1) * 500);

    double ns = getTimeNow();
    Serial.printf("ns = %g\n",ns);


    // To blink the LED, first we'll turn it on...
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);

    // We'll leave it on for 1 second...
    //delay(1000);
    delay(dly);

    // Then we'll turn it off...
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);


    // Wait 1 second...
    delay(dly);

    // And repeat!
}