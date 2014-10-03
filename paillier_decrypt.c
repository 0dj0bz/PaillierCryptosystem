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

int old_main (int argc, char **argv)
{

	// This is a test program to evaluate the performance of the Paillier cryptosystem.

	// Step 1:  Create encrypted values

	long p = KEY1, q = KEY2;
	long n, nsqr, lambda, g, mu, r;
	long em, dm, m1, m2;

	#ifdef DEBUG 
		printf("ARGC: [%d]\n", argc);
	#endif //DEBUG


	if (argc == 3)
	{
		m1 = atol (argv[2]);

		#ifdef DEBUG
			printf("m1: %ld \n", m1);
		#endif //DEBUG

		// 1.1 - compute n = p*q
		n = p*q;
		nsqr = n*n;

		#ifdef DEBUG
			printf ("N   : [%ld]\n", n);
			printf ("NSQR: [%ld]\n", nsqr);
		#endif //debug


		// 1.2 - compute lambda = lcm (p-1, q-1)
		lambda = lcm (p-1, q-1);

		#ifdef DEBUG
			printf ("LAMBDA: [%ld]\n", lambda);
		#endif //DEBUG

		// 1.3 - compute a random integer g such that 0 < g < n^2
		// For testing, let's use n+1
		g = n+1;

		#ifdef DEBUG
			printf ("G: [%ld]\n", g);
		#endif //DEBUG

		// 1.4 - Generate random value r
		//       For testing, lets use 7.
		r = 7;

		#ifdef DEBUG
			printf ("R: [%ld]\n", r);
		#endif //DEBUG

		// 1.5 - Compute Em
		//		em = ((g^m % nsqr) * (r^n % nsqr)) % nsqr 
		//em = ( (g^m1 % nsqr) * (r^n % nsqr) ) % nsqr ;
		em = ( g^m1 * r^n ) % nsqr ;
		#ifdef DEBUG
			printf ("EM: [%ld]\n", em);
		#endif //DEBUG


		// 1.6 - Compute Dm
		long u;
		u =      (( (g^lambda % nsqr ) - 1) / n);
		u = 1/u;
		dm =   ( (( (em^lambda % nsqr) - 1) / n) * u) % n;

		#ifdef DEBUG
			printf ("DM: [%ld]\n", dm);
		#endif //DEBUG

	}
	return 0;
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

	p = KEY1;
	q = KEY2;

	n = p*q;
	nsqr = n*n;
	m1 = atol(argv[1]);
	r1 = atol(argv[2]);

	m2 = 15;
	r2 = 61;

	printf ("m1:  %lld  r1: %lld  m2: %lld  r2: %lld \n", m1, r1, m2, r2);

	c = encryptMsg (m1 , r1);
	printf ("C: [%lld]\n", c);

	long long lambda = lcm (p-1, q-1);
	printf ("LAMBDA: [%lld]\n", lambda);


	long long u1 = modExp (c, lambda, 5929);
	printf ("U1: [%llu]\n", u1);



	long long u2 = modExp (5652, 30, 5929);
	printf ("U2: [%llu]\n", u2);

	c2 = encryptMsg (m2, r2);

	printf ("C2: [%lld]\n", c2);

	c3 = ((c % 5929) * (c2 % 5929)) % 5929;

	printf ("C3: [%lld]\n", c3);


	long long L_ylam = L( modExp(c3, lambda, nsqr), n);
	long long L_glam = L( u2, n);
	mu = modInv(L_glam, 77);

	printf ("L(c3): [%lld] \n", L_ylam);
	printf ("L(u2): [%lld] \n", L_glam);
	printf ("MU:  %lld \n", mu );

	printf ("(L_ylam * mu) mod 77:  %lld \n", (L_ylam * mu)%77);
	//long long 
	//printf ("DK:")


	return 0;

}