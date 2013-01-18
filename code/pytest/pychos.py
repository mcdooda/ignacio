import os, commands, sys, random

def exec_test(t):
	cmd = './build/nachos-userprog -rs %d -x build/%s' % (random.randint(1, 500), t)
	sys.stdout.write(cmd)
	return commands.getstatusoutput(cmd)

def show_title(title):
	print('~=-=-=-=-=-=-=-=-=-= %s =-=-=-=-=-=-=-=-=-=~' % title)
	
def char_count(line):
	nb = {}
	for j in range(len(line)):
		c = line[j]
		if not c in nb:
			nb[c] = 0
			
		nb[c] += 1
		
	return nb