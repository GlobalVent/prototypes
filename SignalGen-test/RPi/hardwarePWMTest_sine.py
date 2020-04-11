#!/usr/bin/env python

# hardwarePWMTest_sine.py
# 2016-10-06
# Public Domain

# Sine wave test -- frequency looks a little off through an LPF with fc = 241 Hz
# Need to debug (most likely not only from doing the "calculations" in the loop, I would hope)

import pigpio
import math
import time

# Note that freq is device dependent -- 
# 250M/steps (375M/steps for the BCM2711)

# brightness = (math.sin(counter * math.pi * 1.5) / 2) + 0.5

fs = float(200) # This will determine "inter-sample" spacing (5 ms for fs_eq 200 Hz)
Ts = 1/fs

F = float(10) # Waveform frequency
f = F/fs # Normalized by fs
N = 1/f # Number of samples per cycle


n = 0
pi=pigpio.pi()

# Loop to generate periodic waveform
try:
	while 1:    #infinite loop
		if n < N: #if the variable 'count' is less than 100
			n = n + 1    #add one to the variable 'count'
			y = (math.sin(n * 2*math.pi*f) / 2) + 0.5
			pi.hardware_PWM(18, 50000, 1e6*y) # 50,000Hz 50% dutycycle on pin 18   #change the duty cycle to value of 'count'		
			time.sleep(Ts)   #wait for our delay time

		if n == N:    #if count is equal to 100, reset count to 0
			time.sleep(Ts)
			n = 0
			pi.hardware_PWM(18, 50000, 1e6*y) # 50,000Hz 50% dutycycle on pin 18		
			time.sleep(Ts)


except KeyboardInterrupt:   #if Ctrl + C is pressed, cleanly exit our program
	pi.hardware_PWM(18,0,0)
	pi.stop()
	print("Program Exited")