#include "rubiks.h"
#include <stdlib.h>

int main(int argc, char ** argv)
{
	struct rubiks *init = malloc(sizeof(struct rubiks));
	initialize_rubiks(init);
	struct rubiks *rubiks = malloc(sizeof(struct rubiks));
	initialize_rubiks(rubiks);
}
