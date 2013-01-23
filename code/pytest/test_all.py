#!/usr/bin/env python
import sys
sys.path.append('pytest')
import pychos

# ajouter un import pour chaque test
import thread_test3
import thread_test4
import thread_test5
#import thread_test6

if len(sys.argv) > 1:
	pychos.test(sys.argv[1])
else:
	pychos.test_all()
