import numpy as np
import time
import serial
import sys

import pyaudio

CHUNK = 2**10
RATE = 44100

# argument 1 to the script is the sound signal threshold 
# argument 2 is the COM port to which arduino is connected

p=pyaudio.PyAudio()
stream=p.open(format=pyaudio.paInt16,channels=1,rate=RATE,input=True,
              frames_per_buffer=CHUNK)

MAX_PEAK = 30000
n = len(sys.argv)
port = "COM6"
print(n)
print(sys.argv)
print(port)

if ( n > 2) :
	MAX_PEAK=int(sys.argv[1])
	port=str(sys.argv[2])

# open serial port
ser = serial.Serial( port, 9600, timeout=0.1)

time.sleep(2);


while ( 1 ) :
    data = np.fromstring(stream.read(CHUNK),dtype=np.int16)
    peak=np.average(np.abs(data))*10
    if(peak > MAX_PEAK) :
        peak = MAX_PEAK;
    code=int(peak*9/MAX_PEAK);
    print(code)
    ser.flush()
    ser.write(str(code).encode()); 
stream.stop_stream()
stream.close()
p.terminate()