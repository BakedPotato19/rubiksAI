#include "rubiks.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


char color_char[] = {'w', 'y', 'g', 'b', 'r', 'o'};

//Used for clearing 8 bits at given index
uint64_t indexes[] = {0x00FFFFFFFFFFFFFF,
                      0xFF00FFFFFFFFFFFF,
                      0xFFFF00FFFFFFFFFF,
					  0xFFFFFF00FFFFFFFF,
                      0xFFFFFFFF00FFFFFF,
					  0xFFFFFFFFFF00FFFF,
                      0xFFFFFFFFFFFF00FF,
                      0xFFFFFFFFFFFFFF00};

/*========================================================
 * This section contains hardcoded data for
 * turning the cube directly. It mostly consists of
 * lookup_tables and predefined turn_data structs.
 * TODO:
 * Ideally this is not hardcoded; however
 * i do not have the patience to figure out
 * the turning math right now. 
 * Refactor it sometime.
 *
 =========================================================*/

uint8_t cwface_turnorder[] = {0,1,2,4,7,6,5,3};
uint8_t ccwface_turnorder[] = {0,3,5,6,7,4,2,1};

uint8_t left_turn_iterator[] = {1,2,3};
uint8_t right_turn_iterator[] = {3,2,1};

uint8_t top_face_indexes[] = {5,6,7};
uint8_t toprev_face_indexes[] = {7,6,5};
uint8_t right_face_indexes[] = {0,3,5};
uint8_t rightrev_face_indexes[] = {5,3,0};
uint8_t bottom_face_indexes[] = {2,1,0};
uint8_t bottomrev_face_indexes[] = {0,1,2};
uint8_t left_face_indexes[] = {7,4,2};
uint8_t leftrev_face_indexes[] = {2,4,7};

struct turn_data *u_data;
struct turn_data *r_data;
struct turn_data *l_data;
struct turn_data *f_data;
struct turn_data *b_data;
struct turn_data *d_data;



/*=======================================
 * This section contains source for
 * functions reading from and 
 * manipulating the cube faces.
 * This includes: 
 * face_getval(...) -> enum colors
 * face_insertval(...) -> enum colors
 ==========================================*/

enum colors face_getval(uint64_t* face, int index)
{
	assert(index>=0 && index<=7);
	return ((*face) <<(index*8))>>56;
}

enum colors face_insertval(uint64_t* face, int index, enum colors val)
{
	enum colors ret_val = face_getval(face, index); 

	//Clear 8 bits at index
	(*face) &= indexes[index];

	//Insert new val at index
	uint8_t r = ((index*7)+7)%8; // Amount to shift by
	(*face) |= ((uint64_t) val)<<(8*r);	

	return ret_val;
}


// This is the generel turning algorithm


void initialize_face(uint64_t *face, uint8_t color)
{
	(*face) = 0;
	for(int i = 0; i<8;i++)
	{
		(*face) |= color;		
		if(i!=7) (*face) = *face<<8;
	}
}

void initialize_faces(struct rubiks *rubiks)
{
	for(int i = White; i<=Orange;i++)
	{
		initialize_face(rubiks->faces[i], i);
	}
}

void initialize_rubiks(struct rubiks *rubiks)
{
	rubiks->faces = malloc(sizeof(uint64_t*)*6);
	rubiks->faces[White] = malloc(sizeof(uint64_t));
	rubiks->faces[Yellow] = malloc(sizeof(uint64_t));
	rubiks->faces[Green] = malloc(sizeof(uint64_t));
	rubiks->faces[Blue] = malloc(sizeof(uint64_t));
	rubiks->faces[Red] = malloc(sizeof(uint64_t));
	rubiks->faces[Orange] = malloc(sizeof(uint64_t));

}

