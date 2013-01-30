import os, commands, subprocess, sys, random

os.chdir('build')

nachos_prog = './nachos-userprog'
auto_gdb = False
num_test_fails = 0
full = False

def run_single_test(test, n, f):
	show_title(test)
	try:
		for i in range(n):
			seed = random.randint(1, 500)
			cmd = '%s -rs %3d -x %s' % (nachos_prog, seed, test)
			sys.stdout.write('%3d %s' % (i + 1, cmd))
			sys.stdout.flush()
			status, output = commands.getstatusoutput(cmd)
			line = output.split('\n')[0]
			assert status == 0
			nb = char_count(line)
			f(line, nb)
			print('\t\t[OK]')
		print('\n')
	except AssertionError:
		global num_test_fails
		num_test_fails += 1
		print('\n---------------------------------------------------------------------')
		print(output)
		print('---------------------------------------------------------------------')
		print('\n')
			
		if auto_gdb:
			subprocess.call('gdb -ex "run -rs %d -x %s" %s' % (seed, test, nachos_prog) , shell = True)
			
		if not full:
			raise
		

def show_title(title):
	print('==================== %s ====================' % title.upper())
	
def char_count(line):
	nb = {}
	for j in range(len(line)):
		c = line[j]
		if not c in nb:
			nb[c] = 0
			
		nb[c] += 1
		
	return nb
	
def get_tests():
	modules = sys.modules.values()
	tests = [(module.__name__, module) for module in modules if 'run_test' in dir(module)]
	tests.sort()
	tests = [module for module_name, module in tests]
	return tests
	
def test_all():
	global num_test_fails
	num_test_fails = 0
	for test in get_tests():
		test.run_test()
		
	if num_test_fails > 0:
		print('%d tests failed' % num_test_fails)
	
def test(t):
	if t in sys.modules:
		sys.modules[t].run_test()
		
	else:
		print('Available tests:')
		for test in get_tests():
			print('- '+test.__name__)

def step5():
	global nachos_prog
	nachos_prog = './nachos-step5'
	commands.getstatusoutput('./nachos-step5 -f')
	commands.getstatusoutput('./nachos-step5 -cpfs')
