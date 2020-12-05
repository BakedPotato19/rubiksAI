#include <bits/stdint-uintn.h>
#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<assert.h>


/* INDEX FORMATING
 * EACH FACE IS STORED IN A 64-bit int
 * 
 * FORMATTING:
 *
 *       0 1 2
 *       3 X 4
 *       5 6 7
 * 0 1 2 0 1 2 0 1 2 0 1 2
 * 3 X 4 3 X 4 3 X 4 3 X 4
 * 5 6 7 5 6 7 5 6 7 5 6 7
 *       0 1 2
 *       3 X 4
 *       5 6 7
 *
 * X is non-indexable as rubiks-centers is static
 *
 * In the 64-bit indexing is as following: (Greatest bit at lefthandside)
 * 0 - 1 - 2 etc.
 * 
 * */

struct rubiks
{
	uint64_t **faces;
};


enum colors{White, Yellow, Green, Blue, Red, Orange};
//CW = Clockwise
//CCW = Counter Clockwise
enum movement_dir{CW,CCW};

struct turn_data
{
	enum colors center;
	enum colors top;
	enum colors right;
	enum colors bottom;
	enum colors left;
	enum movement_dir dir;
	uint8_t** relative_loc;
};

extern char color_char[];

extern uint8_t turns[];
extern char* turn_symbol[];

enum colors face_getval(uint64_t *face, int index);
enum colors face_insertval(uint64_t *face, int index, enum colors val);

void initialize_face(uint64_t *face, uint8_t color);
void initialize_faces(struct rubiks *rubiks);
void initialize_faces_test(struct rubiks *rubiks);
void initialize_rubiks(struct rubiks *rubiks);
void initialize_turndata();

struct rubiks* turn(struct rubiks *rubiks, struct turn_data *data);
struct rubiks* u_mov(struct rubiks *rubiks);
struct rubiks* r_mov(struct rubiks *rubiks);
struct rubiks* l_mov(struct rubiks *rubiks);
struct rubiks* f_mov(struct rubiks *rubiks);
struct rubiks* b_mov(struct rubiks *rubiks);
struct rubiks* d_mov(struct rubiks *rubiks);

struct rubiks* up_mov(struct rubiks *rubiks);
struct rubiks* rp_mov(struct rubiks *rubiks);
struct rubiks* lp_mov(struct rubiks *rubiks);
struct rubiks* fp_mov(struct rubiks *rubiks);
struct rubiks* bp_mov(struct rubiks *rubiks);
struct rubiks* dp_mov(struct rubiks *rubiks);

void print_cube(struct rubiks *rubiks);

