#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX_VALUE (unsigned short)65535
#define cost_white 10
#define cost_non_white 1

unsigned short *m0, *m1, *m2;
unsigned int m0_height = 0, m0_width = 0;
unsigned int height, width;
int nprocesses; 
int id;

// Struct that holds where in the image each process starts and ends
typedef struct Indices {
    unsigned int start;
    unsigned int end;
} Indices;

Indices *indices;

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
    fscanf(fp,"%d", &m0_width);
    fscanf(fp,"%d", &m0_height);
    while (getc(fp) != '\n');           /* assume max value is 65535 */
    fseek(fp, 4, SEEK_CUR);             /* 4 extra bytes of white space? */
    
    unsigned char* chars = (unsigned char*)malloc(sizeof(unsigned char) * m0_width * m0_height);
    fread(chars, sizeof(unsigned char), m0_width * m0_height, fp);

    m0 = (unsigned short *)malloc(sizeof(unsigned short) * m0_width * m0_height);
    for (int i = 0; i < m0_width * m0_height; i++)
    {
        if (chars[i] > 0) m0[i] = MAX_VALUE;       /* make it brightest value as possible */
        else m0[i] = 0;
    }
    free(chars);
    fclose(fp);
}

void save_image_ascii(unsigned short *m, unsigned int h, unsigned int w, char *filename)
{
    FILE *fp;
    fp = fopen(filename, "wb");

    fprintf(fp, "P2\n");    
    fprintf(fp, "%d %d\n", w, h);  
  
    unsigned short max_value = 0;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            unsigned short current = m[x + y * w];
            if (current > max_value)
                max_value = current;
        }
    }
    fprintf(fp, "%hu\n", max_value);  

    for (int i = 0; i < h; i++) { 
        for (int j = 0; j < w; j++) { 
            fprintf(fp, "%d ", m[j + i * w]); 
        } 
        fprintf(fp, "\n"); 
    } 
}

void initialize(int argc, char* argv[])
{
    if (argc <= 1)
        load_image("input.pgm");
    else
        load_image(argv[1]);
}

