/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    25.05.2023
 * @brief   Generate gamma correction tables
 */

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Main function of gamma correction table generator
 *
 * @param argc Number of command line arguments
 * @param argv Command line arguments array
 * @return Exit code
 */
int main(int argc, char *argv[])
{
	double gamma_value;
	int i, max_in, max_out;

	if(argc != 2)
	{
		fprintf(stderr, "Usage: ./gamma correction-factor (eg. 0.4545)\n");
		return 1;
	}

	gamma_value = atof(argv[1]);
	max_in = 255;
	max_out = 255;

	for(i = 0; i <= max_in; ++i)
	{
		if(i > 0)
		{
			printf(", ");
		}

		if((i % 16) == 0)
		{
			printf("\n");
		}

		printf("%3d", (int)(pow((double)i / (double)max_in, gamma_value) * max_out + 0.5));
	}

	printf("\n\n");
	return 0;
}
