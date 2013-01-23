import pychos, time

def run_test():
	pychos.run_single_test('thread_test4', 50, check_test)
	
def check_test(line, nb):
	assert nb['a'] == 3, line
	assert nb['b'] == 3, line
	assert nb['c'] == 3, line