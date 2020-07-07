#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_VALUE (unsigned short)65535     /* PGM file format only supports maximum of 16 bits */
#define REP 20

unsigned int height = 0, width = 0;
unsigned short *m0, *m1, *m2;

int transform(unsigned short *a, unsigned short *b)
{
    int has_changed = 0;

    // Range from 1 to size - 1 to ignore borders
    // or range from 0 to size to consider them
    for (int y = 0; y < height; y++)          
    {
        for (int x = 0; x < width; x++)      
        {
            unsigned short *value_a = &a[x + y * width];
            unsigned short *value_b = &b[x + y * width];

            if (*value_a == MAX_VALUE)
            {
                unsigned short min = MAX_VALUE;
                unsigned short min_aux = 0;

                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        // Ignore stencil center
                        if (!(i == 0 && j == 0))
                        {
                            // Consider outside of borders
                            if (x+j < 0 || x+j >= width || y+i < 0 || y+i >= height)
                                continue;

                            min_aux = a[(x + j) + (y + i) * width];
                            if (min_aux < min)
                                min = min_aux;
                        }
                    }
                }

                if (min != MAX_VALUE)
                {
                    *value_b = min + 1;
                    has_changed = 1;
                }
            }
            else if (*value_b != *value_a)
                *value_b = *value_a;
        }
    }

    return has_changed;
}

void load_image(char* filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(8);
    }

    char ch = getc(fp);
    if(ch != 'P')
    {
        printf("Error: Not valid pgm/ppm file type\n");
        exit(1);
    }

    while(getc(fp) != '\n');            /* skip to end of line*/
  
    while (getc(fp) == '#')             /* skip comment lines */
    {
      while (getc(fp) != '\n');         /* skip to end of comment line */
    }

    fseek(fp, -1, SEEK_CUR);            /* backup one character */
    fscanf(fp,"%d", &width);
    fscanf(fp,"%d", &height);
    while (getc(fp) != '\n');           /* assume max value is 65535 */
    fseek(fp, 4, SEEK_CUR);             /* 4 extra bytes of white space? */
    
    unsigned char* chars = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
    fread(chars, sizeof(unsigned char), width * height, fp);

    m0 = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
    for (int i = 0; i < width * height; i++)
    {
        if (chars[i] > 0) m0[i] = MAX_VALUE;       /* make it brightest value as possible */
        else m0[i] = 0;
    }
    free(chars);
    fclose(fp);
}

void save_image_ascii(unsigned short *m)
{
    FILE *fp;
    char *filename = "output.pgm";
    fp = fopen(filename, "wb");

    fprintf(fp, "P2\n");    
    fprintf(fp, "%d %d\n", width, height);  
  
    // Writing the maximum gray value
    unsigned short max_value = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned short current = m[x + y * width];
            if (current > max_value)
                max_value = current;
        }
    }
    fprintf(fp, "%hu\n", max_value);  

    // Writing values
    for (int i = 0; i < height; i++) { 
        for (int j = 0; j < width; j++) { 
            fprintf(fp, "%d ", m[j + i * width]); 
        } 
        fprintf(fp, "\n"); 
    } 
}

void initialize(int argc, char* argv[])
{
    /* Load image to first matrix */
    if (argc <= 1)
        load_image("../img512.pgm");
    else
        load_image(argv[1]);
}

void main(int argc, char *argv[])
{
    initialize(argc, argv);
    printf("Image Size: %d\n", height);
    printf("Sequential\n");
    double results[REP] = {0};

    for (int i = 0; i < REP; i++)
    {
        m1 = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
        memcpy(m1, m0, sizeof(unsigned short) * width * height);

        m2 = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
        memcpy(m2, m0, sizeof(unsigned short) * width * height);
        
        double start = omp_get_wtime();

        unsigned short *a, *b, *c;
        a = &m1[0];
        b = &m2[0];
        
        while(transform(a, b))
        {
            /* Ping-pong matrices */
            c = b;
            b = a;
            a = c;
        }

        double finaltime = omp_get_wtime() - start;

        results[i] = finaltime;
        //results[i] = finaltime;

        printf("Result %d: %lf\n", i+1, finaltime);

        if (i != REP - 1)
        {
            free(m1);
            free(m2);
        }
    }

    save_image_ascii(m1);
    free(m0);
    free(m1);
    free(m2);
}
