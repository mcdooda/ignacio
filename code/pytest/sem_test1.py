import pychos, time

a = ''
b = ''
c = ''
d = ''
for i in range(1000):
	a += 'a'
for i in range(1000):
	c += 'c'
for i in range(1000):
	d += 'd'
for i in range(1000):
	b += 'b'

def run_test():
	pychos.run_single_test('test_sem1', 50, check_test)
	
def check_test(line, nb):
	assert a in line
	assert b in line
	assert c in line
	assert d in line
