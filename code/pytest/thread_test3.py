import pychos

def run_test():
	pychos.show_title('THREAD_TEST3')
	for i in range(10):
		status, output = pychos.exec_test('thread_test3')
		line = output.split('\n')[0]
		
		assert status == 0
		
		nb = pychos.char_count(line)
		
		assert nb['a'] == 50, line
		assert nb['b'] == 50, line
		assert nb['c'] == 50, line
		assert nb['d'] == 2000, line
		assert nb['Q'] == 4, line
		assert nb['Z'] == 4, line
		
		print(' \t[OK]')
