#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

void print_usage(char *name) {
  printf("Usage: %s <integer> <integer>\n\n", name);
  exit(0);
}

long long int parse_number(char *stringValue) {
  long long int result;

  result = strtoll(stringValue, NULL, 0);
  if ((result == LLONG_MIN) || (result == LLONG_MAX)) {
    perror("invalid number");
    exit(0);
  }
  return result;
}

// Use Euclid's algorithm for small integers
long long int euclid(long long int m, long long int n) {
  long long int t;
  
  while (n != 0) {
    t = n;
    n = m % n;
    m = t;
  }
  return m;
}

long long int gcd(long long int a, long long int b) {
  long long int m, n, g, t;

  m = llabs(a);
  n = llabs(b);

  // the smallest number should be the first argument
  if (n < m) {
    t = m; m = n; n = t;
  }
  if (m == 0) {
    return n;
  }
  return euclid(m, n);
}

int main(int argc, char **argv) {
  long long int g, m, n;

  // if two numeric arguments aren't provided
  // print usage and exit
  if (argc != 3) {
    print_usage(argv[0]);
  }

  // parse the inputs
  m = parse_number(argv[1]);
  n = parse_number(argv[2]);

  g = gcd(m, n);
  
  printf("%lld\n", g);
}
