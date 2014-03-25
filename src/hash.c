#include "hash.h"

/* Bernstein's hash */
unsigned int
bernstein_hash(const char * key) {
  unsigned int hash = 5381;
  while (*key) hash = 33 * hash + *key++;
  return hash ^ (hash >> 16);
}

/* Paul Larson (http://research.microsoft.com/~PALARSON/) */
unsigned int
larson_hash(const char * key) {
  unsigned int hash = 0;
  while (*key) hash = 101 * hash + *key++;
  return hash ^ (hash >> 16);
}

/* Kernighan & Ritchie, "The C programming Language", 3rd edition. */
unsigned int
kr_hash(const char * key) {
  unsigned int hash = 0;
  while (*key) hash = 31 * hash + *key++;
  return hash;
}

/* A hash function with multiplier 65599 (from Red Dragon book) */
unsigned int
x65599_hash(const char * key) {
  unsigned int hash = 0;
  while (*key) hash = 65599 * hash + *key++;
  return hash ^ (hash >> 16);
}

/* FNV hash, http://isthe.com/chongo/tech/comp/fnv/ */
unsigned int
fnv1a_hash(const char * key) {
  unsigned int hash = 2166136261;
  while (*key) hash = 16777619 * (hash ^ *key++);
  return hash ^ (hash >> 16);
}

/* Peter Weinberger's hash (from Red Dragon book) */
unsigned int
weinberger_hash(const char * key) {
  unsigned int h = 0, g;
  while (*key) {
    h = (h << 4) + *key++;
    if(( g = (h & 0xF0000000) ) != 0)
      h ^= g >> 24 ^ g;
  }
  return h ^ (h >> 16);
}

/* Ramakrishna hash */
unsigned int
ramakrishna_hash(const char * key) {
  unsigned int h = 0;
  while (*key) h ^= (h << 5) + (h >> 2) + *key++;
  return h;
}

unsigned int
sdbm_hash(const char * key) {
  unsigned int h = 0;
  while (*key) h = (unsigned char)*key++ + (h << 6) + (h << 16) - h;
  return h;
}
