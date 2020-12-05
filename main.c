#include "rubiks.h"

int main(int argc, char** argv)
{

	struct rubiks *rubiks = malloc(sizeof(struct rubiks));
	initialize_rubiks(rubiks);
	initialize_faces(rubiks);
	initialize_turndata();

	print_cube(rubiks);
	rubiks = dp_mov(rubiks);
	print_cube(rubiks);
	//rubiks = fp_mov(rubiks);
	//print_cube(rubiks);

	//print_cube(rubiks);
	//rubiks = l_mov(rubiks);
	//print_cube(rubiks);
	//rubiks = r_mov(rubiks);
	//print_cube(rubiks);
	//rubiks = r_mov(rubiks);
	//print_cube(rubiks);
	//rubiks = f_mov(rubiks);
	//print_cube(rubiks);
	//rubiks = d_mov(rubiks);
	//print_cube(rubiks);

	

	return 0;
}
