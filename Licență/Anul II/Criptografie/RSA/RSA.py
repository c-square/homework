import sys, os
import random
import math
from fractions import gcd


primeMin = 179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216
primeMax = 359538626972463181545861038157804946723595395788461314546860162315465351611001926265416954644815072042240227759742786715317579537628833244985694861278948248755535786849730970552604439202492188238906165904170011537676301364684925762947826221081654474326701021369172596479894491876959432609670712659248448274432
minE = 10000000000000000000000000000000000000000000000000000000000000



""" HELPER FUNCTIONS """
def totien(p, q):
	return (p-1)*(q-1)

def extended_gcd(a,b):
	prevx, x = 1, 0
	prevy, y = 0, 1
	while b:
		q = a/b
		x, prevx = prevx - q*x, x
		y, prevy = prevy - q*y, y
		a, b = b, a % b
	return a, prevx, prevy


def multi_inverse(a, m):
	g, x, y = extended_gcd(a, m)
	if g != 1:
		return None  # modular inverse does not exist
	else:
		return x % m
	

def helpMe():
	print "Help:"
	print "-dTCR file privateKeyPath // decripts the file using the privateKey"
	print "-a // try simulated attack"
	print "-d file privateKeyPath // decripts the file using the privateKey"
	print "-e file publicKeyPath // encryption"
	print "-g // generate public and private keys into files"
 

'''
   Miller-Rabin 

	Verify if a number is a prime number
'''
def decompose(n):
  	exponentOfTwo = 0
 
   	while n % 2 == 0:
      		n = n/2
      	exponentOfTwo += 1
 
   	return exponentOfTwo, n
 
def isWitness(possibleWitness, p, exponent, remainder):
   	possibleWitness = pow(possibleWitness, remainder, p)
 
   	if possibleWitness == 1 or possibleWitness == p - 1:
      		return False
 
   	for _ in range(exponent):
      		possibleWitness = pow(possibleWitness, 2, p)
 
      	if possibleWitness == p - 1:
         	return False
 
   	return True
 
def probablyPrime(p, accuracy=100):
   	if p == 2 or p == 3: return True
   	if p < 2: return False
 
   	numTries = 0
   	exponent, remainder = decompose(p - 1)
 
   	for _ in range(accuracy):
      		possibleWitness = random.randint(2, p - 2)
      	if isWitness(possibleWitness, p, exponent, remainder):
         	return False
 
   	return True
'''
   Miller-Rabin end
'''





""" RSA IMPLEMENTATION """

def criteriu(l, d, e, n):
	if (l==0):
		 return False
		 
	if (l*n-e*d+l+1)%l != 0:
		 return False
		 
	aux = (l*n-e*d+l+1)/l
	delt = aux*aux-4*n
	
	if (delt<0):
		return False
	if (math.sqrt(delt) * math.sqrt(delt) != delt):
		return False
	if ( (aux + math.sqrt(delt))%2 != 0 or (aux - math.sqrt(delt))%2 != 0):
		return False
		
	if (delt != 0):
		x1 = (aux + math.sqrt(delt))/2
		y = aux-x1
		print " x="+str(x1)+"\n y="+str(y)+"\n d="+str(d)+"\n\n"
		
		if (x1*y == n):
			return True
		x2 = (aux - math.sqrt(delt))/2
		y = aux-x2
		print " x="+str(x2)+"\n y="+str(y)+"\n d="+str(d)+"\n\n"
		
		if (x2*y == n):
			return True
	
	if (delt == 0):
		x1 = (aux+math.sqrt(delt))/2
		y = aux-x1
		print " x="+str(x1)+"\n y="+str(y)+"\n d="+str(d)+"\n\n";
		
		if (x1*y == n):
			return True
			
	return False


