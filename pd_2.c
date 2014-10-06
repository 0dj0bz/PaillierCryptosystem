#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>


//#define DEBUG 0

//Private Keys
#define KEY1 7
#define KEY2 11



long gcd(long a, long b) 
{
  if (b == 0) 
  {
    return a;
  }
  else 
  {
    return gcd(b, a % b);
  }
}

long lcm (long p1, long  p2)
{
	return (p1*p2)/gcd(p1, p2);
}

long long power(long base, long exp)
{
    long long result = 1;
    while(exp) { result *= base; exp--; }
    return result;
}



long long modExp (long base, long ex, long modulus)
{
	long long accum = 1L, p_mod_it = 0L;
	for (int i=0; i<64; i++)
	{
		if (i==0)
			p_mod_it = ((i==0)?base:(p_mod_it)) % modulus;
		else
			p_mod_it = ((i==0)?base:(p_mod_it*p_mod_it)) % modulus;

		if (((ex>>i)&0x1) == 1)
			accum *= p_mod_it;

		#ifdef DEBUG		
		printf ("ITER:%2d EXP>>I&0x1: %ld P_MOD_IT: %lld  ACCUM: %lld\n", i, (ex>>i)&0x1, p_mod_it, accum);
		#endif //DEBUG

	}

	return accum%modulus;
}

long long encryptMsg (long long m, long long r)
{
	long p = KEY1, q = KEY2;
	long n, nsqr, g;

	// 1.1 - compute n = p*q
	n = p*q;
	nsqr = n*n;
	g=5652;

	return ( (modExp(g, m, nsqr) * modExp(r, n, nsqr)) % nsqr );

}

long long L( long long u, long n)
{

	return ( (u-1) / n);
}

// decryptMsg
// c - ciphertext
//long long decryptMsg (long long c, long long n)
//{
    

//}

long long modInv(long a, long b)
{
	long b0 = b, t, q;
	long x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}

int main (int argc, char **argv)
{
	long long p, q;
	long long n, nsqr;
	long long c, c2, c3; //ciphertext
	long long mu;
	long long m1, m2;
	long long r1, r2;
	long long g = 5652;
	long long gm2;
	long long em1_gm2;


	p = KEY1;
	q = KEY2;

	n = p*q;
	nsqr = n*n;
	m1 = atol(argv[1]);
	r1 = atol(argv[2]);

	m2 = 15;
	r2 = 61;

	printf("Input message: [%lld] Random r1: [%lld]\n", m1, r1);


	//printf ("m1:  %lld  r1: %lld  m2: %lld  r2: %lld \n", m1, r1, m2, r2);
	printf ("Private key 1: %lld  Private key 2: %lld\n", p, q);

	printf ("n(k1*k2): %lld  nsqr: %lld\n", n, nsqr);
	c = encryptMsg (m1 , r1);
	printf ("Encrypted input message: [%lld]\n", c);

	long long lambda = lcm (p-1, q-1);
	//printf ("LAMBDA: [%lld]\n", lambda);


	long long u1 = modExp (c, lambda, 5929);
	//printf ("U1: [%llu]\n", u1);

	printf("Message provided by other party: [%lld] Random R2: [%lld]\n", m2, r2);

	gm2 = modExp (g, m2, nsqr);
	printf("Other party message encoded with public keys (g, n): [%lld]\n", gm2);

	em1_gm2 = (c % nsqr) * (gm2 % nsqr) % nsqr;
	printf ("Other party message multiplied against original encrypted message: [%lld]\n", em1_gm2);

	long long u2 = modExp (g, lambda, nsqr);
	// printf ("U2: [%llu]\n", u2);

	// So for this test, let's simply make c2 = u2
	/*
	c2 = u2;
	

	printf ("C2: [%lld]\n", c2);
	*/

	long long L_ylam = L( modExp(em1_gm2, lambda, nsqr), n);
	long long L_glam = L( u2, n);
	mu = modInv(L_glam, 77);

	printf ("L(ylam): [%lld] \n", L_ylam);
	printf ("L(glam): [%lld] \n", L_glam);
	printf ("MU:  %lld \n", mu );

	printf ("Decrypted message from other party:  %lld \n", (L_ylam * mu)%77);
	printf ("Original message sent:               %lld \n", m1);
	printf ("New message - original message: [%lld]\n", ((L_ylam * mu)%77)-m1);
	//long long 
	//printf ("DK:")


	return 0;

}