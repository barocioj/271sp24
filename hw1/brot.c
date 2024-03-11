#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <math.h>
 
// rip from https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C
// try "convert x.ppm x.png" and follow the install instructions to get a png

// In C, we make a 3 dimension unsigned char array of size * size * 3
// Hint - how many times should loop? How many times should you call malloc?
unsigned char ***create_base(int size)
{
	unsigned char **base = (unsigned char ***)malloc(size * sizeof(unsigned char **));
	for (int i = 0; i < size; i++)
	{
		base[i] = (unsigned char **)malloc(size * sizeof(unsqigned char *));
		for (int j = 0; j < size; j++)
		{
			base[i][j] = (unsigned char *)calloc(3, sizeof(unsigned char));
		}
	}
	return base;
}

// Calculate z_(n+1) = z_n^2 + c and return the result
// in C, we accept two complex values and produce a complex output
// I've included sample code to work with complex values.
// Hint - don't use exponentiation
double complex m_seq(double complex z_n, double complex c)
{
	return z_n * z_n + c;
}

// in C we accept a complex value and an integer size and two integer pointers, and populate the integer points with the x and y results
// I've included sample code to zero out x and y.
void c2b(double complex c, int size, int *x, int *y)
{
	    *x = (int)((creal(c) + 2) * size / 4);
	    *y = (int)((cimag(c) + 2) * size / 4);
	    *x = (*x < 0) ? 0 : ((*x >= size) ? size - 1 : *x);
	    *y = (*y < 0) ? 0 : ((*y >= size) ? size - 1 : *y);
}

// in C, we use b2c to loop over all pixels rather than relying on randomization
// return the complex associated with a location x, y
// I've included sample code to work with complex values.
double complex b2c(int size, int x, int y)
{
    return ((double)x * 4.0 / size - 2.0) + ((double)y * 4.0 / size - 2.0) * I;
}

// in C we accept a complex value, and integer number of iterations, and returns with an int that represents whether c escapes, or exceeds absolute value 2 in iters applications of m_seq.
// I included the absolute value sample code
int escapes(double complex c, int iters)
{
    double complex z_n = c;
    for (int i = 0; i < iters; i++)
    {
        z_n = m_seq(z_n, c);
        if (cabs(z_n) > 2)
            return 1;
    }
    return 0;
}

// in C, we accept a 3d array base, an integer for size and for iterations, a color channel of 0,1,2, and a complex value c
void one_val(unsigned char ***base, int size, int iters, int color, double complex c)
{
    if (!escapes(c, iters))
        return;

    double complex z_n = c;
    for (int i = 0; i < iters; i++)
    {
        if (cabs(z_n) > 2)
            return;

        int x, y;
        c2b(z_n, size, &x, &y);
        base[x][y][color] += 25;
        if (base[x][y][color] > 255)
            base[x][y][color] = 255;

        z_n = m_seq(z_n, c);
    }
}

// in C, we accept a 3d array base, an integer for size and for iterations
void get_colors(unsigned char ***base, int size, int iters)
{
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            double complex c = b2c(size, x, y);
            for (int i = 0; i < 3; i++)
            {
                one_val(base, size, (i + 1) * iters, i, c);
            }
        }
    }
}

// OPTIONAL
// That said, you images will look bad without this.
// The Python sample had a hacky solution.
// We accept a base, and equalize values to percentiles rather than counts
// You equalized images in CS 151 ImageShop.
void equalize(unsigned char ***base, int size)
{
    int histogram[3][256] = {0}; 
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            for (int color = 0; color < 3; color++)
            {
                histogram[color][base[x][y][color]]++;
            }
        }
    }

    int cdf[3][256] = {0};

    for (int color = 0; color < 3; color++)
    {
        cdf[color][0] = histogram[color][0];

        for (int i = 1; i < 256; i++)
        {
            cdf[color][i] = cdf[color][i - 1] + histogram[color][i];
        }
    }

    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            for (int color = 0; color < 3; color++)
            {
                int intensity = base[x][y][color];
                double equalized_intensity = (double)(cdf[color][intensity] - cdf[color][0]) /
                                              (size * size - cdf[color][0]) * 255;
                base[x][y][color] = (unsigned char)round(equalized_intensity);
            }
        }
    }
}
 
// Given an edge size and starting iteration count, make a buddhabrot.
// I'm leaving the ppm code
void make_brot(int size, int iters)
{
    unsigned char ***base = create_base(size);

    get_colors(base, size, iters);
    equalize(base, size); 

    FILE *fp = fopen("goya.ppm", "wb"); 
    fprintf(fp, "P6\n%d %d\n255\n", size, size);
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            fwrite(base[x][y], 1, 3, fp);
        }
    }
    fclose(fp);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            free(base[i][j]);
        }
        free(base[i]);
    }
    free(base);
}

 
int main()
{
	make_brot(4000,50);
	return 0;
}
