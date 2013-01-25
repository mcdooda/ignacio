import pychos, time

l = ''
for i in range(1000):
	l += 'a'
for i in range(1000):
	l += 'b'
for i in range(1000):
	l += 'c'
for i in range(1000):
	l += 'd'

def run_test():
	pychos.run_single_test('test_sem1', 50, check_test)
	
def check_test(line, nb):
	line == l