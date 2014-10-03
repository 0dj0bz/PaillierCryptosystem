#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>


//#define DEBUG 0

//Private Keys
#define KEY1 5
#define KEY2 7



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
	g=141;

	return ( (modExp(g, m, nsqr) * modExp(r, n, nsqr)) % nsqr );

}

int main (int argc, char **argv)
{

	printf  ("PLAINTEXT: %ld   CIPHERTEXT: %lld \n", 
		     atol(argv[1]), encryptMsg (atol(argv[1]), atol(argv[2]))
			);

	return 0;

}