#ifndef UTILS_H_INC
#define UTILS_H_INC

#include <signal.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "board.h"
#include "move.h"

#define WHITE 0
#define BLACK 1
#define FORCE 2

#define KINGSIDE  0
#define QUEENSIDE 1

#define EXACTLY 0
#define ATLEAST 1
#define ATMOST  2

static const int inf =  (1 << 30);

#define HT_SIZE (1 << 22)

typedef struct hash_table_entry {
    uint64_t key;
    uint8_t depth;
    uint8_t type;
    uint8_t color;
    MoveScore move;
} hash_table_entry;

static int piece_square[6][64] = {
    { 
 0,  0,  0,  0,  0,  0,  0,  0,
    60, 60, 60, 60, 60, 60, 60, 60,
    10, 20, 40, 50, 50, 40, 20, 10,
     5,  5, 10, 45, 45, 10,  5,  5,
     0,  0,  0, 40, 40,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-20,-20, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
    },
    { 
-50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,
    },
    {   -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
 

    },
    { 
  0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0
    },
    { 
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
    },
    { 
  40,  50,  30,  10,  10,  30,  50,  40,
	30,  40,  20,   0,   0,  20,  40,  30,
	10,  20,   0, -20, -20,   0,  20,  10,
	0,  10, -10, -30, -30, -10,  10,   0,
	-10,   0, -20, -40, -40, -20,   0, -10,
	-20, -10, -30, -50, -50, -30, -10, -20,
	-30, -20, -40, -60, -60, -40, -20, -30,
	-40, -30, -50, -70, -70, -50, -30, -40 
    }
};

static int piece_square1[6][64] = {
    { 
  -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
    },
    { 
 -39, -27, -23, -20, -20, -23, -27, -39,
	-27, -20, -12,  -8,  -8, -12, -20, -27,
	-8,  -4,   0,   8,   8,   0,  -4,  -8,
	-4,   8,  12,  16,  16,  12,   8,  -4,
	8,  16,  20,  23,  23,  20,  16,   8,
	12,  23,  27,  31,  31,  27,  23,  12,
	-2,   2,   4,   8,   8,   4,   2,  -2,
	-16,  -8,  -4,  -4,  -4,  -4,  -8, -16,
    },
    { 
   -39, -27, -23, -20, -20, -23, -27, -39,
	-27, -20, -12,  -8,  -8, -12, -20, -27,
	-8,  -4,   0,   8,   8,   0,  -4,  -8,
	-4,   8,  12,  16,  16,  12,   8,  -4,
	8,  16,  20,  23,  23,  20,  16,   8,
	12,  23,  27,  31,  31,  27,  23,  12,
	-2,   2,   4,   8,   8,   4,   2,  -2,
	-16,  -8,  -4,  -4,  -4,  -4,  -8, -16,
    },
    { 
    -10,  -8,  -6,  -4,  -4,  -6,  -8, -10,
	-8,  -6,  -4,  -2,  -2,  -4,  -6,  -8,
	-4,  -2,   0,   4,   4,   0,  -2,  -4,
	-2,   2,   4,   8,   8,   4,   2,  -2,
	2,   4,   8,  12,  12,   8,   4,   2,
	4,   8,  12,  16,  16,  12,   8,   4,
	20,  21,  23,  23,  23,  23,  21,  20,
	18,  18,  20,  20,  20,  20,  18,  18,
    },
    { 
   -23, -20, -16, -12, -12, -16, -20, -23,
	-18, -14, -12,  -8,  -8, -12, -14, -18,
	-16,  -8,   0,   8,   8,   0,  -8, -16,
	-8,   0,  12,  16,  16,  12,   0,  -8,
	4,  12,  16,  23,  23,  16,  12,   4,
	16,  23,  27,  31,  31,  27,  23,  16,
	4,  12,  16,  23,  23,  16,  12,   4,
	2,   8,  12,  12,  12,  12,   8,   2,
    },
    { 
    -34, -30, -28, -27, -27, -28, -30, -34,
	-17, -13, -11, -10, -10, -11, -13, -17,
	-2,   2,   4,   5,   5,   4,   2,  -2,
	11,  15,  17,  18,  18,  17,  15,  11,
	22,  26,  28,  29,  29,  28,  26,  22,
	31,  34,  37,  38,  38,  37,  34,  31,
	38,  41,  44,  45,  45,  44,  41,  38,
	42,  46,  48,  50,  50,  48,  46,  42,
    }
};

extern uint64_t zobrist[8][64];

MoveScore alphabeta(Game game, int alpha, int beta, int depth,bool isendgame);
Move best_move(Game game,bool isendgame);

#endif
