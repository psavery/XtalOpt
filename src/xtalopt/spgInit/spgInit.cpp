
#include <xtalopt/spgInit/spgInit.h>

// For RANDDOUBLE()
#include <globalsearch/macros.h>

#include <tuple>
#include <iostream>

// Define this for debug output
#define SPGINIT_DEBUG

using namespace std;

// Outer vector is the space group. So there are 230 of them
// Inner vector depends on the number of wyckoff elements that
// exist in each space group
// This contains the wyckoff letter, multiplicity, and x,y,z coordinates for
// the first wyckoff position of each spacegroup
// This list was obtained by parsing html files at
// http://www.cryst.ehu.es/cgi-bin/cryst/programs/nph-table?from=getwp
// on 12/04/15

// For spacegroups with an origin choice, origin choices are all 2
// For spacegroups with rhombohedral vs. hexagonal, all are hexagonal
static const vector<wyckoffPositions> wyckoffPositionsDatabase
{
  { // 0. Not a real space group...
    wyckInfo{'a',0," "}
  },

  { // 1
    wyckInfo{'a',1,"x,y,z"}
  },

  { // 2
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',1,"0,0.5,0"},
    wyckInfo{'d',1,"0.5,0,0"},
    wyckInfo{'e',1,"0.5,0.5,0"},
    wyckInfo{'f',1,"0.5,0,0.5"},
    wyckInfo{'g',1,"0,0.5,0.5"},
    wyckInfo{'h',1,"0.5,0.5,0.5"},
    wyckInfo{'i',2,"x,y,z"}
  },

  { // 3 - unique axis b
    wyckInfo{'a',1,"0,y,0"},
    wyckInfo{'b',1,"0,y,0.5"},
    wyckInfo{'c',1,"0.5,y,0"},
    wyckInfo{'d',1,"0.5,y,0.5"},
    wyckInfo{'e',2,"x,y,z"}
  },

  { // 4 - unique axis b
    wyckInfo{'a',2,"x,y,z"}
  },

  { // 5 - unique axis b
    wyckInfo{'a',2,"0,y,0"},
    wyckInfo{'b',2,"0,y,0.5"},
    wyckInfo{'c',4,"x,y,z"}
  },

  { // 6 - unique axis b
    wyckInfo{'a',1,"x,0,z"},
    wyckInfo{'b',1,"x,0.5,z"},
    wyckInfo{'c',2,"x,y,z"}
  },

  { // 7 - unique axis b
    wyckInfo{'a',2,"x,y,z"}
  },

  { // 8 - unique axis b
    wyckInfo{'a',2,"x,0,z"},
    wyckInfo{'b',4,"x,y,z"}
  },

  { // 9 - unique axis b
    wyckInfo{'a',4,"x,y,z"}
  },

  { // 10 - unique axis b
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0.5,0"},
    wyckInfo{'c',1,"0,0,0.5"},
    wyckInfo{'d',1,"0.5,0,0"},
    wyckInfo{'e',1,"0.5,0.5,0"},
    wyckInfo{'f',1,"0,0.5,0.5"},
    wyckInfo{'g',1,"0.5,0,0.5"},
    wyckInfo{'h',1,"0.5,0.5,0.5"},
    wyckInfo{'i',2,"0,y,0"},
    wyckInfo{'j',2,"0.5,y,0"},
    wyckInfo{'k',2,"0,y,0.5"},
    wyckInfo{'l',2,"0.5,y,0.5"},
    wyckInfo{'m',2,"x,0,z"},
    wyckInfo{'n',2,"x,0.5,z"},
    wyckInfo{'o',4,"x,y,z"}
  },

  { // 11 - unique axis b
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0.5,0,0"},
    wyckInfo{'c',2,"0,0,0.5"},
    wyckInfo{'d',2,"0.5,0,0.5"},
    wyckInfo{'e',2,"x,0.25,z"},
    wyckInfo{'f',4,"x,y,z"}
  },

  { // 12 - unique axis b
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0.5,0"},
    wyckInfo{'c',2,"0,0,0.5"},
    wyckInfo{'d',2,"0,0.5,0.5"},
    wyckInfo{'e',4,"0.25,0.25,0"},
    wyckInfo{'f',4,"0.25,0.25,0.5"},
    wyckInfo{'g',4,"0,y,0"},
    wyckInfo{'h',4,"0,y,0.5"},
    wyckInfo{'i',4,"x,0,z"},
    wyckInfo{'j',8,"x,y,z"}
  },

  { // 13 - unique axis b
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0.5,0.5,0"},
    wyckInfo{'c',2,"0,0.5,0"},
    wyckInfo{'d',2,"0.5,0,0"},
    wyckInfo{'e',2,"0,y,0.25"},
    wyckInfo{'f',2,"0.5,y,0.25"},
    wyckInfo{'g',4,"x,y,z"}
  },

  { // 14 - unique axis b
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0.5,0,0"},
    wyckInfo{'c',2,"0,0,0.5"},
    wyckInfo{'d',2,"0.5,0,0.5"},
    wyckInfo{'e',4,"x,y,z"}
  },

  { // 15 - unique axis b
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0.5,0"},
    wyckInfo{'c',4,"0.25,0.25,0"},
    wyckInfo{'d',4,"0.25,0.25,0.5"},
    wyckInfo{'e',4,"0,y,0.25"},
    wyckInfo{'f',8,"x,y,z"}
  },

  { // 16
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0.5,0,0"},
    wyckInfo{'c',1,"0,0.5,0"},
    wyckInfo{'d',1,"0,0,0.5"},
    wyckInfo{'e',1,"0.5,0.5,0"},
    wyckInfo{'f',1,"0.5,0,0.5"},
    wyckInfo{'g',1,"0,0.5,0.5"},
    wyckInfo{'h',1,"0.5,0.5,0.5"},
    wyckInfo{'i',2,"x,0,0"},
    wyckInfo{'j',2,"x,0,0.5"},
    wyckInfo{'k',2,"x,0.5,0"},
    wyckInfo{'l',2,"x,0.5,0.5"},
    wyckInfo{'m',2,"0,y,0"},
    wyckInfo{'n',2,"0,y,0.5"},
    wyckInfo{'o',2,"0.5,y,0"},
    wyckInfo{'p',2,"0.5,y,0.5"},
    wyckInfo{'q',2,"0,0,z"},
    wyckInfo{'r',2,"0.5,0,z"},
    wyckInfo{'s',2,"0,0.5,z"},
    wyckInfo{'t',2,"0.5,0.5,z"},
    wyckInfo{'u',4,"x,y,z"}
  },

  { // 17
    wyckInfo{'a',2,"x,0,0"},
    wyckInfo{'b',2,"x,0.5,0"},
    wyckInfo{'c',2,"0,y,0.25"},
    wyckInfo{'d',2,"0.5,y,0.25"},
    wyckInfo{'e',4,"x,y,z"}
  },

  { // 18
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0,0.5,z"},
    wyckInfo{'c',4,"x,y,z"}
  },

  { // 19
    wyckInfo{'a',4,"x,y,z"}
  },

  { // 20
    wyckInfo{'a',4,"x,0,0"},
    wyckInfo{'b',4,"0,y,0.25"},
    wyckInfo{'c',8,"x,y,z"}
  },

  { // 21
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0.5,0"},
    wyckInfo{'c',2,"0.5,0,0.5"},
    wyckInfo{'d',2,"0,0,0.5"},
    wyckInfo{'e',4,"x,0,0"},
    wyckInfo{'f',4,"x,0,0.5"},
    wyckInfo{'g',4,"0,y,0"},
    wyckInfo{'h',4,"0,y,0.5"},
    wyckInfo{'i',4,"0,0,z"},
    wyckInfo{'j',4,"0,0.5,z"},
    wyckInfo{'k',4,"0.25,0.25,z"},
    wyckInfo{'l',8,"x,y,z"}
  },

  { // 22
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0,0.5"},
    wyckInfo{'c',4,"0.25,0.25,0.25"},
    wyckInfo{'d',4,"0.25,0.25,0.75"},
    wyckInfo{'e',8,"x,0,0"},
    wyckInfo{'f',8,"0,y,0"},
    wyckInfo{'g',8,"0,0,z"},
    wyckInfo{'h',8,"0.25,0.25,z"},
    wyckInfo{'i',8,"0.25,y,0.25"},
    wyckInfo{'j',8,"x,0.25,0.25"},
    wyckInfo{'k',16,"x,y,z"}
  },

  { // 23
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0.5,0,0"},
    wyckInfo{'c',2,"0,0,0.5"},
    wyckInfo{'d',2,"0,0.5,0"},
    wyckInfo{'e',4,"x,0,0"},
    wyckInfo{'f',4,"x,0,0.5"},
    wyckInfo{'g',4,"0,y,0"},
    wyckInfo{'h',4,"0.5,y,0"},
    wyckInfo{'i',4,"0,0,z"},
    wyckInfo{'j',4,"0,0.5,z"},
    wyckInfo{'k',8,"x,y,z"}
  },

  { // 24
    wyckInfo{'a',4,"x,0,0.25"},
    wyckInfo{'b',4,"0.25,y,0"},
    wyckInfo{'c',4,"0,0.25,z"},
    wyckInfo{'d',8,"x,y,z"}
  },

  { // 25
    wyckInfo{'a',1,"0,0,z"},
    wyckInfo{'b',1,"0,0.5,z"},
    wyckInfo{'c',1,"0.5,0,z"},
    wyckInfo{'d',1,"0.5,0.5,z"},
    wyckInfo{'e',2,"x,0,z"},
    wyckInfo{'f',2,"x,0.5,z"},
    wyckInfo{'g',2,"0,y,z"},
    wyckInfo{'h',2,"0.5,y,z"},
    wyckInfo{'i',4,"x,y,z"}
  },

  { // 26
    wyckInfo{'a',2,"0,y,z"},
    wyckInfo{'b',2,"0.5,y,z"},
    wyckInfo{'c',4,"x,y,z"}
  },

  { // 27
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0,0.5,z"},
    wyckInfo{'c',2,"0.5,0,z"},
    wyckInfo{'d',2,"0.5,0.5,z"},
    wyckInfo{'e',4,"x,y,z"}
  },

  { // 28
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0,0.5,z"},
    wyckInfo{'c',2,"0.25,y,z"},
    wyckInfo{'d',4,"x,y,z"}
  },

  { // 29
    wyckInfo{'a',4,"x,y,z"}
  },

  { // 30
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.5,0,z"},
    wyckInfo{'c',4,"x,y,z"}
  },

  { // 31
    wyckInfo{'a',2,"0,y,z"},
    wyckInfo{'b',4,"x,y,z"}
  },

  { // 32
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0,0.5,z"},
    wyckInfo{'c',4,"x,y,z"}
  },

  { // 33
    wyckInfo{'a',4,"x,y,z"}
  },

  { // 34
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0,0.5,z"},
    wyckInfo{'c',4,"x,y,z"}
  },

  { // 35
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0,0.5,z"},
    wyckInfo{'c',4,"0.25,0.25,z"},
    wyckInfo{'d',4,"x,0,z"},
    wyckInfo{'e',4,"0,y,z"},
    wyckInfo{'f',8,"x,y,z"}
  },

  { // 36
    wyckInfo{'a',4,"0,y,z"},
    wyckInfo{'b',8,"x,y,z"}
  },

  { // 37
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',4,"0,0.5,z"},
    wyckInfo{'c',4,"0.25,0.25,z"},
    wyckInfo{'d',8,"x,y,z"}
  },

  { // 38
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.5,0,z"},
    wyckInfo{'c',4,"x,0,z"},
    wyckInfo{'d',4,"0,y,z"},
    wyckInfo{'e',4,"0.5,y,z"},
    wyckInfo{'f',8,"x,y,z"}
  },

  { // 39
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',4,"0.5,0,z"},
    wyckInfo{'c',4,"x,0.25,z"},
    wyckInfo{'d',8,"x,y,z"}
  },

  { // 40
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',4,"0.25,y,z"},
    wyckInfo{'c',8,"x,y,z"}
  },

  { // 41
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',8,"x,y,z"}
  },

  { // 42
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',8,"0.25,0.25,z"},
    wyckInfo{'c',8,"0,y,z"},
    wyckInfo{'d',8,"x,0,z"},
    wyckInfo{'e',16,"x,y,z"}
  },

  { // 43
    wyckInfo{'a',8,"0,0,z"},
    wyckInfo{'b',16,"x,y,z"}
  },

  { // 44
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0,0.5,z"},
    wyckInfo{'c',4,"x,0,z"},
    wyckInfo{'d',4,"0,y,z"},
    wyckInfo{'e',8,"x,y,z"}
  },

  { // 45
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',4,"0,0.5,z"},
    wyckInfo{'c',8,"x,y,z"}
  },

  { // 46
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',4,"0.25,y,z"},
    wyckInfo{'c',8,"x,y,z"}
  },

  { // 47
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0.5,0,0"},
    wyckInfo{'c',1,"0,0,0.5"},
    wyckInfo{'d',1,"0.5,0,0.5"},
    wyckInfo{'e',1,"0,0.5,0"},
    wyckInfo{'f',1,"0.5,0.5,0"},
    wyckInfo{'g',1,"0,0.5,0.5"},
    wyckInfo{'h',1,"0.5,0.5,0.5"},
    wyckInfo{'i',2,"x,0,0"},
    wyckInfo{'j',2,"x,0,0.5"},
    wyckInfo{'k',2,"x,0.5,0"},
    wyckInfo{'l',2,"x,0.5,0.5"},
    wyckInfo{'m',2,"0,y,0"},
    wyckInfo{'n',2,"0,y,0.5"},
    wyckInfo{'o',2,"0.5,y,0"},
    wyckInfo{'p',2,"0.5,y,0.5"},
    wyckInfo{'q',2,"0,0,z"},
    wyckInfo{'r',2,"0,0.5,z"},
    wyckInfo{'s',2,"0.5,0,z"},
    wyckInfo{'t',2,"0.5,0.5,z"},
    wyckInfo{'u',4,"0,y,z"},
    wyckInfo{'v',4,"0.5,y,z"},
    wyckInfo{'w',4,"x,0,z"},
    wyckInfo{'x',4,"x,0.5,z"},
    wyckInfo{'y',4,"x,y,0"},
    wyckInfo{'z',4,"x,y,0.5"},
    wyckInfo{'A',8,"x,y,z"}
  },

  { // 48
    wyckInfo{'a',2,"0.25,0.25,0.25"},
    wyckInfo{'b',2,"0.75,0.25,0.25"},
    wyckInfo{'c',2,"0.25,0.25,0.75"},
    wyckInfo{'d',2,"0.25,0.75,0.25"},
    wyckInfo{'e',4,"0.5,0.5,0.5"},
    wyckInfo{'f',4,"0,0,0"},
    wyckInfo{'g',4,"x,0.25,0.25"},
    wyckInfo{'h',4,"x,0.25,0.75"},
    wyckInfo{'i',4,"0.25,y,0.25"},
    wyckInfo{'j',4,"0.75,y,0.25"},
    wyckInfo{'k',4,"0.25,0.25,z"},
    wyckInfo{'l',4,"0.25,0.75,z"},
    wyckInfo{'m',8,"x,y,z"}
  },

  { // 49
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0.5,0.5,0"},
    wyckInfo{'c',2,"0,0.5,0"},
    wyckInfo{'d',2,"0.5,0,0"},
    wyckInfo{'e',2,"0,0,0.25"},
    wyckInfo{'f',2,"0.5,0,0.25"},
    wyckInfo{'g',2,"0,0.5,0.25"},
    wyckInfo{'h',2,"0.5,0.5,0.25"},
    wyckInfo{'i',4,"x,0,0.25"},
    wyckInfo{'j',4,"x,0.5,0.25"},
    wyckInfo{'k',4,"0,y,0.25"},
    wyckInfo{'l',4,"0.5,y,0.25"},
    wyckInfo{'m',4,"0,0,z"},
    wyckInfo{'n',4,"0.5,0.5,z"},
    wyckInfo{'o',4,"0,0.5,z"},
    wyckInfo{'p',4,"0.5,0,z"},
    wyckInfo{'q',4,"x,y,0"},
    wyckInfo{'r',8,"x,y,z"}
  },

  { // 50
    wyckInfo{'a',2,"0.25,0.25,0"},
    wyckInfo{'b',2,"0.75,0.25,0"},
    wyckInfo{'c',2,"0.75,0.25,0.5"},
    wyckInfo{'d',2,"0.25,0.25,0.5"},
    wyckInfo{'e',4,"0,0,0"},
    wyckInfo{'f',4,"0,0,0.5"},
    wyckInfo{'g',4,"x,0.25,0"},
    wyckInfo{'h',4,"x,0.25,0.5"},
    wyckInfo{'i',4,"0.25,y,0"},
    wyckInfo{'j',4,"0.25,y,0.5"},
    wyckInfo{'k',4,"0.25,0.25,z"},
    wyckInfo{'l',4,"0.25,0.75,z"},
    wyckInfo{'m',8,"x,y,z"}
  },

  { // 51
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0.5,0"},
    wyckInfo{'c',2,"0,0,0.5"},
    wyckInfo{'d',2,"0,0.5,0.5"},
    wyckInfo{'e',2,"0.25,0,z"},
    wyckInfo{'f',2,"0.25,0.5,z"},
    wyckInfo{'g',4,"0,y,0"},
    wyckInfo{'h',4,"0,y,0.5"},
    wyckInfo{'i',4,"x,0,z"},
    wyckInfo{'j',4,"x,0.5,z"},
    wyckInfo{'k',4,"0.25,y,z"},
    wyckInfo{'l',8,"x,y,z"}
  },

  { // 52
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0,0.5"},
    wyckInfo{'c',4,"0.25,0,z"},
    wyckInfo{'d',4,"x,0.25,0.25"},
    wyckInfo{'e',8,"x,y,z"}
  },

  { // 53
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0.5,0,0"},
    wyckInfo{'c',2,"0.5,0.5,0"},
    wyckInfo{'d',2,"0,0.5,0"},
    wyckInfo{'e',4,"x,0,0"},
    wyckInfo{'f',4,"x,0.5,0"},
    wyckInfo{'g',4,"0.25,y,0.25"},
    wyckInfo{'h',4,"0,y,z"},
    wyckInfo{'i',8,"x,y,z"}
  },

  { // 54
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0.5,0"},
    wyckInfo{'c',4,"0,y,0.25"},
    wyckInfo{'d',4,"0.25,0,z"},
    wyckInfo{'e',4,"0.25,0.5,z"},
    wyckInfo{'f',8,"x,y,z"}
  },

  { // 55
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',2,"0,0.5,0"},
    wyckInfo{'d',2,"0,0.5,0.5"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0,0.5,z"},
    wyckInfo{'g',4,"x,y,0"},
    wyckInfo{'h',4,"x,y,0.5"},
    wyckInfo{'i',8,"x,y,z"}
  },

  { // 56
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0,0.5"},
    wyckInfo{'c',4,"0.25,0.25,z"},
    wyckInfo{'d',4,"0.25,0.75,z"},
    wyckInfo{'e',8,"x,y,z"}
  },

  { // 57
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0.5,0,0"},
    wyckInfo{'c',4,"x,0.25,0"},
    wyckInfo{'d',4,"x,y,0.25"},
    wyckInfo{'e',8,"x,y,z"}
  },

  { // 58
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',2,"0,0.5,0"},
    wyckInfo{'d',2,"0,0.5,0.5"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0,0.5,z"},
    wyckInfo{'g',4,"x,y,0"},
    wyckInfo{'h',8,"x,y,z"}
  },

  { // 59
    wyckInfo{'a',2,"0.25,0.25,z"},
    wyckInfo{'b',2,"0.25,0.75,z"},
    wyckInfo{'c',4,"0,0,0"},
    wyckInfo{'d',4,"0,0,0.5"},
    wyckInfo{'e',4,"0.25,y,z"},
    wyckInfo{'f',4,"x,0.25,z"},
    wyckInfo{'g',8,"x,y,z"}
  },

  { // 60
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0.5,0"},
    wyckInfo{'c',4,"0,y,0.25"},
    wyckInfo{'d',8,"x,y,z"}
  },

  { // 61
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0,0.5"},
    wyckInfo{'c',8,"x,y,z"}
  },

  { // 62
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0,0.5"},
    wyckInfo{'c',4,"x,0.25,z"},
    wyckInfo{'d',8,"x,y,z"}
  },

  { // 63
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0.5,0"},
    wyckInfo{'c',4,"0,y,0.25"},
    wyckInfo{'d',8,"0.25,0.25,0"},
    wyckInfo{'e',8,"x,0,0"},
    wyckInfo{'f',8,"0,y,z"},
    wyckInfo{'g',8,"x,y,0.25"},
    wyckInfo{'h',16,"x,y,z"}
  },

  { // 64
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0.5,0,0"},
    wyckInfo{'c',8,"0.25,0.25,0"},
    wyckInfo{'d',8,"x,0,0"},
    wyckInfo{'e',8,"0.25,y,0.25"},
    wyckInfo{'f',8,"0,y,z"},
    wyckInfo{'g',16,"x,y,z"}
  },

  { // 65
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0.5,0,0"},
    wyckInfo{'c',2,"0.5,0,0.5"},
    wyckInfo{'d',2,"0,0,0.5"},
    wyckInfo{'e',4,"0.25,0.25,0"},
    wyckInfo{'f',4,"0.25,0.25,0.5"},
    wyckInfo{'g',4,"x,0,0"},
    wyckInfo{'h',4,"x,0,0.5"},
    wyckInfo{'i',4,"0,y,0"},
    wyckInfo{'j',4,"0,y,0.5"},
    wyckInfo{'k',4,"0,0,z"},
    wyckInfo{'l',4,"0,0.5,z"},
    wyckInfo{'m',8,"0.25,0.25,z"},
    wyckInfo{'n',8,"0,y,z"},
    wyckInfo{'o',8,"x,0,z"},
    wyckInfo{'p',8,"x,y,0"},
    wyckInfo{'q',8,"x,y,0.5"},
    wyckInfo{'r',16,"x,y,z"}
  },

  { // 66
    wyckInfo{'a',4,"0,0,0.25"},
    wyckInfo{'b',4,"0,0.5,0.25"},
    wyckInfo{'c',4,"0,0,0"},
    wyckInfo{'d',4,"0,0.5,0"},
    wyckInfo{'e',4,"0.25,0.25,0"},
    wyckInfo{'f',4,"0.25,0.75,0"},
    wyckInfo{'g',8,"x,0,0.25"},
    wyckInfo{'h',8,"0,y,0.25"},
    wyckInfo{'i',8,"0,0,z"},
    wyckInfo{'j',8,"0,0.5,z"},
    wyckInfo{'k',8,"0.25,0.25,z"},
    wyckInfo{'l',8,"x,y,0"},
    wyckInfo{'m',16,"x,y,z"}
  },

  { // 67
    wyckInfo{'a',4,"0.25,0,0"},
    wyckInfo{'b',4,"0.25,0,0.5"},
    wyckInfo{'c',4,"0,0,0"},
    wyckInfo{'d',4,"0,0,0.5"},
    wyckInfo{'e',4,"0.25,0.25,0"},
    wyckInfo{'f',4,"0.25,0.25,0.5"},
    wyckInfo{'g',4,"0,0.25,z"},
    wyckInfo{'h',8,"x,0,0"},
    wyckInfo{'i',8,"x,0,0.5"},
    wyckInfo{'j',8,"0.25,y,0"},
    wyckInfo{'k',8,"0.25,y,0.5"},
    wyckInfo{'l',8,"0.25,0,z"},
    wyckInfo{'m',8,"0,y,z"},
    wyckInfo{'n',8,"x,0.25,z"},
    wyckInfo{'o',16,"x,y,z"}
  },

  { // 68
    wyckInfo{'a',4,"0,0.25,0.25"},
    wyckInfo{'b',4,"0,0.25,0.75"},
    wyckInfo{'c',8,"0.25,0.75,0"},
    wyckInfo{'d',8,"0,0,0"},
    wyckInfo{'e',8,"x,0.25,0.25"},
    wyckInfo{'f',8,"0,y,0.25"},
    wyckInfo{'g',8,"0,0.25,z"},
    wyckInfo{'h',8,"0.25,0,z"},
    wyckInfo{'i',16,"x,y,z"}
  },

  { // 69
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0,0.5"},
    wyckInfo{'c',8,"0,0.25,0.25"},
    wyckInfo{'d',8,"0.25,0,0.25"},
    wyckInfo{'e',8,"0.25,0.25,0"},
    wyckInfo{'f',8,"0.25,0.25,0.25"},
    wyckInfo{'g',8,"x,0,0"},
    wyckInfo{'h',8,"0,y,0"},
    wyckInfo{'i',8,"0,0,z"},
    wyckInfo{'j',16,"0.25,0.25,z"},
    wyckInfo{'k',16,"0.25,y,0.25"},
    wyckInfo{'l',16,"x,0.25,0.25"},
    wyckInfo{'m',16,"0,y,z"},
    wyckInfo{'n',16,"x,0,z"},
    wyckInfo{'o',16,"x,y,0"},
    wyckInfo{'p',32,"x,y,z"}
  },

  { // 70
    wyckInfo{'a',8,"0.125,0.125,0.125"},
    wyckInfo{'b',8,"0.125,0.125,0.625"},
    wyckInfo{'c',16,"0,0,0"},
    wyckInfo{'d',16,"0.5,0.5,0.5"},
    wyckInfo{'e',16,"x,0.125,0.125"},
    wyckInfo{'f',16,"0.125,y,0.125"},
    wyckInfo{'g',16,"0.125,0.125,z"},
    wyckInfo{'h',32,"x,y,z"}
  },

  { // 71
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0.5,0.5"},
    wyckInfo{'c',2,"0.5,0.5,0"},
    wyckInfo{'d',2,"0.5,0,0.5"},
    wyckInfo{'e',4,"x,0,0"},
    wyckInfo{'f',4,"x,0.5,0"},
    wyckInfo{'g',4,"0,y,0"},
    wyckInfo{'h',4,"0,y,0.5"},
    wyckInfo{'i',4,"0,0,z"},
    wyckInfo{'j',4,"0.5,0,z"},
    wyckInfo{'k',8,"0.25,0.25,0.25"},
    wyckInfo{'l',8,"0,y,z"},
    wyckInfo{'m',8,"x,0,z"},
    wyckInfo{'n',8,"x,y,0"},
    wyckInfo{'o',16,"x,y,z"}
  },

  { // 72
    wyckInfo{'a',4,"0,0,0.25"},
    wyckInfo{'b',4,"0.5,0,0.25"},
    wyckInfo{'c',4,"0,0,0"},
    wyckInfo{'d',4,"0.5,0,0"},
    wyckInfo{'e',8,"0.25,0.25,0.25"},
    wyckInfo{'f',8,"x,0,0.25"},
    wyckInfo{'g',8,"0,y,0.25"},
    wyckInfo{'h',8,"0,0,z"},
    wyckInfo{'i',8,"0,0.5,z"},
    wyckInfo{'j',8,"x,y,0"},
    wyckInfo{'k',16,"x,y,z"}
  },

  { // 73
    wyckInfo{'a',8,"0,0,0"},
    wyckInfo{'b',8,"0.25,0.25,0.25"},
    wyckInfo{'c',8,"x,0,0.25"},
    wyckInfo{'d',8,"0.25,y,0"},
    wyckInfo{'e',8,"0,0.25,z"},
    wyckInfo{'f',16,"x,y,z"}
  },

  { // 74
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0,0.5"},
    wyckInfo{'c',4,"0.25,0.25,0.25"},
    wyckInfo{'d',4,"0.25,0.25,0.75"},
    wyckInfo{'e',4,"0,0.25,z"},
    wyckInfo{'f',8,"x,0,0"},
    wyckInfo{'g',8,"0.25,y,0.25"},
    wyckInfo{'h',8,"0,y,z"},
    wyckInfo{'i',8,"x,0.25,z"},
    wyckInfo{'j',16,"x,y,z"}
  },

  { // 75
    wyckInfo{'a',1,"0,0,z"},
    wyckInfo{'b',1,"0.5,0.5,z"},
    wyckInfo{'c',2,"0,0.5,z"},
    wyckInfo{'d',4,"x,y,z"}
  },

  { // 76
    wyckInfo{'a',4,"x,y,z"}
  },

  { // 77
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.5,0.5,z"},
    wyckInfo{'c',2,"0,0.5,z"},
    wyckInfo{'d',4,"x,y,z"}
  },

  { // 78
    wyckInfo{'a',4,"x,y,z"}
  },

  { // 79
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',4,"0,0.5,z"},
    wyckInfo{'c',8,"x,y,z"}
  },

  { // 80
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',8,"x,y,z"}
  },

  { // 81
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',1,"0.5,0.5,0"},
    wyckInfo{'d',1,"0.5,0.5,0.5"},
    wyckInfo{'e',2,"0,0,z"},
    wyckInfo{'f',2,"0.5,0.5,z"},
    wyckInfo{'g',2,"0,0.5,z"},
    wyckInfo{'h',4,"x,y,z"}
  },

  { // 82
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',2,"0,0.5,0.25"},
    wyckInfo{'d',2,"0,0.5,0.75"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0,0.5,z"},
    wyckInfo{'g',8,"x,y,z"}
  },

  { // 83
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',1,"0.5,0.5,0"},
    wyckInfo{'d',1,"0.5,0.5,0.5"},
    wyckInfo{'e',2,"0,0.5,0"},
    wyckInfo{'f',2,"0,0.5,0.5"},
    wyckInfo{'g',2,"0,0,z"},
    wyckInfo{'h',2,"0.5,0.5,z"},
    wyckInfo{'i',4,"0,0.5,z"},
    wyckInfo{'j',4,"x,y,0"},
    wyckInfo{'k',4,"x,y,0.5"},
    wyckInfo{'l',8,"x,y,z"}
  },

  { // 84
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0.5,0.5,0"},
    wyckInfo{'c',2,"0,0.5,0"},
    wyckInfo{'d',2,"0,0.5,0.5"},
    wyckInfo{'e',2,"0,0,0.25"},
    wyckInfo{'f',2,"0.5,0.5,0.25"},
    wyckInfo{'g',4,"0,0,z"},
    wyckInfo{'h',4,"0.5,0.5,z"},
    wyckInfo{'i',4,"0,0.5,z"},
    wyckInfo{'j',4,"x,y,0"},
    wyckInfo{'k',8,"x,y,z"}
  },

  { // 85
    wyckInfo{'a',2,"0.25,0.75,0"},
    wyckInfo{'b',2,"0.25,0.75,0.5"},
    wyckInfo{'c',2,"0.25,0.25,z"},
    wyckInfo{'d',4,"0,0,0"},
    wyckInfo{'e',4,"0,0,0.5"},
    wyckInfo{'f',4,"0.25,0.75,z"},
    wyckInfo{'g',8,"x,y,z"}
  },

  { // 86
    wyckInfo{'a',2,"0.25,0.25,0.25"},
    wyckInfo{'b',2,"0.25,0.25,0.75"},
    wyckInfo{'c',4,"0,0,0"},
    wyckInfo{'d',4,"0,0,0.5"},
    wyckInfo{'e',4,"0.75,0.25,z"},
    wyckInfo{'f',4,"0.25,0.25,z"},
    wyckInfo{'g',8,"x,y,z"}
  },

  { // 87
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',4,"0,0.5,0"},
    wyckInfo{'d',4,"0,0.5,0.25"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',8,"0.25,0.25,0.25"},
    wyckInfo{'g',8,"0,0.5,z"},
    wyckInfo{'h',8,"x,y,0"},
    wyckInfo{'i',16,"x,y,z"}
  },

  { // 88
    wyckInfo{'a',4,"0,0.25,0.125"},
    wyckInfo{'b',4,"0,0.25,0.625"},
    wyckInfo{'c',8,"0,0,0"},
    wyckInfo{'d',8,"0,0,0.5"},
    wyckInfo{'e',8,"0,0.25,z"},
    wyckInfo{'f',16,"x,y,z"}
  },

  { // 89
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',1,"0.5,0.5,0"},
    wyckInfo{'d',1,"0.5,0.5,0.5"},
    wyckInfo{'e',2,"0.5,0,0"},
    wyckInfo{'f',2,"0.5,0,0.5"},
    wyckInfo{'g',2,"0,0,z"},
    wyckInfo{'h',2,"0.5,0.5,z"},
    wyckInfo{'i',4,"0,0.5,z"},
    wyckInfo{'j',4,"x,x,0"},
    wyckInfo{'k',4,"x,x,0.5"},
    wyckInfo{'l',4,"x,0,0"},
    wyckInfo{'m',4,"x,0.5,0.5"},
    wyckInfo{'n',4,"x,0,0.5"},
    wyckInfo{'o',4,"x,0.5,0"},
    wyckInfo{'p',8,"x,y,z"}
  },

  { // 90
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',2,"0,0.5,z"},
    wyckInfo{'d',4,"0,0,z"},
    wyckInfo{'e',4,"x,x,0"},
    wyckInfo{'f',4,"x,x,0.5"},
    wyckInfo{'g',8,"x,y,z"}
  },

  { // 91
    wyckInfo{'a',4,"0,y,0"},
    wyckInfo{'b',4,"0.5,y,0"},
    wyckInfo{'c',4,"x,x,0.375"},
    wyckInfo{'d',8,"x,y,z"}
  },

  { // 92
    wyckInfo{'a',4,"x,x,0"},
    wyckInfo{'b',8,"x,y,z"}
  },

  { // 93
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0.5,0.5,0"},
    wyckInfo{'c',2,"0,0.5,0"},
    wyckInfo{'d',2,"0,0.5,0.5"},
    wyckInfo{'e',2,"0,0,0.25"},
    wyckInfo{'f',2,"0.5,0.5,0.25"},
    wyckInfo{'g',4,"0,0,z"},
    wyckInfo{'h',4,"0.5,0.5,z"},
    wyckInfo{'i',4,"0,0.5,z"},
    wyckInfo{'j',4,"x,0,0"},
    wyckInfo{'k',4,"x,0.5,0.5"},
    wyckInfo{'l',4,"x,0,0.5"},
    wyckInfo{'m',4,"x,0.5,0"},
    wyckInfo{'n',4,"x,x,0.25"},
    wyckInfo{'o',4,"x,x,0.75"},
    wyckInfo{'p',8,"x,y,z"}
  },

  { // 94
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',4,"0,0,z"},
    wyckInfo{'d',4,"0,0.5,z"},
    wyckInfo{'e',4,"x,x,0"},
    wyckInfo{'f',4,"x,x,0.5"},
    wyckInfo{'g',8,"x,y,z"}
  },

  { // 95
    wyckInfo{'a',4,"0,y,0"},
    wyckInfo{'b',4,"0.5,y,0"},
    wyckInfo{'c',4,"x,x,0.625"},
    wyckInfo{'d',8,"x,y,z"}
  },

  { // 96
    wyckInfo{'a',4,"x,x,0"},
    wyckInfo{'b',8,"x,y,z"}
  },

  { // 97
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',4,"0,0.5,0"},
    wyckInfo{'d',4,"0,0.5,0.25"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',8,"0,0.5,z"},
    wyckInfo{'g',8,"x,x,0"},
    wyckInfo{'h',8,"x,0,0"},
    wyckInfo{'i',8,"x,0,0.5"},
    wyckInfo{'j',8,"x,x+0.5,0.25"},
    wyckInfo{'k',16,"x,y,z"}
  },

  { // 98
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0,0.5"},
    wyckInfo{'c',8,"0,0,z"},
    wyckInfo{'d',8,"x,x,0"},
    wyckInfo{'e',8,"-x,x,0"},
    wyckInfo{'f',8,"x,0.25,0.125"},
    wyckInfo{'g',16,"x,y,z"}
  },

  { // 99
    wyckInfo{'a',1,"0,0,z"},
    wyckInfo{'b',1,"0.5,0.5,z"},
    wyckInfo{'c',2,"0.5,0,z"},
    wyckInfo{'d',4,"x,x,z"},
    wyckInfo{'e',4,"x,0,z"},
    wyckInfo{'f',4,"x,0.5,z"},
    wyckInfo{'g',8,"x,y,z"}
  },

  { // 100
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.5,0,z"},
    wyckInfo{'c',4,"x,x+0.5,z"},
    wyckInfo{'d',8,"x,y,z"}
  },

  { // 101
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.5,0.5,z"},
    wyckInfo{'c',4,"0,0.5,z"},
    wyckInfo{'d',4,"x,x,z"},
    wyckInfo{'e',8,"x,y,z"}
  },

  { // 102
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',4,"0,0.5,z"},
    wyckInfo{'c',4,"x,x,z"},
    wyckInfo{'d',8,"x,y,z"}
  },

  { // 103
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.5,0.5,z"},
    wyckInfo{'c',4,"0,0.5,z"},
    wyckInfo{'d',8,"x,y,z"}
  },

  { // 104
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',4,"0,0.5,z"},
    wyckInfo{'c',8,"x,y,z"}
  },

  { // 105
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.5,0.5,z"},
    wyckInfo{'c',2,"0,0.5,z"},
    wyckInfo{'d',4,"x,0,z"},
    wyckInfo{'e',4,"x,0.5,z"},
    wyckInfo{'f',8,"x,y,z"}
  },

  { // 106
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',4,"0,0.5,z"},
    wyckInfo{'c',8,"x,y,z"}
  },

  { // 107
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',4,"0,0.5,z"},
    wyckInfo{'c',8,"x,x,z"},
    wyckInfo{'d',8,"x,0,z"},
    wyckInfo{'e',16,"x,y,z"}
  },

  { // 108
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',4,"0.5,0,z"},
    wyckInfo{'c',8,"x,x+0.5,z"},
    wyckInfo{'d',16,"x,y,z"}
  },

  { // 109
    wyckInfo{'a',4,"0,0,z"},
    wyckInfo{'b',8,"0,y,z"},
    wyckInfo{'c',16,"x,y,z"}
  },

  { // 110
    wyckInfo{'a',8,"0,0,z"},
    wyckInfo{'b',16,"x,y,z"}
  },

  { // 111
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0.5,0.5,0.5"},
    wyckInfo{'c',1,"0,0,0.5"},
    wyckInfo{'d',1,"0.5,0.5,0"},
    wyckInfo{'e',2,"0.5,0,0"},
    wyckInfo{'f',2,"0.5,0,0.5"},
    wyckInfo{'g',2,"0,0,z"},
    wyckInfo{'h',2,"0.5,0.5,z"},
    wyckInfo{'i',4,"x,0,0"},
    wyckInfo{'j',4,"x,0.5,0.5"},
    wyckInfo{'k',4,"x,0,0.5"},
    wyckInfo{'l',4,"x,0.5,0"},
    wyckInfo{'m',4,"0,0.5,z"},
    wyckInfo{'n',4,"x,x,z"},
    wyckInfo{'o',8,"x,y,z"}
  },

  { // 112
    wyckInfo{'a',2,"0,0,0.25"},
    wyckInfo{'b',2,"0.5,0,0.25"},
    wyckInfo{'c',2,"0.5,0.5,0.25"},
    wyckInfo{'d',2,"0,0.5,0.25"},
    wyckInfo{'e',2,"0,0,0"},
    wyckInfo{'f',2,"0.5,0.5,0"},
    wyckInfo{'g',4,"x,0,0.25"},
    wyckInfo{'h',4,"0.5,y,0.25"},
    wyckInfo{'i',4,"x,0.5,0.25"},
    wyckInfo{'j',4,"0,y,0.25"},
    wyckInfo{'k',4,"0,0,z"},
    wyckInfo{'l',4,"0.5,0.5,z"},
    wyckInfo{'m',4,"0,0.5,z"},
    wyckInfo{'n',8,"x,y,z"}
  },

  { // 113
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',2,"0,0.5,z"},
    wyckInfo{'d',4,"0,0,z"},
    wyckInfo{'e',4,"x,x+0.5,z"},
    wyckInfo{'f',8,"x,y,z"}
  },

  { // 114
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',4,"0,0,z"},
    wyckInfo{'d',4,"0,0.5,z"},
    wyckInfo{'e',8,"x,y,z"}
  },

  { // 115
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0.5,0.5,0"},
    wyckInfo{'c',1,"0.5,0.5,0.5"},
    wyckInfo{'d',1,"0,0,0.5"},
    wyckInfo{'e',2,"0,0,z"},
    wyckInfo{'f',2,"0.5,0.5,z"},
    wyckInfo{'g',2,"0,0.5,z"},
    wyckInfo{'h',4,"x,x,0"},
    wyckInfo{'i',4,"x,x,0.5"},
    wyckInfo{'j',4,"x,0,z"},
    wyckInfo{'k',4,"x,0.5,z"},
    wyckInfo{'l',8,"x,y,z"}
  },

  { // 116
    wyckInfo{'a',2,"0,0,0.25"},
    wyckInfo{'b',2,"0.5,0.5,0.25"},
    wyckInfo{'c',2,"0,0,0"},
    wyckInfo{'d',2,"0.5,0.5,0"},
    wyckInfo{'e',4,"x,x,0.25"},
    wyckInfo{'f',4,"x,x,0.75"},
    wyckInfo{'g',4,"0,0,z"},
    wyckInfo{'h',4,"0.5,0.5,z"},
    wyckInfo{'i',4,"0,0.5,z"},
    wyckInfo{'j',8,"x,y,z"}
  },

  { // 117
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',2,"0,0.5,0"},
    wyckInfo{'d',2,"0,0.5,0.5"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0,0.5,z"},
    wyckInfo{'g',4,"x,x+0.5,0"},
    wyckInfo{'h',4,"x,x+0.5,0.5"},
    wyckInfo{'i',8,"x,y,z"}
  },

  { // 118
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',2,"0,0.5,0.25"},
    wyckInfo{'d',2,"0,0.5,0.75"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"x,-x+0.5,0.25"},
    wyckInfo{'g',4,"x,x+0.5,0.25"},
    wyckInfo{'h',4,"0,0.5,z"},
    wyckInfo{'i',8,"x,y,z"}
  },

  { // 119
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',2,"0,0.5,0.25"},
    wyckInfo{'d',2,"0,0.5,0.75"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0,0.5,z"},
    wyckInfo{'g',8,"x,x,0"},
    wyckInfo{'h',8,"x,x+0.5,0.25"},
    wyckInfo{'i',8,"x,0,z"},
    wyckInfo{'j',16,"x,y,z"}
  },

  { // 120
    wyckInfo{'a',4,"0,0,0.25"},
    wyckInfo{'b',4,"0,0,0"},
    wyckInfo{'c',4,"0,0.5,0.25"},
    wyckInfo{'d',4,"0,0.5,0"},
    wyckInfo{'e',8,"x,x,0.25"},
    wyckInfo{'f',8,"0,0,z"},
    wyckInfo{'g',8,"0,0.5,z"},
    wyckInfo{'h',8,"x,x+0.5,0"},
    wyckInfo{'i',16,"x,y,z"}
  },

  { // 121
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',4,"0,0.5,0"},
    wyckInfo{'d',4,"0,0.5,0.25"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',8,"x,0,0"},
    wyckInfo{'g',8,"x,0,0.5"},
    wyckInfo{'h',8,"0,0.5,z"},
    wyckInfo{'i',8,"x,x,z"},
    wyckInfo{'j',16,"x,y,z"}
  },

  { // 122
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0,0.5"},
    wyckInfo{'c',8,"0,0,z"},
    wyckInfo{'d',8,"x,0.25,0.125"},
    wyckInfo{'e',16,"x,y,z"}
  },

  { // 123
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',1,"0.5,0.5,0"},
    wyckInfo{'d',1,"0.5,0.5,0.5"},
    wyckInfo{'e',2,"0,0.5,0.5"},
    wyckInfo{'f',2,"0,0.5,0"},
    wyckInfo{'g',2,"0,0,z"},
    wyckInfo{'h',2,"0.5,0.5,z"},
    wyckInfo{'i',4,"0,0.5,z"},
    wyckInfo{'j',4,"x,x,0"},
    wyckInfo{'k',4,"x,x,0.5"},
    wyckInfo{'l',4,"x,0,0"},
    wyckInfo{'m',4,"x,0,0.5"},
    wyckInfo{'n',4,"x,0.5,0"},
    wyckInfo{'o',4,"x,0.5,0.5"},
    wyckInfo{'p',8,"x,y,0"},
    wyckInfo{'q',8,"x,y,0.5"},
    wyckInfo{'r',8,"x,x,z"},
    wyckInfo{'s',8,"x,0,z"},
    wyckInfo{'t',8,"x,0.5,z"},
    wyckInfo{'u',16,"x,y,z"}
  },

  { // 124
    wyckInfo{'a',2,"0,0,0.25"},
    wyckInfo{'b',2,"0,0,0"},
    wyckInfo{'c',2,"0.5,0.5,0.25"},
    wyckInfo{'d',2,"0.5,0.5,0"},
    wyckInfo{'e',4,"0,0.5,0"},
    wyckInfo{'f',4,"0,0.5,0.25"},
    wyckInfo{'g',4,"0,0,z"},
    wyckInfo{'h',4,"0.5,0.5,z"},
    wyckInfo{'i',8,"0,0.5,z"},
    wyckInfo{'j',8,"x,x,0.25"},
    wyckInfo{'k',8,"x,0,0.25"},
    wyckInfo{'l',8,"x,0.5,0.25"},
    wyckInfo{'m',8,"x,y,0"},
    wyckInfo{'n',16,"x,y,z"}
  },

  { // 125
    wyckInfo{'a',2,"0.25,0.25,0"},
    wyckInfo{'b',2,"0.25,0.25,0.5"},
    wyckInfo{'c',2,"0.75,0.25,0"},
    wyckInfo{'d',2,"0.75,0.25,0.5"},
    wyckInfo{'e',4,"0,0,0"},
    wyckInfo{'f',4,"0,0,0.5"},
    wyckInfo{'g',4,"0.25,0.25,z"},
    wyckInfo{'h',4,"0.75,0.25,z"},
    wyckInfo{'i',8,"x,x,0"},
    wyckInfo{'j',8,"x,x,0.5"},
    wyckInfo{'k',8,"x,0.25,0"},
    wyckInfo{'l',8,"x,0.25,0.5"},
    wyckInfo{'m',8,"x,-x,z"},
    wyckInfo{'n',16,"x,y,z"}
  },

  { // 126
    wyckInfo{'a',2,"0.25,0.25,0.25"},
    wyckInfo{'b',2,"0.25,0.25,0.75"},
    wyckInfo{'c',4,"0.25,0.75,0.75"},
    wyckInfo{'d',4,"0.25,0.75,0"},
    wyckInfo{'e',4,"0.25,0.25,z"},
    wyckInfo{'f',8,"0,0,0"},
    wyckInfo{'g',8,"0.25,0.75,z"},
    wyckInfo{'h',8,"x,x,0.25"},
    wyckInfo{'i',8,"x,0.25,0.25"},
    wyckInfo{'j',8,"x,0.75,0.25"},
    wyckInfo{'k',16,"x,y,z"}
  },

  { // 127
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',2,"0,0.5,0.5"},
    wyckInfo{'d',2,"0,0.5,0"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0,0.5,z"},
    wyckInfo{'g',4,"x,x+0.5,0"},
    wyckInfo{'h',4,"x,x+0.5,0.5"},
    wyckInfo{'i',8,"x,y,0"},
    wyckInfo{'j',8,"x,y,0.5"},
    wyckInfo{'k',8,"x,x+0.5,z"},
    wyckInfo{'l',16,"x,y,z"}
  },

  { // 128
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',4,"0,0.5,0"},
    wyckInfo{'d',4,"0,0.5,0.25"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',8,"0,0.5,z"},
    wyckInfo{'g',8,"x,x+0.5,0.25"},
    wyckInfo{'h',8,"x,y,0"},
    wyckInfo{'i',16,"x,y,z"}
  },

  { // 129
    wyckInfo{'a',2,"0.75,0.25,0"},
    wyckInfo{'b',2,"0.75,0.25,0.5"},
    wyckInfo{'c',2,"0.25,0.25,z"},
    wyckInfo{'d',4,"0,0,0"},
    wyckInfo{'e',4,"0,0,0.5"},
    wyckInfo{'f',4,"0.75,0.25,z"},
    wyckInfo{'g',8,"x,-x,0"},
    wyckInfo{'h',8,"x,-x,0.5"},
    wyckInfo{'i',8,"0.25,y,z"},
    wyckInfo{'j',8,"x,x,z"},
    wyckInfo{'k',16,"x,y,z"}
  },

  { // 130
    wyckInfo{'a',4,"0.75,0.25,0.25"},
    wyckInfo{'b',4,"0.75,0.25,0"},
    wyckInfo{'c',4,"0.25,0.25,z"},
    wyckInfo{'d',8,"0,0,0"},
    wyckInfo{'e',8,"0.75,0.25,z"},
    wyckInfo{'f',8,"x,-x,0.25"},
    wyckInfo{'g',16,"x,y,z"}
  },

  { // 131
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0.5,0.5,0"},
    wyckInfo{'c',2,"0,0.5,0"},
    wyckInfo{'d',2,"0,0.5,0.5"},
    wyckInfo{'e',2,"0,0,0.25"},
    wyckInfo{'f',2,"0.5,0.5,0.25"},
    wyckInfo{'g',4,"0,0,z"},
    wyckInfo{'h',4,"0.5,0.5,z"},
    wyckInfo{'i',4,"0,0.5,z"},
    wyckInfo{'j',4,"x,0,0"},
    wyckInfo{'k',4,"x,0.5,0.5"},
    wyckInfo{'l',4,"x,0,0.5"},
    wyckInfo{'m',4,"x,0.5,0"},
    wyckInfo{'n',8,"x,x,0.25"},
    wyckInfo{'o',8,"0,y,z"},
    wyckInfo{'p',8,"0.5,y,z"},
    wyckInfo{'q',8,"x,y,0"},
    wyckInfo{'r',16,"x,y,z"}
  },

  { // 132
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.25"},
    wyckInfo{'c',2,"0.5,0.5,0"},
    wyckInfo{'d',2,"0.5,0.5,0.25"},
    wyckInfo{'e',4,"0,0.5,0.25"},
    wyckInfo{'f',4,"0,0.5,0"},
    wyckInfo{'g',4,"0,0,z"},
    wyckInfo{'h',4,"0.5,0.5,z"},
    wyckInfo{'i',4,"x,x,0"},
    wyckInfo{'j',4,"x,x,0.5"},
    wyckInfo{'k',8,"0,0.5,z"},
    wyckInfo{'l',8,"x,0,0.25"},
    wyckInfo{'m',8,"x,0.5,0.25"},
    wyckInfo{'n',8,"x,y,0"},
    wyckInfo{'o',8,"x,x,z"},
    wyckInfo{'p',16,"x,y,z"}
  },

  { // 133
    wyckInfo{'a',4,"0.25,0.25,0"},
    wyckInfo{'b',4,"0.75,0.25,0"},
    wyckInfo{'c',4,"0.25,0.25,0.25"},
    wyckInfo{'d',4,"0.75,0.25,0.75"},
    wyckInfo{'e',8,"0,0,0"},
    wyckInfo{'f',8,"0.25,0.25,z"},
    wyckInfo{'g',8,"0.75,0.25,z"},
    wyckInfo{'h',8,"x,0.25,0"},
    wyckInfo{'i',8,"x,0.25,0.5"},
    wyckInfo{'j',8,"x,x,0.25"},
    wyckInfo{'k',16,"x,y,z"}
  },

  { // 134
    wyckInfo{'a',2,"0.25,0.75,0.25"},
    wyckInfo{'b',2,"0.75,0.25,0.25"},
    wyckInfo{'c',4,"0.25,0.25,0.25"},
    wyckInfo{'d',4,"0.25,0.25,0"},
    wyckInfo{'e',4,"0,0,0.5"},
    wyckInfo{'f',4,"0,0,0"},
    wyckInfo{'g',4,"0.75,0.25,z"},
    wyckInfo{'h',8,"0.25,0.25,z"},
    wyckInfo{'i',8,"x,0.25,0.75"},
    wyckInfo{'j',8,"x,0.25,0.25"},
    wyckInfo{'k',8,"x,x,0"},
    wyckInfo{'l',8,"x,x,0.5"},
    wyckInfo{'m',8,"x,-x,z"},
    wyckInfo{'n',16,"x,y,z"}
  },

  { // 135
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0,0,0.25"},
    wyckInfo{'c',4,"0,0.5,0"},
    wyckInfo{'d',4,"0,0.5,0.25"},
    wyckInfo{'e',8,"0,0,z"},
    wyckInfo{'f',8,"0,0.5,z"},
    wyckInfo{'g',8,"x,x+0.5,0.25"},
    wyckInfo{'h',8,"x,y,0"},
    wyckInfo{'i',16,"x,y,z"}
  },

  { // 136
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',4,"0,0.5,0"},
    wyckInfo{'d',4,"0,0.5,0.25"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"x,x,0"},
    wyckInfo{'g',4,"x,-x,0"},
    wyckInfo{'h',8,"0,0.5,z"},
    wyckInfo{'i',8,"x,y,0"},
    wyckInfo{'j',8,"x,x,z"},
    wyckInfo{'k',16,"x,y,z"}
  },

  { // 137
    wyckInfo{'a',2,"0.75,0.25,0.75"},
    wyckInfo{'b',2,"0.75,0.25,0.25"},
    wyckInfo{'c',4,"0.75,0.25,z"},
    wyckInfo{'d',4,"0.25,0.25,z"},
    wyckInfo{'e',8,"0,0,0"},
    wyckInfo{'f',8,"x,-x,0.25"},
    wyckInfo{'g',8,"0.25,y,z"},
    wyckInfo{'h',16,"x,y,z"}
  },

  { // 138
    wyckInfo{'a',4,"0.75,0.25,0"},
    wyckInfo{'b',4,"0.75,0.25,0.75"},
    wyckInfo{'c',4,"0,0,0.5"},
    wyckInfo{'d',4,"0,0,0"},
    wyckInfo{'e',4,"0.25,0.25,z"},
    wyckInfo{'f',8,"0.75,0.25,z"},
    wyckInfo{'g',8,"x,-x,0.5"},
    wyckInfo{'h',8,"x,-x,0"},
    wyckInfo{'i',8,"x,x,z"},
    wyckInfo{'j',16,"x,y,z"}
  },

  { // 139
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.5"},
    wyckInfo{'c',4,"0,0.5,0"},
    wyckInfo{'d',4,"0,0.5,0.25"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',8,"0.25,0.25,0.25"},
    wyckInfo{'g',8,"0,0.5,z"},
    wyckInfo{'h',8,"x,x,0"},
    wyckInfo{'i',8,"x,0,0"},
    wyckInfo{'j',8,"x,0.5,0"},
    wyckInfo{'k',16,"x,x+0.5,0.25"},
    wyckInfo{'l',16,"x,y,0"},
    wyckInfo{'m',16,"x,x,z"},
    wyckInfo{'n',16,"0,y,z"},
    wyckInfo{'o',32,"x,y,z"}
  },

  { // 140
    wyckInfo{'a',4,"0,0,0.25"},
    wyckInfo{'b',4,"0,0.5,0.25"},
    wyckInfo{'c',4,"0,0,0"},
    wyckInfo{'d',4,"0,0.5,0"},
    wyckInfo{'e',8,"0.25,0.25,0.25"},
    wyckInfo{'f',8,"0,0,z"},
    wyckInfo{'g',8,"0,0.5,z"},
    wyckInfo{'h',8,"x,x+0.5,0"},
    wyckInfo{'i',16,"x,x,0.25"},
    wyckInfo{'j',16,"x,0,0.25"},
    wyckInfo{'k',16,"x,y,0"},
    wyckInfo{'l',16,"x,x+0.5,z"},
    wyckInfo{'m',32,"x,y,z"}
  },

  { // 141
    wyckInfo{'a',4,"0,0.75,0.125"},
    wyckInfo{'b',4,"0,0.25,0.375"},
    wyckInfo{'c',8,"0,0,0"},
    wyckInfo{'d',8,"0,0,0.5"},
    wyckInfo{'e',8,"0,0.25,z"},
    wyckInfo{'f',16,"x,0,0"},
    wyckInfo{'g',16,"x,x+0.25,0.875"},
    wyckInfo{'h',16,"0,y,z"},
    wyckInfo{'i',32,"x,y,z"}
  },

  { // 142
    wyckInfo{'a',8,"0,0.25,0.375"},
    wyckInfo{'b',8,"0,0.25,0.125"},
    wyckInfo{'c',16,"0,0,0"},
    wyckInfo{'d',16,"0,0.25,z"},
    wyckInfo{'e',16,"x,0,0.25"},
    wyckInfo{'f',16,"x,x+0.25,0.125"},
    wyckInfo{'g',32,"x,y,z"}
  },

  { // 143
    wyckInfo{'a',1,"0,0,z"},
    wyckInfo{'b',1,"0.333333,0.666667,z"},
    wyckInfo{'c',1,"0.666667,0.333333,z"},
    wyckInfo{'d',3,"x,y,z"}
  },

  { // 144
    wyckInfo{'a',3,"x,y,z"}
  },

  { // 145
    wyckInfo{'a',3,"x,y,z"}
  },

  { // 146
    wyckInfo{'a',3,"0,0,z"},
    wyckInfo{'b',9,"x,y,z"}
  },

  { // 147
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',2,"0,0,z"},
    wyckInfo{'d',2,"0.333333,0.666667,z"},
    wyckInfo{'e',3,"0.5,0,0"},
    wyckInfo{'f',3,"0.5,0,0.5"},
    wyckInfo{'g',6,"x,y,z"}
  },

  { // 148
    wyckInfo{'a',3,"0,0,0"},
    wyckInfo{'b',3,"0,0,0.5"},
    wyckInfo{'c',6,"0,0,z"},
    wyckInfo{'d',9,"0.5,0,0.5"},
    wyckInfo{'e',9,"0.5,0,0"},
    wyckInfo{'f',18,"x,y,z"}
  },

  { // 149
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',1,"0.333333,0.666667,0"},
    wyckInfo{'d',1,"0.333333,0.666667,0.5"},
    wyckInfo{'e',1,"0.666667,0.333333,0"},
    wyckInfo{'f',1,"0.666667,0.333333,0.5"},
    wyckInfo{'g',2,"0,0,z"},
    wyckInfo{'h',2,"0.333333,0.666667,z"},
    wyckInfo{'i',2,"0.666667,0.333333,z"},
    wyckInfo{'j',3,"x,-x,0"},
    wyckInfo{'k',3,"x,-x,0.5"},
    wyckInfo{'l',6,"x,y,z"}
  },

  { // 150
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',2,"0,0,z"},
    wyckInfo{'d',2,"0.333333,0.666667,z"},
    wyckInfo{'e',3,"x,0,0"},
    wyckInfo{'f',3,"x,0,0.5"},
    wyckInfo{'g',6,"x,y,z"}
  },

  { // 151
    wyckInfo{'a',3,"x,-x,0.333333"},
    wyckInfo{'b',3,"x,-x,0.833333"},
    wyckInfo{'c',6,"x,y,z"}
  },

  { // 152
    wyckInfo{'a',3,"x,0,0.333333"},
    wyckInfo{'b',3,"x,0,0.833333"},
    wyckInfo{'c',6,"x,y,z"}
  },

  { // 153
    wyckInfo{'a',3,"x,-x,0.666667"},
    wyckInfo{'b',3,"x,-x,0.166667"},
    wyckInfo{'c',6,"x,y,z"}
  },

  { // 154
    wyckInfo{'a',3,"x,0,0.666667"},
    wyckInfo{'b',3,"x,0,0.166667"},
    wyckInfo{'c',6,"x,y,z"}
  },

  { // 155
    wyckInfo{'a',3,"0,0,0"},
    wyckInfo{'b',3,"0,0,0.5"},
    wyckInfo{'c',6,"0,0,z"},
    wyckInfo{'d',9,"x,0,0"},
    wyckInfo{'e',9,"x,0,0.5"},
    wyckInfo{'f',18,"x,y,z"}
  },

  { // 156
    wyckInfo{'a',1,"0,0,z"},
    wyckInfo{'b',1,"0.333333,0.666667,z"},
    wyckInfo{'c',1,"0.666667,0.333333,z"},
    wyckInfo{'d',3,"x,-x,z"},
    wyckInfo{'e',6,"x,y,z"}
  },

  { // 157
    wyckInfo{'a',1,"0,0,z"},
    wyckInfo{'b',2,"0.333333,0.666667,z"},
    wyckInfo{'c',3,"x,0,z"},
    wyckInfo{'d',6,"x,y,z"}
  },

  { // 158
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.333333,0.666667,z"},
    wyckInfo{'c',2,"0.666667,0.333333,z"},
    wyckInfo{'d',6,"x,y,z"}
  },

  { // 159
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.333333,0.666667,z"},
    wyckInfo{'c',6,"x,y,z"}
  },

  { // 160
    wyckInfo{'a',3,"0,0,z"},
    wyckInfo{'b',9,"x,-x,z"},
    wyckInfo{'c',18,"x,y,z"}
  },

  { // 161
    wyckInfo{'a',6,"0,0,z"},
    wyckInfo{'b',18,"x,y,z"}
  },

  { // 162
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',2,"0.333333,0.666667,0"},
    wyckInfo{'d',2,"0.333333,0.666667,0.5"},
    wyckInfo{'e',2,"0,0,z"},
    wyckInfo{'f',3,"0.5,0,0"},
    wyckInfo{'g',3,"0.5,0,0.5"},
    wyckInfo{'h',4,"0.333333,0.666667,z"},
    wyckInfo{'i',6,"x,-x,0"},
    wyckInfo{'j',6,"x,-x,0.5"},
    wyckInfo{'k',6,"x,0,z"},
    wyckInfo{'l',12,"x,y,z"}
  },

  { // 163
    wyckInfo{'a',2,"0,0,0.25"},
    wyckInfo{'b',2,"0,0,0"},
    wyckInfo{'c',2,"0.333333,0.666667,0.25"},
    wyckInfo{'d',2,"0.666667,0.333333,0.25"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0.333333,0.666667,z"},
    wyckInfo{'g',6,"0.5,0,0"},
    wyckInfo{'h',6,"x,-x,0.25"},
    wyckInfo{'i',12,"x,y,z"}
  },

  { // 164
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',2,"0,0,z"},
    wyckInfo{'d',2,"0.333333,0.666667,z"},
    wyckInfo{'e',3,"0.5,0,0"},
    wyckInfo{'f',3,"0.5,0,0.5"},
    wyckInfo{'g',6,"x,0,0"},
    wyckInfo{'h',6,"x,0,0.5"},
    wyckInfo{'i',6,"x,-x,z"},
    wyckInfo{'j',12,"x,y,z"}
  },

  { // 165
    wyckInfo{'a',2,"0,0,0.25"},
    wyckInfo{'b',2,"0,0,0"},
    wyckInfo{'c',4,"0,0,z"},
    wyckInfo{'d',4,"0.333333,0.666667,z"},
    wyckInfo{'e',6,"0.5,0,0"},
    wyckInfo{'f',6,"x,0,0.25"},
    wyckInfo{'g',12,"x,y,z"}
  },

  { // 166
    wyckInfo{'a',3,"0,0,0"},
    wyckInfo{'b',3,"0,0,0.5"},
    wyckInfo{'c',6,"0,0,z"},
    wyckInfo{'d',9,"0.5,0,0.5"},
    wyckInfo{'e',9,"0.5,0,0"},
    wyckInfo{'f',18,"x,0,0"},
    wyckInfo{'g',18,"x,0,0.5"},
    wyckInfo{'h',18,"x,-x,z"},
    wyckInfo{'i',36,"x,y,z"}
  },

  { // 167
    wyckInfo{'a',6,"0,0,0.25"},
    wyckInfo{'b',6,"0,0,0"},
    wyckInfo{'c',12,"0,0,z"},
    wyckInfo{'d',18,"0.5,0,0"},
    wyckInfo{'e',18,"x,0,0.25"},
    wyckInfo{'f',36,"x,y,z"}
  },

  { // 168
    wyckInfo{'a',1,"0,0,z"},
    wyckInfo{'b',2,"0.333333,0.666667,z"},
    wyckInfo{'c',3,"0.5,0,z"},
    wyckInfo{'d',6,"x,y,z"}
  },

  { // 169
    wyckInfo{'a',6,"x,y,z"}
  },

  { // 170
    wyckInfo{'a',6,"x,y,z"}
  },

  { // 171
    wyckInfo{'a',3,"0,0,z"},
    wyckInfo{'b',3,"0.5,0.5,z"},
    wyckInfo{'c',6,"x,y,z"}
  },

  { // 172
    wyckInfo{'a',3,"0,0,z"},
    wyckInfo{'b',3,"0.5,0.5,z"},
    wyckInfo{'c',6,"x,y,z"}
  },

  { // 173
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.333333,0.666667,z"},
    wyckInfo{'c',6,"x,y,z"}
  },

  { // 174
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',1,"0.333333,0.666667,0"},
    wyckInfo{'d',1,"0.333333,0.666667,0.5"},
    wyckInfo{'e',1,"0.666667,0.333333,0"},
    wyckInfo{'f',1,"0.666667,0.333333,0.5"},
    wyckInfo{'g',2,"0,0,z"},
    wyckInfo{'h',2,"0.333333,0.666667,z"},
    wyckInfo{'i',2,"0.666667,0.333333,z"},
    wyckInfo{'j',3,"x,y,0"},
    wyckInfo{'k',3,"x,y,0.5"},
    wyckInfo{'l',6,"x,y,z"}
  },

  { // 175
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',2,"0.333333,0.666667,0"},
    wyckInfo{'d',2,"0.333333,0.666667,0.5"},
    wyckInfo{'e',2,"0,0,z"},
    wyckInfo{'f',3,"0.5,0,0"},
    wyckInfo{'g',3,"0.5,0,0.5"},
    wyckInfo{'h',4,"0.333333,0.666667,z"},
    wyckInfo{'i',6,"0.5,0,z"},
    wyckInfo{'j',6,"x,y,0"},
    wyckInfo{'k',6,"x,y,0.5"},
    wyckInfo{'l',12,"x,y,z"}
  },

  { // 176
    wyckInfo{'a',2,"0,0,0.25"},
    wyckInfo{'b',2,"0,0,0"},
    wyckInfo{'c',2,"0.333333,0.666667,0.25"},
    wyckInfo{'d',2,"0.666667,0.333333,0.25"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0.333333,0.666667,z"},
    wyckInfo{'g',6,"0.5,0,0"},
    wyckInfo{'h',6,"x,y,0.25"},
    wyckInfo{'i',12,"x,y,z"}
  },

  { // 177
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',2,"0.333333,0.666667,0"},
    wyckInfo{'d',2,"0.333333,0.666667,0.5"},
    wyckInfo{'e',2,"0,0,z"},
    wyckInfo{'f',3,"0.5,0,0"},
    wyckInfo{'g',3,"0.5,0,0.5"},
    wyckInfo{'h',4,"0.333333,0.666667,z"},
    wyckInfo{'i',6,"0.5,0,z"},
    wyckInfo{'j',6,"x,0,0"},
    wyckInfo{'k',6,"x,0,0.5"},
    wyckInfo{'l',6,"x,-x,0"},
    wyckInfo{'m',6,"x,-x,0.5"},
    wyckInfo{'n',12,"x,y,z"}
  },

  { // 178
    wyckInfo{'a',6,"x,0,0"},
    wyckInfo{'b',6,"x,2x,0.25"},
    wyckInfo{'c',12,"x,y,z"}
  },

  { // 179
    wyckInfo{'a',6,"x,0,0"},
    wyckInfo{'b',6,"x,2x,0.75"},
    wyckInfo{'c',12,"x,y,z"}
  },

  { // 180
    wyckInfo{'a',3,"0,0,0"},
    wyckInfo{'b',3,"0,0,0.5"},
    wyckInfo{'c',3,"0.5,0,0"},
    wyckInfo{'d',3,"0.5,0,0.5"},
    wyckInfo{'e',6,"0,0,z"},
    wyckInfo{'f',6,"0.5,0,z"},
    wyckInfo{'g',6,"x,0,0"},
    wyckInfo{'h',6,"x,0,0.5"},
    wyckInfo{'i',6,"x,2x,0"},
    wyckInfo{'j',6,"x,2x,0.5"},
    wyckInfo{'k',12,"x,y,z"}
  },

  { // 181
    wyckInfo{'a',3,"0,0,0"},
    wyckInfo{'b',3,"0,0,0.5"},
    wyckInfo{'c',3,"0.5,0,0"},
    wyckInfo{'d',3,"0.5,0,0.5"},
    wyckInfo{'e',6,"0,0,z"},
    wyckInfo{'f',6,"0.5,0,z"},
    wyckInfo{'g',6,"x,0,0"},
    wyckInfo{'h',6,"x,0,0.5"},
    wyckInfo{'i',6,"x,2x,0"},
    wyckInfo{'j',6,"x,2x,0.5"},
    wyckInfo{'k',12,"x,y,z"}
  },

  { // 182
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.25"},
    wyckInfo{'c',2,"0.333333,0.666667,0.25"},
    wyckInfo{'d',2,"0.333333,0.666667,0.75"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0.333333,0.666667,z"},
    wyckInfo{'g',6,"x,0,0"},
    wyckInfo{'h',6,"x,2x,0.25"},
    wyckInfo{'i',12,"x,y,z"}
  },

  { // 183
    wyckInfo{'a',1,"0,0,z"},
    wyckInfo{'b',2,"0.333333,0.666667,z"},
    wyckInfo{'c',3,"0.5,0,z"},
    wyckInfo{'d',6,"x,0,z"},
    wyckInfo{'e',6,"x,-x,z"},
    wyckInfo{'f',12,"x,y,z"}
  },

  { // 184
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',4,"0.333333,0.666667,z"},
    wyckInfo{'c',6,"0.5,0,z"},
    wyckInfo{'d',12,"x,y,z"}
  },

  { // 185
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',4,"0.333333,0.666667,z"},
    wyckInfo{'c',6,"x,0,z"},
    wyckInfo{'d',12,"x,y,z"}
  },

  { // 186
    wyckInfo{'a',2,"0,0,z"},
    wyckInfo{'b',2,"0.333333,0.666667,z"},
    wyckInfo{'c',6,"x,-x,z"},
    wyckInfo{'d',12,"x,y,z"}
  },

  { // 187
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',1,"0.333333,0.666667,0"},
    wyckInfo{'d',1,"0.333333,0.666667,0.5"},
    wyckInfo{'e',1,"0.666667,0.333333,0"},
    wyckInfo{'f',1,"0.666667,0.333333,0.5"},
    wyckInfo{'g',2,"0,0,z"},
    wyckInfo{'h',2,"0.333333,0.666667,z"},
    wyckInfo{'i',2,"0.666667,0.333333,z"},
    wyckInfo{'j',3,"x,-x,0"},
    wyckInfo{'k',3,"x,-x,0.5"},
    wyckInfo{'l',6,"x,y,0"},
    wyckInfo{'m',6,"x,y,0.5"},
    wyckInfo{'n',6,"x,-x,z"},
    wyckInfo{'o',12,"x,y,z"}
  },

  { // 188
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.25"},
    wyckInfo{'c',2,"0.333333,0.666667,0"},
    wyckInfo{'d',2,"0.333333,0.666667,0.25"},
    wyckInfo{'e',2,"0.666667,0.333333,0"},
    wyckInfo{'f',2,"0.666667,0.333333,0.25"},
    wyckInfo{'g',4,"0,0,z"},
    wyckInfo{'h',4,"0.333333,0.666667,z"},
    wyckInfo{'i',4,"0.666667,0.333333,z"},
    wyckInfo{'j',6,"x,-x,0"},
    wyckInfo{'k',6,"x,y,0.25"},
    wyckInfo{'l',12,"x,y,z"}
  },

  { // 189
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',2,"0.333333,0.666667,0"},
    wyckInfo{'d',2,"0.333333,0.666667,0.5"},
    wyckInfo{'e',2,"0,0,z"},
    wyckInfo{'f',3,"x,0,0"},
    wyckInfo{'g',3,"x,0,0.5"},
    wyckInfo{'h',4,"0.333333,0.666667,z"},
    wyckInfo{'i',6,"x,0,z"},
    wyckInfo{'j',6,"x,y,0"},
    wyckInfo{'k',6,"x,y,0.5"},
    wyckInfo{'l',12,"x,y,z"}
  },

  { // 190
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.25"},
    wyckInfo{'c',2,"0.333333,0.666667,0.25"},
    wyckInfo{'d',2,"0.666667,0.333333,0.25"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0.333333,0.666667,z"},
    wyckInfo{'g',6,"x,0,0"},
    wyckInfo{'h',6,"x,y,0.25"},
    wyckInfo{'i',12,"x,y,z"}
  },

  { // 191
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0,0,0.5"},
    wyckInfo{'c',2,"0.333333,0.666667,0"},
    wyckInfo{'d',2,"0.333333,0.666667,0.5"},
    wyckInfo{'e',2,"0,0,z"},
    wyckInfo{'f',3,"0.5,0,0"},
    wyckInfo{'g',3,"0.5,0,0.5"},
    wyckInfo{'h',4,"0.333333,0.666667,z"},
    wyckInfo{'i',6,"0.5,0,z"},
    wyckInfo{'j',6,"x,0,0"},
    wyckInfo{'k',6,"x,0,0.5"},
    wyckInfo{'l',6,"x,2x,0"},
    wyckInfo{'m',6,"x,2x,0.5"},
    wyckInfo{'n',12,"x,0,z"},
    wyckInfo{'o',12,"x,2x,z"},
    wyckInfo{'p',12,"x,y,0"},
    wyckInfo{'q',12,"x,y,0.5"},
    wyckInfo{'r',24,"x,y,z"}
  },

  { // 192
    wyckInfo{'a',2,"0,0,0.25"},
    wyckInfo{'b',2,"0,0,0"},
    wyckInfo{'c',4,"0.333333,0.666667,0.25"},
    wyckInfo{'d',4,"0.333333,0.666667,0"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',6,"0.5,0,0.25"},
    wyckInfo{'g',6,"0.5,0,0"},
    wyckInfo{'h',8,"0.333333,0.666667,z"},
    wyckInfo{'i',12,"0.5,0,z"},
    wyckInfo{'j',12,"x,0,0.25"},
    wyckInfo{'k',12,"x,2x,0.25"},
    wyckInfo{'l',12,"x,y,0"},
    wyckInfo{'m',24,"x,y,z"}
  },

  { // 193
    wyckInfo{'a',2,"0,0,0.25"},
    wyckInfo{'b',2,"0,0,0"},
    wyckInfo{'c',4,"0.333333,0.666667,0.25"},
    wyckInfo{'d',4,"0.333333,0.666667,0"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',6,"0.5,0,0"},
    wyckInfo{'g',6,"x,0,0.25"},
    wyckInfo{'h',8,"0.333333,0.666667,z"},
    wyckInfo{'i',12,"x,2x,0"},
    wyckInfo{'j',12,"x,y,0.25"},
    wyckInfo{'k',12,"x,0,z"},
    wyckInfo{'l',24,"x,y,z"}
  },

  { // 194
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',2,"0,0,0.25"},
    wyckInfo{'c',2,"0.333333,0.666667,0.25"},
    wyckInfo{'d',2,"0.333333,0.666667,0.75"},
    wyckInfo{'e',4,"0,0,z"},
    wyckInfo{'f',4,"0.333333,0.666667,z"},
    wyckInfo{'g',6,"0.5,0,0"},
    wyckInfo{'h',6,"x,2x,0.25"},
    wyckInfo{'i',12,"x,0,0"},
    wyckInfo{'j',12,"x,y,0.25"},
    wyckInfo{'k',12,"x,2x,z"},
    wyckInfo{'l',24,"x,y,z"}
  },

  { // 195
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0.5,0.5,0.5"},
    wyckInfo{'c',3,"0,0.5,0.5"},
    wyckInfo{'d',3,"0.5,0,0"},
    wyckInfo{'e',4,"x,x,x"},
    wyckInfo{'f',6,"x,0,0"},
    wyckInfo{'g',6,"x,0,0.5"},
    wyckInfo{'h',6,"x,0.5,0"},
    wyckInfo{'i',6,"x,0.5,0.5"},
    wyckInfo{'j',12,"x,y,z"}
  },

  { // 196
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0.5,0.5,0.5"},
    wyckInfo{'c',4,"0.25,0.25,0.25"},
    wyckInfo{'d',4,"0.75,0.75,0.75"},
    wyckInfo{'e',16,"x,x,x"},
    wyckInfo{'f',24,"x,0,0"},
    wyckInfo{'g',24,"x,0.25,0.25"},
    wyckInfo{'h',48,"x,y,z"}
  },

  { // 197
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',6,"0,0.5,0.5"},
    wyckInfo{'c',8,"x,x,x"},
    wyckInfo{'d',12,"x,0,0"},
    wyckInfo{'e',12,"x,0.5,0"},
    wyckInfo{'f',24,"x,y,z"}
  },

  { // 198
    wyckInfo{'a',4,"x,x,x"},
    wyckInfo{'b',12,"x,y,z"}
  },

  { // 199
    wyckInfo{'a',8,"x,x,x"},
    wyckInfo{'b',12,"x,0,0.25"},
    wyckInfo{'c',24,"x,y,z"}
  },

  { // 200
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0.5,0.5,0.5"},
    wyckInfo{'c',3,"0,0.5,0.5"},
    wyckInfo{'d',3,"0.5,0,0"},
    wyckInfo{'e',6,"x,0,0"},
    wyckInfo{'f',6,"x,0,0.5"},
    wyckInfo{'g',6,"x,0.5,0"},
    wyckInfo{'h',6,"x,0.5,0.5"},
    wyckInfo{'i',8,"x,x,x"},
    wyckInfo{'j',12,"0,y,z"},
    wyckInfo{'k',12,"0.5,y,z"},
    wyckInfo{'l',24,"x,y,z"}
  },

  { // 201
    wyckInfo{'a',2,"0.25,0.25,0.25"},
    wyckInfo{'b',4,"0,0,0"},
    wyckInfo{'c',4,"0.5,0.5,0.5"},
    wyckInfo{'d',6,"0.25,0.75,0.75"},
    wyckInfo{'e',8,"x,x,x"},
    wyckInfo{'f',12,"x,0.25,0.25"},
    wyckInfo{'g',12,"x,0.75,0.25"},
    wyckInfo{'h',24,"x,y,z"}
  },

  { // 202
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0.5,0.5,0.5"},
    wyckInfo{'c',8,"0.25,0.25,0.25"},
    wyckInfo{'d',24,"0,0.25,0.25"},
    wyckInfo{'e',24,"x,0,0"},
    wyckInfo{'f',32,"x,x,x"},
    wyckInfo{'g',48,"x,0.25,0.25"},
    wyckInfo{'h',48,"0,y,z"},
    wyckInfo{'i',96,"x,y,z"}
  },

  { // 203
    wyckInfo{'a',8,"0.125,0.125,0.125"},
    wyckInfo{'b',8,"0.625,0.625,0.625"},
    wyckInfo{'c',16,"0,0,0"},
    wyckInfo{'d',16,"0.5,0.5,0.5"},
    wyckInfo{'e',32,"x,x,x"},
    wyckInfo{'f',48,"x,0.125,0.125"},
    wyckInfo{'g',96,"x,y,z"}
  },

  { // 204
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',6,"0,0.5,0.5"},
    wyckInfo{'c',8,"0.25,0.25,0.25"},
    wyckInfo{'d',12,"x,0,0"},
    wyckInfo{'e',12,"x,0,0.5"},
    wyckInfo{'f',16,"x,x,x"},
    wyckInfo{'g',24,"0,y,z"},
    wyckInfo{'h',48,"x,y,z"}
  },

  { // 205
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0.5,0.5,0.5"},
    wyckInfo{'c',8,"x,x,x"},
    wyckInfo{'d',24,"x,y,z"}
  },

  { // 206
    wyckInfo{'a',8,"0,0,0"},
    wyckInfo{'b',8,"0.25,0.25,0.25"},
    wyckInfo{'c',16,"x,x,x"},
    wyckInfo{'d',24,"x,0,0.25"},
    wyckInfo{'e',48,"x,y,z"}
  },

  { // 207
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0.5,0.5,0.5"},
    wyckInfo{'c',3,"0,0.5,0.5"},
    wyckInfo{'d',3,"0.5,0,0"},
    wyckInfo{'e',6,"x,0,0"},
    wyckInfo{'f',6,"x,0.5,0.5"},
    wyckInfo{'g',8,"x,x,x"},
    wyckInfo{'h',12,"x,0.5,0"},
    wyckInfo{'i',12,"0,y,y"},
    wyckInfo{'j',12,"0.5,y,y"},
    wyckInfo{'k',24,"x,y,z"}
  },

  { // 208
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',4,"0.25,0.25,0.25"},
    wyckInfo{'c',4,"0.75,0.75,0.75"},
    wyckInfo{'d',6,"0,0.5,0.5"},
    wyckInfo{'e',6,"0.25,0,0.5"},
    wyckInfo{'f',6,"0.25,0.5,0"},
    wyckInfo{'g',8,"x,x,x"},
    wyckInfo{'h',12,"x,0,0"},
    wyckInfo{'i',12,"x,0,0.5"},
    wyckInfo{'j',12,"x,0.5,0"},
    wyckInfo{'k',12,"0.25,y,-y+0.5"},
    wyckInfo{'l',12,"0.25,y,y+0.5"},
    wyckInfo{'m',24,"x,y,z"}
  },

  { // 209
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0.5,0.5,0.5"},
    wyckInfo{'c',8,"0.25,0.25,0.25"},
    wyckInfo{'d',24,"0,0.25,0.25"},
    wyckInfo{'e',24,"x,0,0"},
    wyckInfo{'f',32,"x,x,x"},
    wyckInfo{'g',48,"0,y,y"},
    wyckInfo{'h',48,"0.5,y,y"},
    wyckInfo{'i',48,"x,0.25,0.25"},
    wyckInfo{'j',96,"x,y,z"}
  },

  { // 210
    wyckInfo{'a',8,"0,0,0"},
    wyckInfo{'b',8,"0.5,0.5,0.5"},
    wyckInfo{'c',16,"0.125,0.125,0.125"},
    wyckInfo{'d',16,"0.625,0.625,0.625"},
    wyckInfo{'e',32,"x,x,x"},
    wyckInfo{'f',48,"x,0,0"},
    wyckInfo{'g',48,"0.125,y,-y+0.25"},
    wyckInfo{'h',96,"x,y,z"}
  },

  { // 211
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',6,"0,0.5,0.5"},
    wyckInfo{'c',8,"0.25,0.25,0.25"},
    wyckInfo{'d',12,"0.25,0.5,0"},
    wyckInfo{'e',12,"x,0,0"},
    wyckInfo{'f',16,"x,x,x"},
    wyckInfo{'g',24,"x,0.5,0"},
    wyckInfo{'h',24,"0,y,y"},
    wyckInfo{'i',24,"0.25,y,-y+0.5"},
    wyckInfo{'j',48,"x,y,z"}
  },

  { // 212
    wyckInfo{'a',4,"0.125,0.125,0.125"},
    wyckInfo{'b',4,"0.625,0.625,0.625"},
    wyckInfo{'c',8,"x,x,x"},
    wyckInfo{'d',12,"0.125,y,-y+0.25"},
    wyckInfo{'e',24,"x,y,z"}
  },

  { // 213
    wyckInfo{'a',4,"0.375,0.375,0.375"},
    wyckInfo{'b',4,"0.875,0.875,0.875"},
    wyckInfo{'c',8,"x,x,x"},
    wyckInfo{'d',12,"0.125,y,y+0.25"},
    wyckInfo{'e',24,"x,y,z"}
  },

  { // 214
    wyckInfo{'a',8,"0.125,0.125,0.125"},
    wyckInfo{'b',8,"0.875,0.875,0.875"},
    wyckInfo{'c',12,"0.125,0,0.25"},
    wyckInfo{'d',12,"0.625,0,0.25"},
    wyckInfo{'e',16,"x,x,x"},
    wyckInfo{'f',24,"x,0,0.25"},
    wyckInfo{'g',24,"0.125,y,y+0.25"},
    wyckInfo{'h',24,"0.125,y,-y+0.25"},
    wyckInfo{'i',48,"x,y,z"}
  },

  { // 215
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0.5,0.5,0.5"},
    wyckInfo{'c',3,"0,0.5,0.5"},
    wyckInfo{'d',3,"0.5,0,0"},
    wyckInfo{'e',4,"x,x,x"},
    wyckInfo{'f',6,"x,0,0"},
    wyckInfo{'g',6,"x,0.5,0.5"},
    wyckInfo{'h',12,"x,0.5,0"},
    wyckInfo{'i',12,"x,x,z"},
    wyckInfo{'j',24,"x,y,z"}
  },

  { // 216
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0.5,0.5,0.5"},
    wyckInfo{'c',4,"0.25,0.25,0.25"},
    wyckInfo{'d',4,"0.75,0.75,0.75"},
    wyckInfo{'e',16,"x,x,x"},
    wyckInfo{'f',24,"x,0,0"},
    wyckInfo{'g',24,"x,0.25,0.25"},
    wyckInfo{'h',48,"x,x,z"},
    wyckInfo{'i',96,"x,y,z"}
  },

  { // 217
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',6,"0,0.5,0.5"},
    wyckInfo{'c',8,"x,x,x"},
    wyckInfo{'d',12,"0.25,0.5,0"},
    wyckInfo{'e',12,"x,0,0"},
    wyckInfo{'f',24,"x,0.5,0"},
    wyckInfo{'g',24,"x,x,z"},
    wyckInfo{'h',48,"x,y,z"}
  },

  { // 218
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',6,"0,0.5,0.5"},
    wyckInfo{'c',6,"0.25,0.5,0"},
    wyckInfo{'d',6,"0.25,0,0.5"},
    wyckInfo{'e',8,"x,x,x"},
    wyckInfo{'f',12,"x,0,0"},
    wyckInfo{'g',12,"x,0.5,0"},
    wyckInfo{'h',12,"x,0,0.5"},
    wyckInfo{'i',24,"x,y,z"}
  },

  { // 219
    wyckInfo{'a',8,"0,0,0"},
    wyckInfo{'b',8,"0.25,0.25,0.25"},
    wyckInfo{'c',24,"0,0.25,0.25"},
    wyckInfo{'d',24,"0.25,0,0"},
    wyckInfo{'e',32,"x,x,x"},
    wyckInfo{'f',48,"x,0,0"},
    wyckInfo{'g',48,"x,0.25,0.25"},
    wyckInfo{'h',96,"x,y,z"}
  },

  { // 220
    wyckInfo{'a',12,"0.375,0,0.25"},
    wyckInfo{'b',12,"0.875,0,0.25"},
    wyckInfo{'c',16,"x,x,x"},
    wyckInfo{'d',24,"x,0,0.25"},
    wyckInfo{'e',48,"x,y,z"}
  },

  { // 221
    wyckInfo{'a',1,"0,0,0"},
    wyckInfo{'b',1,"0.5,0.5,0.5"},
    wyckInfo{'c',3,"0,0.5,0.5"},
    wyckInfo{'d',3,"0.5,0,0"},
    wyckInfo{'e',6,"x,0,0"},
    wyckInfo{'f',6,"x,0.5,0.5"},
    wyckInfo{'g',8,"x,x,x"},
    wyckInfo{'h',12,"x,0.5,0"},
    wyckInfo{'i',12,"0,y,y"},
    wyckInfo{'j',12,"0.5,y,y"},
    wyckInfo{'k',24,"0,y,z"},
    wyckInfo{'l',24,"0.5,y,z"},
    wyckInfo{'m',24,"x,x,z"},
    wyckInfo{'n',48,"x,y,z"}
  },

  { // 222
    wyckInfo{'a',2,"0.25,0.25,0.25"},
    wyckInfo{'b',6,"0.75,0.25,0.25"},
    wyckInfo{'c',8,"0,0,0"},
    wyckInfo{'d',12,"0,0.75,0.25"},
    wyckInfo{'e',12,"x,0.25,0.25"},
    wyckInfo{'f',16,"x,x,x"},
    wyckInfo{'g',24,"x,0.75,0.25"},
    wyckInfo{'h',24,"0.25,y,y"},
    wyckInfo{'i',48,"x,y,z"}
  },

  { // 223
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',6,"0,0.5,0.5"},
    wyckInfo{'c',6,"0.25,0,0.5"},
    wyckInfo{'d',6,"0.25,0.5,0"},
    wyckInfo{'e',8,"0.25,0.25,0.25"},
    wyckInfo{'f',12,"x,0,0"},
    wyckInfo{'g',12,"x,0,0.5"},
    wyckInfo{'h',12,"x,0.5,0"},
    wyckInfo{'i',16,"x,x,x"},
    wyckInfo{'j',24,"0.25,y,y+0.5"},
    wyckInfo{'k',24,"0,y,z"},
    wyckInfo{'l',48,"x,y,z"}
  },

  { // 224
    wyckInfo{'a',2,"0.25,0.25,0.25"},
    wyckInfo{'b',4,"0,0,0"},
    wyckInfo{'c',4,"0.5,0.5,0.5"},
    wyckInfo{'d',6,"0.25,0.75,0.75"},
    wyckInfo{'e',8,"x,x,x"},
    wyckInfo{'f',12,"0.5,0.25,0.75"},
    wyckInfo{'g',12,"x,0.25,0.25"},
    wyckInfo{'h',24,"x,0.25,0.75"},
    wyckInfo{'i',24,"0.5,y,y+0.5"},
    wyckInfo{'j',24,"0.5,y,-y"},
    wyckInfo{'k',24,"x,x,z"},
    wyckInfo{'l',48,"x,y,z"}
  },

  { // 225
    wyckInfo{'a',4,"0,0,0"},
    wyckInfo{'b',4,"0.5,0.5,0.5"},
    wyckInfo{'c',8,"0.25,0.25,0.25"},
    wyckInfo{'d',24,"0,0.25,0.25"},
    wyckInfo{'e',24,"x,0,0"},
    wyckInfo{'f',32,"x,x,x"},
    wyckInfo{'g',48,"x,0.25,0.25"},
    wyckInfo{'h',48,"0,y,y"},
    wyckInfo{'i',48,"0.5,y,y"},
    wyckInfo{'j',96,"0,y,z"},
    wyckInfo{'k',96,"x,x,z"},
    wyckInfo{'l',192,"x,y,z"}
  },

  { // 226
    wyckInfo{'a',8,"0.25,0.25,0.25"},
    wyckInfo{'b',8,"0,0,0"},
    wyckInfo{'c',24,"0.25,0,0"},
    wyckInfo{'d',24,"0,0.25,0.25"},
    wyckInfo{'e',48,"x,0,0"},
    wyckInfo{'f',48,"x,0.25,0.25"},
    wyckInfo{'g',64,"x,x,x"},
    wyckInfo{'h',96,"0.25,y,y"},
    wyckInfo{'i',96,"0,y,z"},
    wyckInfo{'j',192,"x,y,z"}
  },

  { // 227
    wyckInfo{'a',8,"0.125,0.125,0.125"},
    wyckInfo{'b',8,"0.375,0.375,0.375"},
    wyckInfo{'c',16,"0,0,0"},
    wyckInfo{'d',16,"0.5,0.5,0.5"},
    wyckInfo{'e',32,"x,x,x"},
    wyckInfo{'f',48,"x,0.125,0.125"},
    wyckInfo{'g',96,"x,x,z"},
    wyckInfo{'h',96,"0,y,-y"},
    wyckInfo{'i',192,"x,y,z"}
  },

  { // 228
    wyckInfo{'a',16,"0.125,0.125,0.125"},
    wyckInfo{'b',32,"0.25,0.25,0.25"},
    wyckInfo{'c',32,"0,0,0"},
    wyckInfo{'d',48,"0.875,0.125,0.125"},
    wyckInfo{'e',64,"x,x,x"},
    wyckInfo{'f',96,"x,0.125,0.125"},
    wyckInfo{'g',96,"0.25,y,-y"},
    wyckInfo{'h',192,"x,y,z"}
  },

  { // 229
    wyckInfo{'a',2,"0,0,0"},
    wyckInfo{'b',6,"0,0.5,0.5"},
    wyckInfo{'c',8,"0.25,0.25,0.25"},
    wyckInfo{'d',12,"0.25,0,0.5"},
    wyckInfo{'e',12,"x,0,0"},
    wyckInfo{'f',16,"x,x,x"},
    wyckInfo{'g',24,"x,0,0.5"},
    wyckInfo{'h',24,"0,y,y"},
    wyckInfo{'i',48,"0.25,y,-y+0.5"},
    wyckInfo{'j',48,"0,y,z"},
    wyckInfo{'k',48,"x,x,z"},
    wyckInfo{'l',96,"x,y,z"}
  },

  { // 230
    wyckInfo{'a',16,"0,0,0"},
    wyckInfo{'b',16,"0.125,0.125,0.125"},
    wyckInfo{'c',24,"0.125,0,0.25"},
    wyckInfo{'d',24,"0.375,0,0.25"},
    wyckInfo{'e',32,"x,x,x"},
    wyckInfo{'f',48,"x,0,0.25"},
    wyckInfo{'g',48,"0.125,y,-y+0.25"},
    wyckInfo{'h',96,"x,y,z"}
  }

};

const wyckoffPositions& SpgInit::getWyckoffPositions(uint spg)
{
  if (spg < 1 || spg > 230) {
    cout << "Error. getWyckoffPositions() was called for a spacegroup "
         << "that does not exist! Given spacegroup is " << spg << endl;
    return wyckoffPositionsDatabase.at(0);
  }

  return wyckoffPositionsDatabase.at(spg);
}

vector<atomStruct> SpgInit::generateInitWyckoffs(uint spg, vector<uint> atomTpes)
{

}

inline vector<uint> SpgInit::getNumOfEachType(vector<uint> atomTypes)
{
  vector<uint> atomsAlreadyCounted, numOfEachType;
  for (size_t i = 0; i < atomTypes.size(); i++) {
    size_t size = 0;
    // If we already counted this one, just continue
    if (std::find(atomsAlreadyCounted.begin(), atomsAlreadyCounted.end(),
                  atomTypes.at(i)) != atomsAlreadyCounted.end()) continue;
    for (size_t j = 0; j < atomTypes.size(); j++)
      if (atomTypes.at(j) == atomTypes.at(i)) size++;
    numOfEachType.push_back(size);
    atomsAlreadyCounted.push_back(atomTypes.at(i));
  }
  // Sort from largest to smallest
  sort(numOfEachType.begin(), numOfEachType.end(), greater<uint>());
  return numOfEachType;
}

// Basic split of a string based upon a delimiter.
static inline vector<string> split(const string& s, char delim)
{
  vector<string> elems;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

// Basic check to see if a string is a number
// Includes negative numbers
// If it runs into an "x", "y", or "z", it should return false
static inline bool isNumber(const string& s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && (isdigit(*it) || *it == '-' || *it == '.')) ++it;
  return !s.empty() && it == s.end();
}

// A unique position is a position that has no x, y, or z in it
inline bool SpgInit::containsUniquePosition(wyckInfo& info)
{
  vector<string> xyzStrings = split(get<2>(info), ',');
  assert(xyzStrings.size() == 3);
  for (size_t i = 0; i < xyzStrings.size(); i++)
    if (!isNumber(xyzStrings.at(i))) return false;
  return true;
}

vector<pair<uint, bool> > SpgInit::getMultiplicityVector(wyckoffPositions& pos)
{
  vector<pair<uint, bool> > multiplicityVector;
  multiplicityVector.reserve(pos.size());
  for (size_t i = 0; i < pos.size(); i++)
    multiplicityVector.push_back(make_pair(get<1>(pos.at(i)), containsUniquePosition(pos.at(i))));
  return multiplicityVector;
}

static bool everyoneFoundAHome(vector<uint> numOfEachType, wyckoffPositions pos)
{
  // The "uint" is the multiplicity and the "bool" is whether it's unique or not
  vector<pair<uint, bool> > multiplicityVector = SpgInit::getMultiplicityVector(pos);

#ifdef SPGINIT_DEBUG
  cout << "multiplicity vector is <multiplicity> <unique?>:\n";
  for (size_t i = 0; i < multiplicityVector.size(); i++)
    cout << multiplicityVector.at(i).first << " " << multiplicityVector.at(i).second << "\n";
#endif

  // Keep track of which wyckoff positions have been used
  vector<bool> wyckoffPositionUsed;
  wyckoffPositionUsed.reserve(multiplicityVector.size());
  for (size_t i = 0; i < multiplicityVector.size(); i++)
    wyckoffPositionUsed.push_back(false);

  // These are arranged from smallest to largest already
  for (size_t i = 0; i < numOfEachType.size(); i++) {
    bool foundAHome = false;
    // Start with the highest wyckoff letter and work our way down
    // This will put as many atoms as possible into the general positions
    // while leaving unique positions for later
    for (int j = multiplicityVector.size() - 1; j >= 0; j--) {
      bool unique = multiplicityVector.at(j).second;
      // If it's not unique
      if (!unique &&
          // Then check to see if it CAN be used
          numOfEachType.at(i) % multiplicityVector.at(j).first == 0) {
        wyckoffPositionUsed[j] = true;
        foundAHome = true;
        break;
      }
      // If it IS unique and hasn't been used
      else if (unique && !wyckoffPositionUsed.at(j) &&
               // Then check to see if they are equivalent
               numOfEachType.at(i) == multiplicityVector.at(j).first) {
        wyckoffPositionUsed[j] = true;
        foundAHome = true;
        break;
      }
      // Finally, if it is unique and hasn't been used
      else if (unique && !wyckoffPositionUsed.at(j) &&
               numOfEachType.at(i) % multiplicityVector.at(j).first == 0) {
        // If it failed the prior test, then this must be a multiple and NOT
        // equivalent (i. e., 4 and 2). Since this is the case, just find a home
        // for the atoms that CAN fit and just proceed to find a home for the
        // others
        wyckoffPositionUsed[j] = true;
        i--;
        numOfEachType[j] -= multiplicityVector.at(j).first;
        foundAHome = true;
        break;
      }
    }
#ifdef SPGINIT_DEBUG
    cout << "wyckoffPositionUsed is:\n";
    for (size_t i = 0; i < wyckoffPositionUsed.size(); i++)
      cout << wyckoffPositionUsed[i] << "\n";
#endif
    if (!foundAHome) return false;
  }
  // If we made it here without returning false, every atom type found a home
  return true;
}

bool SpgInit::isSpgPossible(uint spg, vector<uint> atomTypes)
{
#ifdef SPGINIT_DEBUG
  cout << __FUNCTION__ << " called!\n";
  cout << "atomTypes is:\n";
  for (size_t i = 0; i < atomTypes.size(); i++) cout << atomTypes[i] << "\n";
#endif
  if (spg < 1 || spg > 230) return false;

  wyckoffPositions pos = getWyckoffPositions(spg);
  size_t numAtoms = atomTypes.size();
  vector<uint> numOfEachType = getNumOfEachType(atomTypes);

#ifdef SPGINIT_DEBUG
  cout << "numAtoms is " << numAtoms << "\n";
  cout << "numOfEachType is:\n";
  for (size_t i = 0; i < numOfEachType.size(); i++)
    cout << numOfEachType.at(i) << "\n";
#endif
  if (!everyoneFoundAHome(numOfEachType, pos)) return false;

  return true;
}

template <typename T>
inline T getSmallest(T a, T b, T c)
{
  if (a <= b && a <= c) return a;
  else if (b <= a && b <= c) return b;
  else return c;
}

template <typename T>
inline T getLargest(T a, T b, T c)
{
  if (a >= b && a >= c) return a;
  else if (b >= a && b >= c) return b;
  else return c;
}

inline double getRandDoubleInRange(double min, double max)
{
  // RANDDOUBLE() should generate a random double between 0 and 1
  return RANDDOUBLE() * (max - min) + min;
}

latticeStruct SpgInit::generateLatticeForSpg(uint spg,
                                             latticeStruct& mins,
                                             latticeStruct& maxes)
{
  latticeStruct st;
  if (spg < 1 || spg > 230) {
    cout << "Error: " << __FUNCTION__ << " was called for a "
         << "non-real spacegroup: " << spg << endl;
    // latticeStruct is initialized to have all "0" values. So just return a
    // "0" struct
    return st;
  }

  // Triclinic!
  else if (spg == 1 || spg == 2) {
    // There aren't really any constraints on a triclinic system...
    st.a     = getRandDoubleInRange(mins.a,     maxes.a);
    st.b     = getRandDoubleInRange(mins.b,     maxes.b);
    st.c     = getRandDoubleInRange(mins.c,     maxes.c);
    st.alpha = getRandDoubleInRange(mins.alpha, maxes.alpha);
    st.beta  = getRandDoubleInRange(mins.beta,  maxes.beta);
    st.gamma = getRandDoubleInRange(mins.gamma, maxes.gamma);
    return st;
  }

  // Monoclinic!
  else if (3 <= spg && spg <= 15) {
    // I am making beta unique here. This may or may not be the right angle
    // to make unique for the wyckoff positions in the database...

    // First make sure we can make alpha and gamma 90 degrees
    if (mins.alpha > 90 || maxes.alpha < 90 ||
        mins.gamma > 90 || maxes.gamma < 90) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains alpha and gamma to be 90 degrees. The "
           << "input min and max values for the alpha and gamma do not allow "
           << "this. Please change their min and max values.\n";
      return st;
    }

    st.a     = getRandDoubleInRange(mins.a,     maxes.a);
    st.b     = getRandDoubleInRange(mins.b,     maxes.b);
    st.c     = getRandDoubleInRange(mins.c,     maxes.c);
    st.alpha = st.gamma = 90.0;
    st.beta  = getRandDoubleInRange(mins.beta,  maxes.beta);
    return st;
  }

  // Orthorhombic!
  else if (16 <= spg && spg <= 74) {
    // For orthorhombic, all angles must be 90 degrees.
    // Check to see if we can make 90 degree angles
    if (mins.alpha > 90 || maxes.alpha < 90 ||
        mins.beta  > 90 || maxes.beta  < 90 ||
        mins.gamma > 90 || maxes.gamma < 90) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains all the angles to be 90 degrees. The "
           << "input min and max values for the angles do not allow this. "
           << "Please change their min and max values.\n";
      return st;
    }

    st.a     = getRandDoubleInRange(mins.a,     maxes.a);
    st.b     = getRandDoubleInRange(mins.b,     maxes.b);
    st.c     = getRandDoubleInRange(mins.c,     maxes.c);
    st.alpha = st.beta = st.gamma = 90.0;
    return st;
  }

  // Tetragonal!
  else if (75 <= spg && spg <= 142) {
    // For tetragonal, all angles must be 90 degrees.
    // Check to see if we can make 90 degree angles
    if (mins.alpha > 90 || maxes.alpha < 90 ||
        mins.beta  > 90 || maxes.beta  < 90 ||
        mins.gamma > 90 || maxes.gamma < 90) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains all the angles to be 90 degrees. The "
           << "input min and max values for the angles do not allow this. "
           << "Please change their min and max values.\n";
      return st;
    }

    // a and b need to be able to be the same number
    // Find the larger min and smaller max of each
    double largerMin = (mins.a > mins.b) ? mins.a : mins.b;
    double smallerMax = (maxes.a < maxes.b) ? maxes.a : maxes.b;

    if (largerMin > smallerMax) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains a and b to be equal. The "
           << "input min and max values for a and b do not allow this. "
           << "Please change their min and max values.\n";
      return st;
    }

    st.a     = st.b = getRandDoubleInRange(largerMin, smallerMax);
    st.c     = getRandDoubleInRange(mins.c, maxes.c);
    st.alpha = st.beta = st.gamma = 90.0;
    return st;
  }

  // Trigonal!
  // TODO: we are assuming here that all trigonal crystals can be
  // represented with hexagonal axes (and we are ignoring rhombohedral axes).
  // Is this correct?
  else if (143 <= spg && spg <= 167) {
    // For trigonal, alpha and beta must be 90 degrees, and gamma must be 120
    // Check to see if we can make these angles
    if (mins.alpha > 90 || maxes.alpha < 90 ||
        mins.beta  > 90 || maxes.beta  < 90) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains alpha and beta to be 90 degrees. The "
           << "input min and max values for the angles do not allow this. "
           << "Please change their min and max values.\n";
      return st;
    }

    if (mins.gamma > 120 || maxes.gamma < 120) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains gamma to be 120 degrees. The "
           << "input min and max values for gamma do not allow this. "
           << "Please change the min and max values.\n";
      return st;
    }

    // a and b need to be able to be the same number
    // Find the larger min and smaller max of each
    double largerMin = (mins.a > mins.b) ? mins.a : mins.b;
    double smallerMax = (maxes.a < maxes.b) ? maxes.a : maxes.b;

    if (largerMin > smallerMax) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains a and b to be equal. The "
           << "input min and max values for a and b do not allow this. "
           << "Please change their min and max values.\n";
      return st;
    }

    st.a     = st.b = getRandDoubleInRange(largerMin, smallerMax);
    st.c     = getRandDoubleInRange(mins.c, maxes.c);
    st.alpha = st.beta = 90.0;
    st.gamma = 120.0;
    return st;
  }

  // Hexagonal!
  // Note that this is identical to trigonal since in trigonal we are using
  // hexagonal axes.
  else if (168 <= spg && spg <= 194) {
    // For hexagonal, alpha and beta must be 90 degrees, and gamma must be 120
    // Check to see if we can make these angles
    if (mins.alpha > 90 || maxes.alpha < 90 ||
        mins.beta  > 90 || maxes.beta  < 90) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains alpha and beta to be 90 degrees. The "
           << "input min and max values for the angles do not allow this. "
           << "Please change their min and max values.\n";
      return st;
    }

    if (mins.gamma > 120 || maxes.gamma < 120) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains gamma to be 120 degrees. The "
           << "input min and max values for gamma do not allow this. "
           << "Please change the min and max values.\n";
      return st;
    }

    // a and b need to be able to be the same number
    // Find the larger min and smaller max of each
    double largerMin = (mins.a > mins.b) ? mins.a : mins.b;
    double smallerMax = (maxes.a < maxes.b) ? maxes.a : maxes.b;

    if (largerMin > smallerMax) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains a and b to be equal. The "
           << "input min and max values for a and b do not allow this. "
           << "Please change their min and max values.\n";
      return st;
    }

    st.a     = st.b = getRandDoubleInRange(largerMin, smallerMax);
    st.c     = getRandDoubleInRange(mins.c, maxes.c);
    st.alpha = st.beta = 90.0;
    st.gamma = 120.0;
    return st;
  }

  // Cubic!
  /*   ______
      /     /|
     /_____/ |
     |     | |
     |     | /
     |_____|/
  */
  else if (spg >= 195) {
    // We need to make sure that 90 degrees is an option and that a, b, and c
    // can be equal. Otherwise, it is impossible to generate this lattice
    if (mins.alpha > 90 || maxes.alpha < 90 ||
        mins.beta  > 90 || maxes.beta  < 90 ||
        mins.gamma > 90 || maxes.gamma < 90) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains all the angles to be 90 degrees. The "
           << "input min and max values for the angles do not allow this. "
           << "Please change their min and max values.\n";
      return st;
    }
    // Can a, b, and c be equal?
    // Find the greatest min value and the smallest max value
    double largestMin = getLargest<double>(mins.a, mins.b, mins.c);
    double smallestMax = getSmallest<double>(maxes.a, maxes.b, maxes.c);

    // They can't be equal!
    if (largestMin > smallestMax) {
      cout << "Error: " << __FUNCTION__ << " was called for a spacegroup of "
           << spg << " which constrains a, b, and c to be equal. The "
           << "input min and max values for a, b, and c do not allow this. "
           << "Please change their min and max values.\n";
      return st;
    }

    // If we made it this far, we can set up the cell!
    st.alpha = st.beta = st.gamma = 90.0;
    st.a = st.b = st.c = getRandDoubleInRange(largestMin, smallestMax);

    return st;
  }

  // We shouldn't get to this point because one of the if statements should have
  // worked...
  cout << "Error: " << __FUNCTION__ << " has a problem identifying spg " << spg
       << "\n";
  return st;
}
