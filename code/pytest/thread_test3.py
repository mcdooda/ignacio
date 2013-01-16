import os, commands, sys

def exec_test(semence, t):
	cmd = './build/nachos-userprog -rs %d -x build/%s' % (semence, t)
	sys.stdout.write(cmd)
	return commands.getstatusoutput(cmd)

def show_title(title):
	print('~=-=-=-=-=-=-=-=-=-= %s =-=-=-=-=-=-=-=-=-=~' % title)

def run_test():
	show_title('THREAD_TEST3')
	for i in range(100):
		status, output = exec_test(i + 1, 'thread_test3')
		line = output.split('\n')[0]
		
		assert status == 0
		
		nb = {}
		for j in range(len(line)):
			c = line[j]
			if not c in nb:
				nb[c] = 0
				
			nb[c] += 1
			
		assert nb['a'] == 50
		assert nb['b'] == 50
		assert nb['c'] == 50
		assert nb['d'] == 2000
		assert nb['Q'] == 4
		assert nb['Z'] == 4
		
		print(' \t[OK]')
		