int transform(unsigned short *a, unsigned short *b)
{
    int has_changed = 0;

    // First and last lines are excess from other processes, therefore don't process them
    for (int y = 1; y < height - 1; y++)          
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
                        // Consider outside of lateral borders
                        if (x+j < 0 || x+j >= width)
                             continue;

                        min_aux = a[(x + j) + (y + i) * width];
                        if (min_aux < min)
                            min = min_aux;
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

void setup()
{
    int start_y = 0;
    int end_y = m0_height - 1;
    int has_white = 0;
    indices = (Indices*) malloc(sizeof(Indices) * nprocesses);
    
    //Find where image starts having white pixels so we can discard lines until it
    for (int y = 0; y < m0_height; y++)
    {
        for (int x = 0; x < m0_width; x++)
        {
            if (m0[x + y * m0_width] == MAX_VALUE) 
            {
                has_white = 1;
                break;
            }
        }
        if (has_white)
        {
            if (y != 0)
            {
                start_y = y - 1;
                break;
            }
        }
    }
    //Find where image ends having white pixels so we can discard lines after it
    has_white = 0;
    for (int y = m0_height - 1; y >= 0; y--)
    {
        for (int x = m0_width - 1; x >= 0; x--)
        {
            if (m0[x + y * m0_width] == MAX_VALUE) 
            {
                has_white = 1;
                break;
            }
        }
        if (has_white)
        {
            if (y != m0_height - 1)
            {
                end_y = y + 2;
                break;
            }
        }
    }
    
    //Determine cost of processing full image according to white to non white ratio
    unsigned int total_cost = 0;
    for (int y = start_y; y < end_y; y++)
        for (int x = 0; x < m0_width; x++)
            total_cost += m0[x + y * m0_width] == MAX_VALUE ? cost_white : cost_non_white;

    //Divide the cost for all of our processes
    int process_cost = total_cost / nprocesses;
    
    //Determine which range of lines go to each process by counting the cost of lines
    unsigned int current_cost = 0;
    unsigned int current_process = nprocesses - 1;
    unsigned int current_maxy = end_y;
    for (int y = end_y; y > start_y; y--)
    {
        for (int x = 0; x < m0_width; x++)
        {
            current_cost += m0[x + y * m0_width] == MAX_VALUE ? cost_white : cost_non_white;
        }
        if (current_cost > process_cost)
        {
            if (current_process > 0)
            {
                // Send each process their matrix height and width
                unsigned int nm_starty = y - 1;
                unsigned int nm_endy = current_maxy;
                unsigned int nm[2] = {nm_endy - nm_starty, m0_width};
                MPI_Send(nm, 2, MPI_UNSIGNED, current_process, 0, MPI_COMM_WORLD);
                
                // Now send the matrix data
                unsigned short *nm_ptr = &m0[nm_starty * m0_width];
                MPI_Send(nm_ptr, (nm_endy - nm_starty) * m0_width, MPI_UNSIGNED_SHORT, current_process, 0, MPI_COMM_WORLD);
                
                current_maxy = y;
                indices[current_process].end = nm_endy;
                indices[current_process].start = nm_starty;
                current_cost = 0;
                current_process--;
            }
        }
    }

    // For process zero, get the remaining lines
    unsigned int maxy = current_maxy + 1;
    height = maxy - start_y;
    width  = m0_width;
    m1 = (unsigned short*) malloc(sizeof(unsigned short) * height * width);
    memcpy(m1, &m0[start_y * width], sizeof(unsigned short) * height * width);
    m2 = (unsigned short *)malloc(sizeof(unsigned short) * height * width);
    memcpy(m2, &m0[start_y * width], sizeof(unsigned short) * height * width);
    
    indices[0].start = start_y;
    indices[0].end   = maxy;
}

int main(int argc, char **argv) {
    MPI_Status status;
    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &nprocesses); 
    MPI_Comm_rank (MPI_COMM_WORLD, &id); 

    if (id == 0) initialize(argc, argv);

    double time_start = MPI_Wtime();

    if (id == 0)
    {
        //double s = MPI_Wtime();
        setup();
        //save_image_ascii(m1, height, width, "0d.pgm");
        //double e = MPI_Wtime();
        //printf("Sequential part: %lf\n", e - s);
    }
    else
    {
        //Get size of portion of the image to process
        unsigned int nm[2] = {0};
        MPI_Recv(nm, 2, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, &status);
        height = nm[0];
        width = nm[1];

        //Assign portion of image
        m1 = (unsigned short*) malloc(sizeof(unsigned short) * height * width);
        MPI_Recv(m1, height * width, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, &status);
        //char name[10];
        //sprintf(name,"%dd.pgm",id);
        //save_image_ascii(m1, height, width, name);

        m2 = (unsigned short*) malloc(sizeof(unsigned short) * height * width);
        memcpy(m2, m1, sizeof(unsigned short) * height * width);
    }

    //if (id == 0)
    //{
    //    for (int i = 0; i < nprocesses; i++)
    //    printf("%d: %d %d\n", i, indices[i].start, indices[i].end);
    //}
    //printf("%d: %d\n", id, height);

    unsigned short *a, *b, *c;
    a = &m1[0];
    b = &m2[0];

    int active_top = 1;
    int active_bot = 1;
    int active_self = 1;

    //int i = 0;
    while(active_self)
    {
        //printf("========Iteration %d========", i);
        // If we finished processing, we still want to send messages to other processes
        // telling them we have finished and finally trading our shared lines
        //double s = MPI_Wtime();
        active_self = transform(a, b);
        //double e = MPI_Wtime();
        //printf("Transform %d %d: %lf\n", id, i, e - s);
        c = b;
        b = a;
        a = c;

        //s = MPI_Wtime();
        // Send message down if process underneath has not ended
        if (id != nprocesses - 1 && active_bot)
        {
            unsigned short *send_down = &b[(height-2) * width];
            MPI_Send(send_down, width, MPI_UNSIGNED_SHORT, id+1, active_self, MPI_COMM_WORLD);
            //printf("%d -> %d\n", id, id+1);

            MPI_Recv(b + (height - 1) * width, width, MPI_UNSIGNED_SHORT, id+1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            memcpy(a + (height - 1) * width, b + (height - 1) * width, sizeof(unsigned short) * width);
            //printf("%d <- %d\n", id, id+1);
            // Update status of process underneath
            active_bot = status.MPI_TAG;
        }

        // Send message up if process above has not ended
        if (id != 0 && active_top)
        {
            MPI_Recv(b, width, MPI_UNSIGNED_SHORT, id-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            //printf("%d <- %d\n", id, id-1);
            memcpy(a, b, sizeof(unsigned short) * width);
            // Update status of process above
            active_top = status.MPI_TAG;

            MPI_Send(b + width, width, MPI_UNSIGNED_SHORT, id-1, active_self, MPI_COMM_WORLD);
            //printf("%d -> %d\n", id, id-1);
        }
        //e = MPI_Wtime();
        //if (active_bot || active_top)
        //    printf("Communication %d %d: %lf\n", id, i, e - s);
        //i++;
    }
    
    // Up to process 0 to reconstruct the final image
    if (id == 0)
    {
        //double s = MPI_Wtime();
        unsigned short *start = &m1[width];
        unsigned short *m0_start = &m0[indices[0].start * width];
        
        memcpy(m0_start, start, sizeof(unsigned short) * (height - 2) * width);
        //save_image_ascii(start, height - 2, width, "0.pgm");
        // Receive others matrices and update ours
        for(int i = 1; i < nprocesses; i++)
        {
            // We don't have access to others' heights directly, so we calculate them
            int iheight = indices[i].end - indices[i].start - 1;
            MPI_Recv(m0 + indices[i].start * width, iheight * width, MPI_UNSIGNED_SHORT, i, 0, MPI_COMM_WORLD, &status);
        }
        //double e = MPI_Wtime();
        //printf("Reconstruction %d %d: %lf\n", id, i, e - s);
    }
    // Send everything to process 0 so it can reconstruct the final image    
    else
    {
        //char name[10];
        //sprintf(name,"%d.pgm",id);
        //save_image_ascii(m1 + width, height - 1, width, name);
        MPI_Send(m1 + width, width * (height - 1), MPI_UNSIGNED_SHORT, 0, 0, MPI_COMM_WORLD);
    }

    double time_end = MPI_Wtime();
    printf("Time for %d: %lf\n", id, time_end - time_start);
    //if (id == 0) save_image_ascii(m0, m0_height, m0_width, "output.pgm");

    free(m0);
    free(m1);
    free(m2);

    MPI_Finalize();
    return 0;
}
