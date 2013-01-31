import pychos

def run_test():
	for i in range(50):
		pychos.run_single_test('file_test3', 1, lambda x, y: None)
		pychos.run_single_test('cat', 1, check_test)

def check_test(line, nb):
	assert nb['a'] == 100, line
	assert nb['b'] == 100, line
	assert nb['c'] == 100, line
	assert nb['d'] == 100, line
