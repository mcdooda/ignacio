import pychos, time

def run_test():
	pychos.run_single_test('thread_test6', 50, check_test)
	
def check_test(line, nb):
	