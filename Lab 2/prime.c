/*
 prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <math.h>
/**
 * Returns 1 if n is prime else 0.
 * @param n - the number to be checked for prime status
 * @return - 1 or 0 (true or false) whether or not n is prime
*/
int is_prime(int n){
  if(n<2){
    return 0;
  }
  int sqrt_rounded_up_int = (int) sqrt(n) +1;
  for(int i = 2;i<sqrt_rounded_up_int;i++){
    if(n % i == 0){
      return 0;
    }
  }
  return 1;
}

int main(void){
  printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
  printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
  printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
}


