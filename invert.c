/*
 * invert.c - calculate the inverse of a large integer in an integer multiplation group
 *
 * 2016-11-01 Steven Wart created this file
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
#include <gmp.h>

void print_usage(char *name) {
  printf("This program will compute the inverse of integer value, in the multiplication group for the modulus provided\n");
  printf("Usage: %s <value> <prime>\n", name);
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

int main(int argc, char **argv) {
  mpz_t value, mod, pminus2, result;
  char *resultString;

  mpz_inits(value, mod, pminus2, result, NULL);
  
  // if three numeric arguments aren't provided
  // print usage and exit
  if (argc != 3) {
    print_usage(argv[0]);
  }

  // parse the inputs
  parse_integer(value, argv[1]);
  parse_integer(mod, argv[2]);

  // 1. for prime p we have a^p = a mod p
  // 2. and if a is not divisible by p, then a^(p-1) = 1 mod p
  // 3. and a * (a^(p-2)) = a^(p-1) = 1
  // 4. or 1 / a mod p = a^(p-2) mod p
  // 5. therefore dividing by a is like multiplying by a^(p-2) mod p
  mpz_sub_ui(pminus2, mod, 2L);           // compute p-2

  /* resultString = mpz_get_str(NULL, 10, pminus2); */
  /* printf("p-2=%s\n", resultString); */
  
  mpz_powm(result, value, pminus2, mod);  // compute a^(p-2) mod p

  // use the current allocation function for the result string (and in base 10)
  resultString = mpz_get_str(NULL, 10, result);
  
  printf("%s\n", resultString);
}
