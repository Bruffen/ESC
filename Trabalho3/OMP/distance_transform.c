#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_VALUE (unsigned short)65535     /* PGM file format only supports maximum of 16 bits */
#define REP 1


unsigned int height = 0, width = 0;
unsigned short *m0, *m1, *m2;
int numthreads = 1;
int dynamicCores = 1;

void print_matrix(unsigned short *m)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            printf("%4d", m[x + y * width]);
        }
        printf("\n");
    }
    printf("________________________\n");
}

int transform(unsigned short *a, unsigned short *b)
{
    int has_changed = 0;

    // Range from 1 to size - 1 to ignore borders
    // or range from 0 to size to consider them
    //#pragma omp parallel num_threads(numthreads)
    //{
        #pragma omp parallel for schedule(dynamic, dynamicCores) num_threads(numthreads)
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
        //}
    }

    /* Debug */
    /*
    print_matrix(a);
    //print_matrix(b);
    printf("\n\n");
    */
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

void save_image(unsigned short *m)
{
    FILE *fp;
    char *filename = "output.pgm";
    fp = fopen(filename, "w");
    unsigned short max_value = 0;

    // Get brightest value
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned short current = m[x + y * width];
            if (current > max_value)
                max_value = current;
        }
    }

    //max_value = -1;
    //max_value = MAX_VALUE;

    // Header
    fprintf(fp,"P5\n%s\n%d %d\n%d\n", "#", width, height, max_value);

    /* Write values as 8 bit*/
    if (max_value < 256) {
        for (int i = 0; i < width * height; i++)
            fputc(m[i], fp);
    }
    /* Write values as 16 bit*/
    else {
        fwrite(m, sizeof(unsigned short), width * height, fp);
    }

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

void save_results(double *results, double *results_seq, int repetitions)
{
    printf("Image Size: %d\n", height);
    printf("Num Threads: %d\n", numthreads);

    for (int i = 0; i < repetitions; i++)
    {
        printf("Result Sequetial %d: %lf\tResult Parallel %d: %lf\n", i+1, results_seq[i], i+1, results[i]);
    }
}


void initialize(int argc, char* argv[])
{
    /* Load image to first matrix */
    if (argc <= 1)
        load_image("img512.pgm");
    else
        load_image(argv[1]);

    if (argc >= 1)
    {
        char *c = argv[2];
        numthreads = atoi(c);
    }    

    if (argc >= 2)
    {
        char *b = argv[3];
        dynamicCores = atoi(b);
    }    
}

void main(int argc, char *argv[])
{
    double results[REP];
    double results_seq[REP];

    initialize(argc, argv);
    int count = 0;

    for (int i = 0; i < REP; i++)
    {
        /* Copy values to second matrix */
        m2 = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
        memcpy(m2, m0, sizeof(unsigned short) * width * height);

        m1 = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
        memcpy(m1, m0, sizeof(unsigned short) * width * height);
        
        double start = omp_get_wtime();

        unsigned short *a, *b, *c;
        a = &m1[0];
        b = &m2[0];

        double seq_time = omp_get_wtime() - start;;

        while(transform(a, b))
        {
            double start_inside = omp_get_wtime();
            /* Ping-pong matrices */
            c = b;
            b = a;
            a = c;

            seq_time += omp_get_wtime() - start_inside;
        }
        double finaltime = omp_get_wtime() - start;
        results[i] = finaltime;
        results_seq[i] = seq_time;

        if (i != REP - 1)
        {
            free(m1);
            free(m2);
        }
    }

    save_results(results, results_seq, REP);
    /* Both matrices at the end are equal so just use the first one */
    //print_matrix(m1);
    save_image_ascii(m1);
    free(m1);
    free(m2);
}