void initialize_turndata()
{
	u_data = malloc(sizeof(struct turn_data));
	u_data->center = White;
	u_data->top = Red;
	u_data->right = Green;
	u_data->bottom = Orange;
	u_data->left = Blue;
	u_data->dir = CW;
	u_data->relative_loc = malloc(sizeof(uint8_t*)*4);
	u_data->relative_loc[0] = top_face_indexes;
	u_data->relative_loc[1] = right_face_indexes;
	u_data->relative_loc[2] = bottom_face_indexes;
	u_data->relative_loc[3] = left_face_indexes;

	r_data = malloc(sizeof(struct turn_data));
	r_data->center = Green;
	r_data->top = Red;
	r_data->right = Yellow;
	r_data->bottom = Orange;
	r_data->left = White;
	r_data->dir = CW;
	r_data->relative_loc = malloc(sizeof(uint8_t*)*4);
	r_data->relative_loc[0] = left_face_indexes;
	r_data->relative_loc[1] = right_face_indexes;
	r_data->relative_loc[2] = left_face_indexes;
	r_data->relative_loc[3] = left_face_indexes;

	l_data = malloc(sizeof(struct turn_data));
	l_data->center = Blue;
	l_data->top = Red;
	l_data->right = White;
	l_data->bottom = Orange;
	l_data->left = Yellow;
	l_data->dir = CW;
	l_data->relative_loc = malloc(sizeof(uint8_t*)*4);
	l_data->relative_loc[0] = right_face_indexes;
	l_data->relative_loc[1] = right_face_indexes;
	l_data->relative_loc[2] = right_face_indexes;
	l_data->relative_loc[3] = left_face_indexes;

	f_data = malloc(sizeof(struct turn_data));
	f_data->center = Orange;
	f_data->top = White;
	f_data->right = Green;
	f_data->bottom = Yellow;
	f_data->left = Blue;
	f_data->dir = CW;
	f_data->relative_loc = malloc(sizeof(uint8_t*)*4);
	f_data->relative_loc[0] = top_face_indexes;
	f_data->relative_loc[1] = top_face_indexes;
	f_data->relative_loc[2] = top_face_indexes;
	f_data->relative_loc[3] = top_face_indexes;

	d_data = malloc(sizeof(struct turn_data));
	d_data->center = Yellow;
	d_data->top = Orange;
	d_data->right = Green;
	d_data->bottom = Red;
	d_data->left = Blue;
	d_data->dir = CW;
	d_data->relative_loc = malloc(sizeof(uint8_t*)*4);
	d_data->relative_loc[0] = top_face_indexes;
	d_data->relative_loc[1] = left_face_indexes;
	d_data->relative_loc[2] = bottom_face_indexes;
	d_data->relative_loc[3] = right_face_indexes;

	b_data = malloc(sizeof(struct turn_data));
	b_data->center = Red;
	b_data->top = Yellow;
	b_data->right = Green;
	b_data->bottom = White;
	b_data->left = Blue;
	b_data->dir = CW;
	b_data->relative_loc = malloc(sizeof(uint8_t*)*4);
	b_data->relative_loc[0] = bottom_face_indexes;
	b_data->relative_loc[1] = bottom_face_indexes;
	b_data->relative_loc[2] = bottom_face_indexes;
	b_data->relative_loc[3] = bottom_face_indexes;
}

struct rubiks* turn(struct rubiks *rubiks, struct turn_data *data)
{
	
	// Copy rubiks
	struct rubiks *ret_val = malloc(sizeof(struct rubiks));
	initialize_rubiks(ret_val);
	for(int i = White; i<=Orange; i++)
	{
		*(ret_val->faces[i]) = *(rubiks->faces[i]);
	}

	// Store pointers from "struct rubiks" chosen from 
	// the "struct data" to local variabels.
	// This is done for ease of use and readability.
	uint64_t *center = ret_val->faces[data->center];
	uint64_t *sides[] = {ret_val->faces[data->top],
						 ret_val->faces[data->right],
						 ret_val->faces[data->bottom],
						 ret_val->faces[data->left]};

	int dir = !data->dir;

	uint8_t* l[] = {right_face_indexes, bottom_face_indexes, left_face_indexes};


	// Move sides
	for(int i = 0; i<3; i++)
	{
		enum colors c = face_getval(sides[0], data->relative_loc[0][i]);
		for(int j = 0; j<3;j++)
		{
			uint8_t index = dir ? left_turn_iterator[j] : right_turn_iterator[j];
			c = face_insertval(sides[index], data->relative_loc[index][i], c);
		}
		face_insertval(sides[0], data->relative_loc[0][i], c);
	}

	//Move center face
	//TODO: Refactor this shit.
	enum colors c = face_getval(center,0);
	for(int i = 1; i<8;i++)
	{
		uint8_t index = dir ? cwface_turnorder[i] : ccwface_turnorder[i];
		c = face_insertval(center, index, c);
	}
	face_insertval(center, 0, c);
	c = face_getval(center,0);
	for(int i = 1; i<8;i++)
	{
		uint8_t index = dir ? cwface_turnorder[i] : ccwface_turnorder[i];
		c = face_insertval(center, index, c);
	}
	face_insertval(center, 0, c);

