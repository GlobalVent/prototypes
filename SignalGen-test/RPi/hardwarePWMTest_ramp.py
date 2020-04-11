#!/usr/bin/env python

# hardwarePWMTest_ramp.py
# 2016-10-06
# Public Domain

# Ramp generator test -- this should give a triangle wave w Twave = T*N = 0.005*100 = 
# 500 ms (right?), but looks a little off through an LPF with fc = 241 Hz

import pigpio
import math
import time

# Note that freq is device dependent -- 
# 250M/steps (375M/steps for the BCM2711)

# brightness = (math.sin(counter * math.pi * 1.5) / 2) + 0.5

count = 0
T = 0.005 # This will determine "inter-sample" spacing (5 ms for fs_eq 200 Hz)
pi=pigpio.pi()

# Loop to generate periodic waveform
try:
	while 1:    #infinite loop
		if count < 100: #if the variable 'count' is less than 100
			count = count + 1    #add one to the variable 'count'
			
			pi.hardware_PWM(18, 50000, 1e6*count/100) # 50,000Hz 50% dutycycle on pin 18   #change the duty cycle to value of 'count'
			#delay(T)			
			time.sleep(T)   #wait for our delay time

		if count == 100:    #if count is equal to 100, reset count to 0
			time.sleep(T)
			count = 0
			pi.hardware_PWM(18, 50000, 1e6*count/100) # 50,000Hz 50% dutycycle on pin 18
			#delay(1*10)			
			time.sleep(T)


except KeyboardInterrupt:   #if Ctrl + C is pressed, cleanly exit our program
	pi.hardware_PWM(18,0,0)
	pi.stop()
	#GPIO.cleanup()
	print("Program Exited")


# AJO TO-DO -- add any more init code, any cleanup code at end of file?