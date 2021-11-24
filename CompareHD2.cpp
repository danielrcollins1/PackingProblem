/*
=====================================================================
  COMPAREHD2.CPP (Version 1.00)
    Confirms core mechanic by direct calculation.
    Calculate expected hits to take down given hit dice.
    Also, includes functions for x!, nPr, nCr, and dice frequency.
  Copyright (c) 2009 Daniel R. Collins. All rights reserved.
  See the bottom of this file for any licensing information.
=====================================================================
*/

#include <math.h>
#include <stdio.h>
#include <assert.h>


/*
=====================================================================
  Audit functions
=====================================================================
*/

double gMaxFactorial = 0.0;
double gMaxPermutation = 0.0;
double gMaxCombination = 0.0;
double gMaxDiceFreq = 0.0;


// Update an audit

void updateAudit (double *var, double newVal) {
  if (newVal > *var)
    *var = newVal;
}


// Print audit information

void printAudit (void) {
  printf("Max Factorial  : %e\n", gMaxFactorial);
  printf("Max Permutation: %e\n", gMaxPermutation);
  printf("Max Combination: %e\n", gMaxCombination);
  printf("Max Dice Freq. : %e\n", gMaxDiceFreq);
  printf("\n");
}


/*
=====================================================================
  Counting functions
=====================================================================
*/

// Factorial

double factorial (int x) {
  double product = 1;
  for (int i = 1; i <= x; i++) {
    product *= i;
  }
  updateAudit(&gMaxFactorial, product);
  return product;
}


// Permutations

double permutations (int n, int r) {
  double product = 1;
  for (int i = n-r+1; i <= n; i++) {
    product *= i;
  }
  updateAudit(&gMaxPermutation, product);
  return product;
}


// Combinations

double combinations (int n, int r) {
  double ratio = permutations(n,r) / factorial(r);
  updateAudit(&gMaxCombination, ratio);
  return ratio;
}


/*
=====================================================================
  Dice probability functions
    Ways for for (n) dice with (s) sides to roll total (k)
    Formula from Wikipedia: Dice#Probability
=====================================================================
*/

// Negative one raised to a power: (-1)^n

int negOnePow (int n) {
  // -1 if odd, +1 if even
  return (n%2 ? -1 : +1);
}


// Dice frequency

double diceFreq (int n, int s, int k) {
  double sum = 0;
  if (k < n) return 0;
  for (int i = 0; i <= (k-n)/s; i++) {
    sum += negOnePow(i) * combinations(n, i)
      * combinations(k-s*i-1, n-1);
  }
  updateAudit(&gMaxDiceFreq, sum);
  return sum;
}


// Dice probability

double diceProb (int n, int s, int k) {
  return diceFreq(n, s, k) / pow(s, n);
}


/*
=====================================================================
  Hits-to-kill probability functions
=====================================================================
*/

// Probability to kill (h) hit points in (n) hits

double probKillWithHits (int h, int n) {
  // Sum chance to live, return complement
  double sum = 0;
  for (int k = 0; k <= h-1; k++) {
    sum += diceProb(n, 6, k);
  }
  return 1 - sum;
}


// Expected number of hits to kill (h) hit points

double meanHitsToKillHp (int h) {
  double sum = 0;
  double priorProb = 0;
  for (int n = 1; n <= h; n++) {
    double cumulativeProb = probKillWithHits(h, n);
    sum += n * (cumulativeProb - priorProb);
    priorProb = cumulativeProb;
  }
  return sum;
}


// Expected hits to kill creature of (d) hit dice

double meanHitsToKillHd (int d) {
  double sum = 0;
  for (int h = d; h <= 6*d; h++) {
    sum += diceProb(d, 6, h) * meanHitsToKillHp(h);
  }
  return sum;
}


/*
=====================================================================
  Testing functions
=====================================================================
*/

// Confirm double values effectively equal
const double e = 0.001;


// Test suite

void testSuite (void) {
  // Basic counting
  assert( factorial(5) == 120 );
  assert( permutations(5,3) == 60 );
  assert( combinations(5, 3) == 10);

  // Dice probability
  assert( negOnePow(3) == -1 );
  assert( negOnePow(20) == 1 );
  assert( diceFreq(2, 6, 7) == 6);
  assert( diceFreq(3, 6, 10) == 27);
  assert( fabs(diceProb(2, 6, 7) - 0.167) < e );
  assert( fabs(diceProb(3, 6, 10) - 0.125) < e );

  // Hits-to-kill probability
  assert( fabs(probKillWithHits(1, 0) - 0.000) < e );
  assert( fabs(probKillWithHits(1, 1) - 1.000) < e );
  assert( fabs(probKillWithHits(2, 1) - 0.833) < e );
  assert( fabs(probKillWithHits(2, 2) - 1.000) < e );
  assert( fabs(meanHitsToKillHp(1) - 1.000) < e);
  assert( fabs(meanHitsToKillHp(2) - 1.167) < e);
  printf("Test suite SUCCESS.\n");
}


// Make table

void makeTable () {
  
  // Title
  printf("Hit die equivalence for different HD:\n\n");
  
  // Header
  printf("HD\tHits to Kill\tHits per HD\tPro-Rated\n");
  printf("----------------------------------------------------\n");

  // Body
  double hitsOneHD = meanHitsToKillHd(1);
  for (int HD = 1; HD <= 15; HD++) {
    double avgHits = meanHitsToKillHd(HD);
    printf("%d\t", HD);
    printf("%f\t", avgHits);
    printf("%f\t", avgHits/HD);
    printf("%f\t", avgHits/hitsOneHD);
    printf("\n");
  }

  // Tail
  printf("\n\n");
}


// Main function

int main (int argc, char *argv[]) {
  testSuite();
  makeTable();
  printAudit();
  return 0;
}

