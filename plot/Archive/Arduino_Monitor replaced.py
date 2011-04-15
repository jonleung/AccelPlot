# Listen to serial, return most recent numeric values
# Lots of help from here:
# http://stackoverflow.com/questions/1093598/pyserial-how-to-read-last-line-sent-from-serial-device


from threading import Thread
import time, serial, re

last_received = ''
def receiving(ser):
	global last_received
	buffer = ''
	while True:
		buffer = buffer + ser.read(ser.inWaiting())
		if '\n' in buffer:
			lines = buffer.split('\n')
			last_received = lines[-2]
			buffer = lines[-1]

class SerialData(object):
	def __init__(self, init=50):
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
			self.ser = None
		else:
			Thread(target=receiving, args=(self.ser,)).start()
	
	def next(self):
		if not self.ser:
			return 100
		for i in range(40):
			raw_line = last_received
			print raw_line
			m = re.match(r'x=(?P<x>...\d+),y=(?P<y>...\d+),z=(?P<z>...\d+)', raw_line)
			if m:
				print m.group('x'), m.group('y'), m.group('z')
		return 0
	
	def __del__(self):
		        if self.ser:
		            self.ser.close()

if __name__=='__main__':
    s = SerialData()
    for i in range(500):
        time.sleep(.015)
        print s.next()