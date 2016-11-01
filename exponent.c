/*
 * exponent.c - calculate the exponent of a large integer in an integer multiplation group
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
  printf("This program will raise a large integer value to the exponent provided, in the multiplication group modulo the modulus provided\n");
  printf("Usage: %s <exp> <base> <mod>\n", name);
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
  mpz_t exp, base, mod, result;
  char *resultString;

  mpz_inits(exp, base, mod, result, NULL);
  
  // if three numeric arguments aren't provided
  // print usage and exit
  if (argc != 4) {
    print_usage(argv[0]);
  }

  // parse the inputs
  parse_integer(exp, argv[1]);
  parse_integer(base, argv[2]);
  parse_integer(mod, argv[3]);

  printf("exp=%s\n", mpz_get_str(NULL, 10, exp));
  printf("base=%s\n", mpz_get_str(NULL, 10, base));
  printf("mod=%s\n", mpz_get_str(NULL, 10, mod));
  
  mpz_powm(result, base, exp, mod);

  // use the current allocation function for the result string (and in base 10)
  resultString = mpz_get_str(NULL, 10, result);
  
  printf("%s\n", resultString);
}
