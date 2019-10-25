/*
 * discrete-log.c - calculate the discrete logarithm
 *
 * 2016-11-01 Steven Wart created this file
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
#include <gmp.h>
#include <glib.h>

void print_usage(char *name) {
  printf("This program will compute the discrete logarithm of a number x such that <val> = <base>^x mod <mod>\n");
  printf("Usage: %s <val> <base> <mod>\n", name);
  exit(0);
}

//
// Parse an integer value from the string and put it into result
// If the number cannot be parsed, exit the program
//
void parse_integer(mpz_t result, char *stringValue) {
  // parse number using base=0 so we can accept 0x for hexadecimal numbers
  int err = mpz_set_str(result, stringValue, 0);
  if (err < 0) {
    perror("invalid number");
    exit(0);
  }
}

// hash function for putting mpz_t values into a glib hash function
// see: http://www.isthe.com/chongo/tech/comp/fnv/
// we use the 64-bit variant but instead of iterating one byte at
// a time (as you would for strings) we iterate 64-bits at a time
guint (*GHashFunc) dl_mpz_hash(mpz_t key); {
  return 0;
}

// equality function for mpz_t values
// return TRUE if a and b are equal, FALSE otherwise
gboolean(*GEqualFunc) dl_mpz_equal(gconstpointer a, gconstpointer b) {
  mpz_t *op1 = (mpz_t *)a;
  mpz_t *op2 = (mpz_t *)b;
  return mpz_cmp (op1, op2) == 0;
}


// compute the discrete logarithm of g to the base h modulo p
void discrete_log(mpz_t result, mpz_t g, mpz_t h, mpz_t p) {
  GHashTable *lookup;
  unsigned long int B = 2^20;
  mpz_t lhs, rhs;

  // Set g to the greatest common divisor of a and b, and in addition set s and t to coefficients satisfying a*s + b*t = g.
  // This will give us s, the multiplicative inverse of a (needed for subsequent computations)
  // mpz_gcdext (mpz_t g, mpz_t s, mpz_t t, const mpz_t a, const mpz_t b)

  mpz_inits(lhs, rhs, NULL);

  // create a hashtable to lookup the results
  lookup = g_hash_table_new(dl_mpz_hash, dl_mpz_equal);  // XXX use mpz_cmp instead of g_str_equal
  
  // 1. First build a dictionary of all the possible values of the left hand size of the equation h/g^x_1
  for (long int x_1=0; x_1 < B; x_1++) {
    mpz_powm(result, g, x_1, p);  // g^x_1 % p
    mpz_fdiv(lhs, g, exp, mod);   // floor div h / g^x_1
    g_hash_table_insert(lookup, lhs, x_1);
  }
  
  // 2. Then, for each value x_0 = 0, 1, 2, ..., 2^20 check if the right hand side (g^B)^x_0 is in the hash table
  for (long int x_0=0; x_0 < B; x_0++) {
    mpz_pow_ui(result, g, B);      // g^B
    mpz_pow_ui(rhs, result, x_0);  // (g^B)^x_0
    if (g_hash_table_lookup(lookup, rhs)) {
      mpz_mul_ui(result, x_0, B);   // x_0*B
      mpz_add(result, result, x_1); // x_0*B + x_1
      g_hash_table_destroy(lookup);
      mpz_clears(lhs, rhs, result, NULL);
      return;
    }
  }
  g_hash_table_destroy(lookup);
  mpz_clears(lhs, rhs, result, NULL);
}

int main(int argc, char **argv) {
  mpz_t val, base, mod, result;
  char *resultString;

  mpz_inits(val, base, mod, result, NULL);
  
  // if three numeric arguments aren't provided
  // print usage and exit
  if (argc != 4) {
    print_usage(argv[0]);
  }

  // parse the inputs
  parse_integer(val, argv[1]);
  parse_integer(base, argv[2]);
  parse_integer(mod, argv[3]);
  
  discrete_log(result, base, exp, mod);

  // use the current allocation function for the result string (and in base 10)
  resultString = mpz_get_str(NULL, 10, result);
  
  printf("%s\n", resultString);
}
