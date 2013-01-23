import os, commands, sys, random

def exec_test(t):
	cmd = './build/nachos-userprog -rs %d -x build/%s' % (random.randint(1, 500), t)
	sys.stdout.write(cmd)
	return commands.getstatusoutput(cmd)
	
def run_single_test(t, n, f):
	show_title(t)
	for i in range(n):
		status, output = exec_test(t)
		line = output.split('\n')[0]
		assert status == 0
		nb = char_count(line)
		f(line, nb)
		print(' \t[OK]')

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
	
def test_all():
	modules = sys.modules.values()
	tests = [module for module in modules if 'run_test' in dir(module)]
	tests.sort()
	for test in tests:
		test.run_test()
	
def test(t):
	sys.modules[t].run_test()