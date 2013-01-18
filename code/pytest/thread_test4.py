import pychos, time

def run_test():
	pychos.show_title('THREAD_TEST4')
	for i in range(50):
		status, output = pychos.exec_test('thread_test4')
		line = output.split('\n')[0]
		
		assert status == 0
		
		nb = pychos.char_count(line)
		
		assert nb['a'] == 3, line
		assert nb['b'] == 3, line
		assert nb['c'] == 3, line
		
		print(' \t[OK]')