	return ret_val;
}

void print_cube(struct rubiks *rubiks)
{

	uint8_t white[8];
	uint8_t yellow[8];
	uint8_t green[8];
	uint8_t blue[8];
	uint8_t red[8];
	uint8_t orange[8];

	for(int i = 0; i<8;i++)
	{
		white[i]=color_char[face_getval(rubiks->faces[White], i)];
		yellow[i]=color_char[face_getval(rubiks->faces[Yellow], i)];
		green[i]=color_char[face_getval(rubiks->faces[Green], i)];
		blue[i]=color_char[face_getval(rubiks->faces[Blue], i)];
		red[i]=color_char[face_getval(rubiks->faces[Red], i)];
		orange[i]=color_char[face_getval(rubiks->faces[Orange], i)];

	}
	// Red face
	printf("%*s| %c %c %c |\n",8," ",red[0],red[1],red[2]);
	printf("%*s| %c %c %c |\n",8," ",red[3],color_char[Red],red[4]);
	printf("%*s| %c %c %c |\n",8," ",red[5],red[6],red[7]);
	printf("\n");

	// In order from left to right:
	// Blue Face
	// White Face
	// Green Face
	// Yellow Face
	printf("| %c %c %c | %c %c %c | %c %c %c | %c %c %c |\n",
			blue[0],blue[1],blue[2],white[0],white[1],white[2],
			green[0],green[1],green[2],yellow[0],yellow[1],yellow[2]);
	printf("| %c %c %c | %c %c %c | %c %c %c | %c %c %c |\n",
			blue[3],color_char[Blue],blue[4],white[3],color_char[White],white[4],
			green[3],color_char[Green],green[4],yellow[3],color_char[Yellow],yellow[4]);
	printf("| %c %c %c | %c %c %c | %c %c %c | %c %c %c |\n",
			blue[5],blue[6],blue[7],white[5],white[6],white[7],
			green[5],green[6],green[7],yellow[5],yellow[6],yellow[7]);
	

	// Orange Face
	printf("\n");
	printf("%*s| %c %c %c |\n",8," ",orange[0],orange[1],orange[2]);
	printf("%*s| %c %c %c |\n",8," ",orange[3],color_char[Orange],orange[4]);
	printf("%*s| %c %c %c |\n",8," ",orange[5],orange[6],orange[7]);
	printf("--------------------------------\n");
	

}

struct rubiks* u_mov(struct rubiks *rubiks)
{
	u_data->dir = CW;
	return turn(rubiks, u_data);
}
struct rubiks* up_mov(struct rubiks *rubiks)
{
	u_data->dir = CCW;
	return turn(rubiks, u_data);
}

struct rubiks* r_mov(struct rubiks *rubiks)
{
	r_data->dir = CW;
	return turn(rubiks, r_data);
}

struct rubiks* rp_mov(struct rubiks *rubiks)
{
	r_data->dir = CCW;
	return turn(rubiks, r_data);
}

struct rubiks* l_mov(struct rubiks *rubiks)
{
	l_data->dir = CW;
	return turn(rubiks, l_data);
}

struct rubiks* lp_mov(struct rubiks *rubiks)
{
	l_data->dir = CCW;
	return turn(rubiks, l_data);
}

struct rubiks* f_mov(struct rubiks *rubiks)
{
	f_data->dir = CW;
	return turn(rubiks, f_data);
}

struct rubiks* fp_mov(struct rubiks *rubiks)
{
	f_data->dir = CCW;
	return turn(rubiks, f_data);
}

struct rubiks* b_mov(struct rubiks *rubiks)
{
	b_data->dir = CW;
	return turn(rubiks, b_data);
}

struct rubiks* bp_mov(struct rubiks *rubiks)
{
	b_data->dir = CCW;
	return turn(rubiks, b_data);
}

struct rubiks* d_mov(struct rubiks *rubiks)
{
	d_data->dir = CW;
	return turn(rubiks, d_data);
}

struct rubiks* dp_mov(struct rubiks *rubiks)
{
	d_data->dir = CCW;
	return turn(rubiks, d_data);
}

