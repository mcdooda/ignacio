import pychos

def run_test():
	pychos.run_single_test('test_lr_sem', 50, check_test)

def check_test(line, nb):
	assert 'X' not in line
