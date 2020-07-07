#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <fstream>
#include <sstream> 
#include <vector> 
#include <sys/time.h>

using namespace std;

#define MAX_VALUE (unsigned short)65535     /* PGM file format only supports maximum of 16 bits */
#define REP 1
#define TIME_RESOLUTION 1000000		// time measuring resolution (us)

unsigned int height = 0, width = 0;
unsigned short *m1, *m2, *m0;
unsigned short *a, *b, *c;
int numthreads = 1;
int dynamicCores = 1;
int has_changed = 1;
long long unsigned initial_time;
timeval t;

void start (void) {
	gettimeofday(&t, NULL);
	initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

long long unsigned stop () {
	gettimeofday(&t, NULL);
	long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;

	return final_time - initial_time;
}

void transform(int rank)
{
    //int has_changed = 0;
    long thread_rank = (long) rank;
    long long slices = height/numthreads;
    long long thread_start = slices*thread_rank;
    long long thread_end = thread_start + slices;
    // Range from 1 to size - 1 to ignore borders
    // or range from 0 to size to consider them
    //#pragma omp parallel for schedule(dynamic, dynamicCores) num_threads(numthreads)
    for (int y = thread_start; y < thread_end; y++)          
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

    //return has_changed;
}

void load_image(string filename)
{
    int row = 0, col = 0;
    ifstream infile(filename);
    stringstream ss;
    string inputLine = "";

    // First line : version
    getline(infile,inputLine);
    if(inputLine.compare("P5") != 0) cout << "Version error " << inputLine << endl;
    else cout << "Version : " << inputLine << endl;

    // Second line : comment
    getline(infile,inputLine);
    cout << "Comment : " << inputLine << endl;

    // Continue with a stringstream
    ss << infile.rdbuf();
    // Third line : size
    ss >> width >> height;
    cout << width << " columns and " << height << " rows" << endl;
    int maxvalue = 0;
    ss >> maxvalue;
    cout << "Max value: " << maxvalue << endl;

    unsigned short* chars = (unsigned short *)malloc(sizeof(unsigned short) * width * height);

    // Following lines : data
    for(row = 0; row < height; ++row)
        for (col = 0; col < width; ++col) 
        {
            char temp;
            ss >> temp;
            chars[col + row * width] = (unsigned short)temp;
        }

    m0 = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
    for (int i = 0; i < width * height; i++)
    {
        if (chars[i] > 0) m0[i] = MAX_VALUE;       /* make it brightest value as possible */
        else m0[i] = 0;
    }
    free(chars);
    infile.close();
}


void save_image_ascii(unsigned short *m)
{
    ofstream file;
    string filename = "output.pgm";
    file.open(filename);

    if (!file)
    {
        cout << "Error: Unable to open file";
        exit(8);
    }
    file << "P2\n";
    file << width << " " << height << "\n";
  
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
    file << max_value << "\n";

    // Writing values
    for (int i = 0; i < height; i++) { 
        for (int j = 0; j < width; j++) { 
            file << m[j + i * width] << " ";
        } 
        file << "\n";
    } 
}

void initialize(int argc, char* argv[])
{
    /* Load image to first matrix */
    if (argc <= 1)
        load_image("input.pgm");
    else
    {
        string s(argv[1]);
        load_image(s);
    }

    if (argc > 1)
    {
        numthreads =  strtol(argv[2], NULL, 10); 
    }    
}


int main(int argc, char *argv[])
{
    initialize(argc, argv);
    cout << "Image Size: " << height << "\n";
    cout << "Num Threads: " << numthreads << "\n";

    std::vector<std::thread> thread_array;

    for (int i = 0; i < REP; i++)
    {
        m1 = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
        //memcpy(m1, m0, sizeof(unsigned short) * width * height);
        std::copy(m0, m0+(width*height), m1);

        m2 = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
        //memcpy(m2, m0, sizeof(unsigned short) * width * height);
        std::copy(m0, m0+(width*height), m2);
        
        //double start = omp_get_wtime();

        start();

        a = &m1[0];
        b = &m2[0];

        //double seq_time = omp_get_wtime() - start;;
        while(has_changed == 1)
        {
            //double start_inside = omp_get_wtime();
            has_changed = 0;
            //std::thread th(transform, 0);
            //std::thread th2(transform, 1);
            for (int t = 0; t < numthreads; t++)  
            {
                thread_array.push_back(std::thread(transform,t));
            }

            for (std::thread & th : thread_array)
            {
                if (th.joinable())
                    th.join();
            }

            thread_array.clear();
            

            /* Ping-pong matrices */
            c = b;
            b = a;
            a = c;

            //seq_time += omp_get_wtime() - start_inside;
        }
        //double finaltime = omp_get_wtime() - start;
        long long unsigned finaltime = stop();
        cout << "Run time: " << finaltime << endl;

        //printf("Result Sequetial %d: %lf\tResult Parallel %d: %lf\n", i+1, seq_time, i+1, finaltime);
        cout << "heres" << endl;
        if (i != REP - 1)
        {
            std::free(m1);
            std::free(m2);
        }
    }

    save_image_ascii(m1);
    std::free(m0);
    std::free(m1);
    std::free(m2);
    cout << "DONE" << endl;

    return 0;
}