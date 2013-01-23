import pychos, time

def run_test():
	pychos.run_single_test('thread_test5', 50, check_test)
			
def check_test(line, nb):
	assert nb['a'] == 1000, line
	assert nb['b'] == 1000, line