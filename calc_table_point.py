
def mod_pow(base, exp, modulus):
	c = 1
	base = base % modulus
	for i in range(0, exp):
		c = ( c * base ) % modulus
	return c

expr = []
log = [0] * 257
c = 1
for i in range(0, 256):#	print 'expr[',i,'] = ',c
	y = c
	if (c == 256): y = 0
	log[y] = i
	print y,',',
	c = ( c * 45 ) % 257

print "\n\n"

for i in range(0, 256):
	print log[i],',',
