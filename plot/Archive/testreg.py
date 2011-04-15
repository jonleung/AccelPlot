import re
s = 'x=654,y=662,z=761'
mb = re.match(r'x=(?P<x>\d+),y=(?P<y>\d+),z=(?P<z>\d+)', s)
m = re.search(r'x=(?P<x>\d+),y=(?P<y>\d+),z=(?P<z>\d+)',s)
print m.group(1), m.group(2), m.group(3)
