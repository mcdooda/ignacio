#!/usr/bin/env python
import sys
sys.path.append('pytest')
import pychos

# ajouter un import pour chaque test
import thread_test3
import thread_test4
import thread_test5
import fork_test1
#import thread_test6

import sem_test1

pychos.nachos_prog = './nachos-userprog'

test = None
i = 0
for arg in sys.argv:
	if i > 0:
		if arg == '-step5':
			pychos.nachos_prog = './nachos-step5'
			
		elif arg == '-gdb':
			pychos.auto_gdb = True
			
		elif arg == '-full':
			pychos.full = True
			
		else:
			test = arg
	
	i += 1

if test is None:
	pychos.test_all()
else:
	pychos.test(test)
