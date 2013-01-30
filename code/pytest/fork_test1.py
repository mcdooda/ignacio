import pychos

def run_test():
	pychos.run_single_test('forkexec', 500, check_test)

def check_test(line, nb):
	assert nb['a'] == 16, line
	assert nb['b'] == 16, line
	assert nb['c'] == 1, line
	assert nb['x'] == 1, line
	assert nb['y'] == 1, line
	assert nb['z'] == 1, line
	assert nb['A'] == 6, line
	assert nb['B'] == 6, line
