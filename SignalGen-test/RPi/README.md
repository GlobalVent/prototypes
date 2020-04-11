-TXT files are from a UK test run that was on the G drive, all numbers in CMH2O (just a few diff formats of the same thing,
  test.txt is the Psys channel). Data were originally sampled at 1kHz but downsampled to 200 Hz. fs = 200 Hz unless otherwise noted.

Code:
Python files:
-csvtest.py - simple test to read in data and output using a time.sleep() (this is the recommended workflow pretty much everywhere..drifts
 were the worst with this method)
-csvtest_timingFix1.py - Switched to using perf_counter() which is supposed to give much higher precision/reliability than time.sleep(), 
 made an improvement but still not good enough

C files:
-hpwm_fromFile.c - Implementation in C to read in 1 txt file and output on GPIO18 (hardware PWM line). 
  -Added some extra debug code in here in the main "output+gpiodelay" loop to output, commented out for this commit
  -gpioDelay() was recommended here and delays for "at least the arg time in us", but is supposed to give good precision.
  -Using the average of the times in the debug TXT file we can come up with an offset to subtract from the delay time we pass in 
  to help correct for some of the latency (not currently done)
