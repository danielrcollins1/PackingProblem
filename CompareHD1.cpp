/*
=====================================================================
  COMPAREHD1.CPP (Version 1.00)
    Compare hits taken for each HD category, by random simulation.
  Copyright (c) 2009 Daniel R. Collins. All rights reserved.
  See the bottom of this file for any licensing information.
=====================================================================
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>


/*
=====================================================================
  Constants
=====================================================================
*/

const int HD_SIDES = 6;         // Hit dice sides
const int DD_SIDES = 6;         // Damage dice sides
const int NUM_TRIALS = 100000;  // Number of guys to kill


/*
=====================================================================
  Dice-rolling functions
=====================================================================
*/

void diceInit (void) {
  srand(time(NULL));
}


int dieRoll (int sides) {
  return rand() % sides + 1;
}


int diceRoll (int num, int sides) {
  int sum = 0;
  for (int i = 0; i < num; i++) {
    sum += dieRoll(sides);
  }
  return sum;
}


/*
=====================================================================
  Functions
=====================================================================
*/

// How many hits to kill one creature?

int hitsToKill (int HD) {
  int hits = 0;
  int hitPoints = diceRoll(HD, HD_SIDES);
  while (hitPoints > 0) {
    hitPoints -= dieRoll(DD_SIDES);
    hits++;
  }
  return hits;
}


// Average hits to kill over many trials

double averageHitsToKill (int HD) {
  int total = 0;
  for (int i = 0; i < NUM_TRIALS; i++) {
    total += hitsToKill(HD);
  }
  return (double) total / NUM_TRIALS;
}


// Make table

void makeTable () {
  
  // Title
  printf("Hit die equivalence for different HD:\n\n");
  
  // Header
  printf("HD\tHits to Kill\tHits per HD\tPro-Rated\n");
  printf("----------------------------------------------------\n");

  // Body
  double hitsOneHD = averageHitsToKill(1);
  for (int HD = 1; HD <= 15; HD++) {
    double avgHits = averageHitsToKill(HD);
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
  diceInit();
  makeTable();
  return 0;
}

