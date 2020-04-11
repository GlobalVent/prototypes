import csv
import pigpio
import time

csvname = '20_20_500_12_15_System.txt'
fPWM = 50000 # PWM Frequency
fs = float(300) # This will determine "inter-sample" spacing (5 ms for fs_eq 200 Hz)
Ts = 1/fs

V_HIGH_RPI = 3.0 # 3.3 V when GPIO high -- AJO to-do account for attenuation by RC circuit and fPWM
PWM_VSTEP = 250e6 # According to rasp pi docs PWM_VSTEP/fPWM gives resolution of PWM output, different on BCM2711 (375M)
CMH2O_TO_BAR = 0.000980638 # For pressure sensor data conversions

# Note Python 2.x crashes on "open(csvname, newline='')
with open(csvname, newline='') as csvfile:
    data = list(csv.reader(csvfile, delimiter='\t', quoting=csv.QUOTE_NONNUMERIC))

Pres = [i[0] for i in data] # Column 0, 0-1 BarG - PXM319-001G10V 
Psys = [i[1] for i in data] # Column 1, 0-140 millibarG - PXM319-0.14G10V

# print(len(Pres))
print(Pres[0:4])
print(Psys[0:4])

print("Playing back waveforms")
# First loop - convert Pcmh20 to V, then from [0,10]V to Vpwm_eq (equivalent PWM voltage)
for i in range(len(Psys)):
    # Convert pressure to voltage (assumes linear relationship)
    # Convert pressures to normalized values based on sensor ranges
    Pres[i] = Pres[i] * CMH2O_TO_BAR  # * 10.0 # 0-1 Bar maps from 0 to 10 V
    Psys[i] = Psys[i] * CMH2O_TO_BAR * 1/0.14 # 0 - 140 mBar maps from 0 to 10 V, AJO TODO - functionize conversion taking arrays as inputs

print(Pres[0:4])
print(Psys[0:4])
   
   
pi=pigpio.pi()

# Next loop
for i in range(len(Psys)):
    #Pres[i] = Pres[i]*V_HIGH_RPI/(PWM_VSTEP/fPWM)
    #Psys[i] = Psys[i]*V_HIGH_RPI/(PWM_VSTEP/fPWM)
    #Pres[i] = Pres[i]*V_HIGH_RPI/10.0
    #Psys[i] = Psys[i]*V_HIGH_RPI/10.0
    # Write PWM value to pin
    pi.hardware_PWM(18, fPWM, int(1e6*Psys[i])) # PWM on pin 18
    time.sleep(Ts)   #wait for our delay time -- note this doesn't account for computation time for the other step(s) of the loop

#except KeyboardInterrupt:   #if Ctrl + C is pressed, cleanly exit our program
pi.hardware_PWM(18,0,0)
pi.stop()
print("Program Exited")
    
#print(len(Pres))
#print(Pres[0:4])