def tryAttack():

	al = [0,0,0]
	be = [0,0,0]
	
	l1 = 256
	l2 = 60
	
	while True:
		p = random.randint(2, l1)
		while (not probablyPrime(p)):
			p = random.randint(2, l1)
			
		q = random.randint(2, l1)
		while (not probablyPrime(q)) and (q<=p or q >= 2*q):
			q = random.randint(2, l1)
			
		n = p*q
		
		d = random.randint(2, l2)
		while (not probablyPrime(d)):
			d = random.randint(2, l2)
			
		print "Initial d: "+str(d);
		
		fi = totien(p,q)
		e = multi_inverse(d, fi)
		
		if e != None:
			break

	print "Found e : "+str(e);
	
	i = 0;
	a = e;
	b = n;
	
	while True:
		if (b == 0):
			break
		
		i = i+1
		r = a%b
		q = (a-r)/b
		a = b
		b = r
		
		if (i == 1):
			al[0] = q
			be[0] = 1
			
		if (i == 2):
			 al[1] = al[0]*q+1
			 be[1] = q
		if (i > 2):
			al[2] = q*al[1] + al[0]
			al[0] = al[1]
			al[1] = al[2]
			be[2] = q*be[1] + be[0]
			be[0] = be[1]
			be[1] = be[2]
			
		if (i == 1):
			l = al[0]
			d = be[0]
		if (i == 2):
			l = al[1]
			d = be[1]
		if (i > 2):
			l = al[2]
			d = be[2]
		if (criteriu(l,d,e,n) != False):
			break
			
	print "Found d:"+str(d)


def encrypt(targetFile, publicKeyFile):
	target = open(targetFile, "r")
	keyFile = open(publicKeyFile, "r")
	crypted = open("cryptedText.txt", "w")

	n = int(keyFile.readline())
	key = int(keyFile.readline())
	
	text = target.read()
	while not text=="":
		for i in range(len(text)):
			crypted.write(str(pow(ord(text[i]), key, n))+"\n")
		text = target.read()

	target.close()
	keyFile.close()
	crypted.close()


def decrypt(targetFile, privateKeyFile):
	target = open(targetFile, "r")
	keyFile = open(privateKeyFile, "r")
	plain = open("plainText.txt", "w")

	p = int(keyFile.readline())
	q = int(keyFile.readline())
	n = p*q
	key = int(keyFile.readline())
	
	text = target.readline()
	while not text=="":
		plain.write(str(chr(pow(int(text), key, n))))
		text = target.readline()

	target.close()
	keyFile.close()
	plain.close()
	
def TCR(targetFile, privateKeyFile):
	target = open(targetFile, "r")
	keyFile = open(privateKeyFile, "r")
	plain = open("plainText.txt", "w")

	p = int(keyFile.readline())
	q = int(keyFile.readline())
	n = p*q
	d = int(keyFile.readline())

	dp = d%(p-1)
	dq = d%(q-1)
	qinv = multi_inverse(q%p, p)
	
	text = target.readline()
	while not text=="":
		m1 = pow((int(text))%p, dp, p)
		m2 = pow((int(text))%q, dq, q)
		h = (qinv*(m1-m2))%p;
		toText = chr(m2+h*q)
		plain.write(str(toText))
		text = target.readline()

	target.close()
	keyFile.close()
	plain.close()

	

def generateKeys():	
	
	print "Generating"
	
        p = random.randint(primeMin+2, primeMax-2)
	while not (probablyPrime(p)):
	        p = random.randint(primeMin+2, primeMax-2)

	print "Found first prime"
	
	q = random.randint(primeMin+2, primeMax-2)
	while (not q==p)and(not probablyPrime(q)):
		q = random.randint(primeMin+2, primeMax-2)

	print "Found second prime"

	n = p*q
	totienN = totien(p,q)

	e = random.randint(1, totienN)
	while (gcd(e,totienN)!=1):
		e = random.randint(1, totienN)
	
	d = multi_inverse(e, totienN)

	
	publicFile = open("public.key", "w")	
	publicFile.write(str(n)+"\n")
	publicFile.write(str(e))
	publicFile.close()

	privateFile = open("private.key", "w")
	privateFile.write(str(p)+"\n")
	privateFile.write(str(q)+"\n")
	privateFile.write(str(d))
	privateFile.close()
	
	


if (len(sys.argv)<=1):
	helpMe()
else:
	if (sys.argv[1] == "-d"):
		decrypt(sys.argv[2], sys.argv[3])
	elif (sys.argv[1] == "-a"):
		tryAttack()
	elif (sys.argv[1] == "-dTCR"):
		TCR(sys.argv[2], sys.argv[3])
	elif (sys.argv[1] == "-e"):
		encrypt(sys.argv[2], sys.argv[3])
	elif (sys.argv[1] == "-g"):
		generateKeys()
	else:
		helpMe()
	