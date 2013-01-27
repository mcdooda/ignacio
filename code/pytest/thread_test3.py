import pychos

def run_test():
	pychos.run_single_test('thread_test3', 50, check_test)

def check_test(line, nb):
	assert nb['a'] == 50
	assert nb['b'] == 50
	assert nb['c'] == 50
	assert nb['d'] == 2000
	assert nb['Q'] == 4
	assert nb['Z'] == 4
