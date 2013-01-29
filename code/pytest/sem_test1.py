import pychos, time

l1 = ''
for i in range(1000):
	l1 += 'a'
for i in range(1000):
	l1 += 'c'
for i in range(1000):
	l1 += 'd'
for i in range(1000):
	l1 += 'b'

l2 = ''
for i in range(1000):
	l2 += 'a'
for i in range(1000):
	l2 += 'd'
for i in range(1000):
	l2 += 'c'
for i in range(1000):
	l2 += 'b'

l3 = ''
for i in range(1000):
	l3 += 'a'
for i in range(1000):
	l3 += 'b'
for i in range(1000):
	l3 += 'd'
for i in range(1000):
	l3 += 'c'

l4 = ''
for i in range(1000):
	l4 += 'a'
for i in range(1000):
	l4 += 'd'
for i in range(1000):
	l4 += 'b'
for i in range(1000):
	l4 += 'c'
	
l5 = ''
for i in range(1000):
	l5 += 'a'
for i in range(1000):
	l5 += 'b'
for i in range(1000):
	l5 += 'c'
for i in range(1000):
	l5 += 'd'
	
l6 = ''
for i in range(1000):
	l6 += 'a'
for i in range(1000):
	l6 += 'c'
for i in range(1000):
	l6 += 'b'
for i in range(1000):
	l6 += 'd'
	
l7 = ''
for i in range(1000):
	l7 += 'b'
for i in range(1000):
	l7 += 'd'
for i in range(1000):
	l7 += 'c'
for i in range(1000):
	l7 += 'a'

l8 = ''
for i in range(1000):
	l8 += 'b'
for i in range(1000):
	l8 += 'c'
for i in range(1000):
	l8 += 'd'
for i in range(1000):
	l8 += 'a'

l9 = ''
for i in range(1000):
	l9 += 'b'
for i in range(1000):
	l9 += 'd'
for i in range(1000):
	l9 += 'a'
for i in range(1000):
	l9 += 'c'

l10 = ''
for i in range(1000):
	l10 += 'b'
for i in range(1000):
	l10 += 'a'
for i in range(1000):
	l10 += 'd'
for i in range(1000):
	l10 += 'c'

l11 = ''
for i in range(1000):
	l11 += 'b'
for i in range(1000):
	l11 += 'c'
for i in range(1000):
	l11 += 'a'
for i in range(1000):
	l11 += 'd'

l12 = ''
for i in range(1000):
	l12 += 'b'
for i in range(1000):
	l12 += 'a'
for i in range(1000):
	l12 += 'c'
for i in range(1000):
	l12 += 'd'

l13 = ''
for i in range(1000):
	l13 += 'c'
for i in range(1000):
	l13 += 'b'
for i in range(1000):
	l13 += 'd'
for i in range(1000):
	l13 += 'a'

l14 = ''
for i in range(1000):
	l14 += 'c'
for i in range(1000):
	l14 += 'd'
for i in range(1000):
	l14 += 'b'
for i in range(1000):
	l14 += 'a'

l15 = ''
for i in range(1000):
	l15 += 'c'
for i in range(1000):
	l15 += 'a'
for i in range(1000):
	l15 += 'd'
for i in range(1000):
	l15 += 'b'

l16 = ''
for i in range(1000):
	l16 += 'c'
for i in range(1000):
	l16 += 'd'
for i in range(1000):
	l16 += 'a'
for i in range(1000):
	l16 += 'b'

l17 = ''
for i in range(1000):
	l17 += 'c'
for i in range(1000):
	l17 += 'a'
for i in range(1000):
	l17 += 'b'
for i in range(1000):
	l17 += 'd'

l18 = ''
for i in range(1000):
	l18 += 'c'
for i in range(1000):
	l18 += 'b'
for i in range(1000):
	l18 += 'a'
for i in range(1000):
	l18 += 'd'

l19 = ''
for i in range(1000):
	l19 += 'd'
for i in range(1000):
	l19 += 'b'
for i in range(1000):
	l19 += 'c'
for i in range(1000):
	l19 += 'a'

l20 = ''
for i in range(1000):
	l20 += 'd'
for i in range(1000):
	l20 += 'c'
for i in range(1000):
	l20 += 'b'
for i in range(1000):
	l20 += 'a'

l21 = ''
for i in range(1000):
	l21 += 'd'
for i in range(1000):
	l21 += 'a'
for i in range(1000):
	l21 += 'c'
for i in range(1000):
	l21 += 'b'

l22 = ''
for i in range(1000):
	l22 += 'd'
for i in range(1000):
	l22 += 'c'
for i in range(1000):
	l22 += 'a'
for i in range(1000):
	l22 += 'b'

l23 = ''
for i in range(1000):
	l23 += 'd'
for i in range(1000):
	l23 += 'a'
for i in range(1000):
	l23 += 'b'
for i in range(1000):
	l23 += 'c'

l24 = ''
for i in range(1000):
	l24 += 'd'
for i in range(1000):
	l24 += 'b'
for i in range(1000):
	l24 += 'a'
for i in range(1000):
	l24 += 'c'

def run_test():
	pychos.run_single_test('test_sem1', 50, check_test)
	
def check_test(line, nb):
	assert line == l1 or line == l2 or line == l3 or line == l4 or line == l5 or line == l6 or line == l7 or line == l8 or line == l9 or line == l10 or line == l11 or line == l12 or line == l13 or line == l14 or line == l15 or line == l16 or line == l17 or line == l18 or line == l19 or line == l20 or line == l21 or line == l22 or line == l23 or line == l24
