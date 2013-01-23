import pychos

def run_test():
	pychos.run_single_test('thread_test3', 50, check_test)

def check_test(line, nb):
	assert nb['a'] == 50, line
	assert nb['b'] == 50, line
	assert nb['c'] == 50, line
	assert nb['d'] == 2000, line
	assert nb['Q'] == 4, line
	assert nb['Z'] == 4, line