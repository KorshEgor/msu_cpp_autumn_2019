import subprocess

def check(expr, result):
	out = subprocess.run(['./calc', expr], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	if str(out.stdout.decode("utf-8")) != result + '\r\n' and str(out.stdout.decode("utf-8")) != result + '\n' and \
		str(out.stderr.decode("utf-8")) != result + '\r\n' and str(out.stderr.decode("utf-8")) != result + '\n':
		print('error {0} != {1}, got {2} and err {3}'.format(expr, result, out.stdout, out.stderr))

check('2 + 2', '4')
check('3 + 3', '6')
check('324*20/9', '720')
check('  -18 +-  20 ', '-38')
check('8 / 5*0', '0')
check(' 52--4', '56')
check(' 52---4', '48')
check('355*-30', '-10650')
check('-18/4', '-4')
check('', 'Error: Wait for number')
check('     ', 'Error: Wait for number')
check('   4 - 3*  ', 'Error: Wait for number')
check('   4 ** 3  ', 'Error: Wait for number')
check('   4 */ 3  ', 'Error: Wait for number')
check('   4 - 3*  -', 'Error: Wait for number')
check('   4  3*2  ', 'Error: Wait for operation')
check('4*0/0*0', 'Error: Division by 0')
check('8 / +3', 'Error: Unary + is\'t supported')
check('  121 + 12a', 'Error: Wrong latter')
check(' 23179+  1325*- 46/- -7786/2*--39--178  ', '23240')
check('-2147483648/-1', '-2147483648')