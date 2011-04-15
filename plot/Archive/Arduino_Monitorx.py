"""
Listen to serial, return most recent numeric values
Lots of help from here:
http://stackoverflow.com/questions/1093598/pyserial-how-to-read-last-line-sent-from-serial-device
"""
from threading import Thread
import time
import serial, re, sys

last_received = ''
def receiving(ser):
	global last_received
	buffer = ''
	while True:
		buffer = buffer + ser.read(ser.inWaiting())
		if '\n' in buffer:
			lines = buffer.split('\n') # Guaranteed to have at least 2 entries
			last_received = lines[-2]
			#If the Arduino sends lots of empty lines, you'll lose the
			#last filled line, so you could make the above statement conditional
			#like so: if lines[-2]: last_received = lines[-2]
			buffer = lines[-1]


class SerialData(object):
	def __init__(self, init=20):
		try:
			self.ser = ser = serial.Serial(
				port='/dev/tty.usbserial-000013FDA',
				baudrate=115200,
				bytesize=serial.EIGHTBITS,
				parity=serial.PARITY_NONE,
				stopbits=serial.STOPBITS_ONE,
				timeout=0.1,
				xonxoff=0,
				rtscts=0,
				interCharTimeout=None
			)
		except serial.serialutil.SerialException:
			#no serial connection
			self.ser = None
		else:
			Thread(target=receiving, args=(self.ser,)).start()
		
	def next(self):
		if not self.ser:
			return float(20) #return anything so we can test when Arduino isn't connected
		#return a float value or try a few times until we get one
		for i in range(5):
			raw_line = last_received
			#print str(raw_line.strip())

			m = re.search(r'x=(?P<x>0.\d+),y=(?P<y>0.\d+),z=(?P<z>0.\d+)',str(raw_line.strip()))
			n = re.search(r'x=', str(raw_line.strip()))
			try:
				#print m.group(1), m.group(2), m.group(3)
				return float(m.group(1))
			except:
				print 'bogus data',raw_line
				time.sleep(.005)
				
			return 30
	def __del__(self):
		if self.ser:
			self.ser.close()

if __name__=='__main__':
	s = SerialData()
	for i in range(1000):
		time.sleep(.03)
		print s.next()
