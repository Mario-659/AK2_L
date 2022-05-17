#include <iostream>
#include <time.h>
#include <string>
#include <fstream>

#define SIZE 2048
#define REPS 10

using namespace std;
 
struct Vector{
    float a, b, c, d;
};

// Arrays with vectors
Vector Arr1[SIZE];
Vector Arr2[SIZE];

// Array with results
Vector Results[SIZE];

time_t start, stop;
double avgTimeAdd = 0;
double avgTimeSub = 0;
double avgTimeMul = 0;
double avgTimeDiv = 0;

// Fills arrays with random values
void fillRandom();

// SISD and SIMD operations, return clocks
double add_SIMD(int i);
double sub_SIMD(int i);
double mul_SIMD(int i);
double div_SIMD(int i);
double add_SISD(int i);
double sub_SISD(int i);
double mul_SISD(int i);
double div_SISD(int i);

// test operations and store average time to double avgTime...
void testSIMD();
void testSISD();

// calculate time in [s] based on clocks and REPS, store it in time
void storeTime(double clocks, double& time);

// logs current value of avgTime to console and file
void logResults(const string type);

// returns vector as a string
string toString(Vector vector);

// returns number of clocks from start to stop
double getClocks(time_t start, time_t stop);

int main() {
    // seed with current time
    srand(time(NULL));
    fillRandom();

    testSIMD();
    logResults("SIMD");

    testSISD();
    logResults("SISD");
}

void testSIMD() {
    double sumClocks = 0;
    for (int i=0; i<REPS; i++) {
        for(int j=0; j<SIZE; j++) {
            sumClocks += add_SIMD(j);
        }
    }
    storeTime(sumClocks, avgTimeAdd);

    sumClocks = 0;
    for (int i=0; i<REPS; i++) {
        for(int j=0; j<SIZE; j++) {
            sumClocks += sub_SIMD(j);
        }
    }
    storeTime(sumClocks, avgTimeSub);

    sumClocks = 0;
    for (int i=0; i<REPS; i++) {
        for(int j=0; j<SIZE; j++) {
            sumClocks += mul_SIMD(j);
        }
    }
    storeTime(sumClocks, avgTimeMul);

    sumClocks = 0;
    for (int i=0; i<REPS; i++) {
        for(int j=0; j<SIZE; j++) {
            sumClocks += div_SIMD(j);
        }
    }
    storeTime(sumClocks, avgTimeDiv);
}

void testSISD() {    
    double sumClocks = 0;
    for (int i=0; i<REPS; i++) {
        for(int j=0; j<SIZE; j++) {
            sumClocks += add_SISD(j);
        }
    }
    storeTime(sumClocks, avgTimeAdd);

    sumClocks = 0;
    for (int i=0; i<REPS; i++) {
        for(int j=0; j<SIZE; j++) {
            sumClocks += sub_SISD(j);
        }
    }
    storeTime(sumClocks, avgTimeSub);

    sumClocks = 0;
    for (int i=0; i<REPS; i++) {
        for(int j=0; j<SIZE; j++) {
            sumClocks += mul_SISD(j);
        }
    }
    storeTime(sumClocks, avgTimeMul);

    sumClocks = 0;
    for (int i=0; i<REPS; i++) {
        for(int j=0; j<SIZE; j++) {
            sumClocks += div_SISD(j);
        }
    }
    storeTime(sumClocks, avgTimeDiv);
}


/*//////////////////////////////
            Assembly
//////////////////////////////*/

double add_SIMD(int i) {
    start = clock();
    asm(R"(
        movaps %1, %%xmm0
        movaps %2, %%xmm1
        addps  %%xmm1, %%xmm0
        movaps %%xmm0, %0
    )"
    :"=m" (Results[i]) // 0 -> output
    :
    "m" (Arr1[i]),       // 1 -> input
    "m" (Arr2[i])        // 2 -> input
    );
    stop = clock();
    return getClocks(start, stop);
}

double sub_SIMD(int i) {
    start = clock();
    asm(R"(
        movaps %1, %%xmm0
        movaps %2, %%xmm1
        subps  %%xmm1, %%xmm0
        movaps %%xmm0, %0
    )"
    :"=m" (Results[i]) // 0 -> output
    :
    "m" (Arr1[i]),       // 1 -> input
    "m" (Arr2[i])        // 2 -> input
    );
    stop = clock();
    return getClocks(start, stop);
}

double mul_SIMD(int i) {
    start = clock();
    asm(R"(
        movaps %1, %%xmm0
        movaps %2, %%xmm1
        mulps  %%xmm1, %%xmm0
        movaps %%xmm0, %0
    )"
    :"=m" (Results[i]) // 0 -> output
    :
    "m" (Arr1[i]),       // 1 -> input
    "m" (Arr2[i])        // 2 -> input
    );
    stop = clock();
    return getClocks(start, stop);
}

double div_SIMD(int i) {
    start = clock();
    asm(R"(
        movaps %1, %%xmm0
        movaps %2, %%xmm1
        divps  %%xmm1, %%xmm0
        movaps %%xmm0, %0
    )"
    :"=m" (Results[i]) // 0 -> output
    :
    "m" (Arr1[i]),       // 1 -> input
    "m" (Arr2[i])        // 2 -> input
    );
    stop = clock();
    return getClocks(start, stop);
}

double add_SISD(int i) {
    start = clock();
    asm(R"(
        fld %4
        fld %8
        faddp
        fstp %0

        fld %5
        fld %9
        faddp
        fstp %1

        fld %6
        fld %10
        faddp
        fstp %2

        fld %7
        fld %11
        faddp
        fst %3
    )"
    :  // outputs
    "=m" (Results[i].a),    // 0
    "=m" (Results[i].b),    // 1
    "=m" (Results[i].c),    // 2
    "=m" (Results[i].d)     // 3
    :  // inputs
    "g" (Arr1[i].a),        // 4
    "g" (Arr1[i].b),        // 5
    "g" (Arr1[i].c),        // 6
    "g" (Arr1[i].d),        // 7
    "g" (Arr2[i].a),        // 8
    "g" (Arr2[i].b),        // 9
    "g" (Arr2[i].c),        // 10
    "g" (Arr2[i].d)         // 11
    );
    stop = clock();
    return getClocks(start, stop);
}

double sub_SISD(int i) {
    start = clock();
    asm(R"(
        fld %4
        fld %8
        fsubp
        fstp %0

        fld %5
        fld %9
        fsubp
        fstp %1

        fld %6
        fld %10
        fsubp
        fstp %2

        fld %7
        fld %11
        fsubp
        fst %3
    )"
    :  // outputs
    "=m" (Results[i].a),    // 0
    "=m" (Results[i].b),    // 1
    "=m" (Results[i].c),    // 2
    "=m" (Results[i].d)     // 3
    :  // inputs
    "g" (Arr1[i].a),        // 4
    "g" (Arr1[i].b),        // 5
    "g" (Arr1[i].c),        // 6
    "g" (Arr1[i].d),        // 7
    "g" (Arr2[i].a),        // 8
    "g" (Arr2[i].b),        // 9
    "g" (Arr2[i].c),        // 10
    "g" (Arr2[i].d)         // 11
    );
    stop = clock();
    return getClocks(start, stop);
}

double mul_SISD(int i) {
    start = clock();
    asm(R"(
        fld %4
        fld %8
        fmulp
        fstp %0

        fld %5
        fld %9
        fmulp
        fstp %1

        fld %6
        fld %10
        fmulp
        fstp %2

        fld %7
        fld %11
        fmulp
        fst %3
    )"
    :  // outputs
    "=m" (Results[i].a),    // 0
    "=m" (Results[i].b),    // 1
    "=m" (Results[i].c),    // 2
    "=m" (Results[i].d)     // 3
    :  // inputs
    "g" (Arr1[i].a),        // 4
    "g" (Arr1[i].b),        // 5
    "g" (Arr1[i].c),        // 6
    "g" (Arr1[i].d),        // 7
    "g" (Arr2[i].a),        // 8
    "g" (Arr2[i].b),        // 9
    "g" (Arr2[i].c),        // 10
    "g" (Arr2[i].d)         // 11
    );
    stop = clock();
    return getClocks(start, stop);
}

double div_SISD(int i) {
    start = clock();
    asm(R"(
        fld %4
        fld %8
        fdivp
        fstp %0

        fld %5
        fld %9
        fdivp
        fstp %1

        fld %6
        fld %10
        fdivp
        fstp %2

        fld %7
        fld %11
        fdivp
        fst %3
    )"
    :  // outputs
    "=m" (Results[i].a),    // 0
    "=m" (Results[i].b),    // 1
    "=m" (Results[i].c),    // 2
    "=m" (Results[i].d)     // 3
    :  // inputs
    "g" (Arr1[i].a),        // 4
    "g" (Arr1[i].b),        // 5
    "g" (Arr1[i].c),        // 6
    "g" (Arr1[i].d),        // 7
    "g" (Arr2[i].a),        // 8
    "g" (Arr2[i].b),        // 9
    "g" (Arr2[i].c),        // 10
    "g" (Arr2[i].d)         // 11
    );
    stop = clock();
    return getClocks(start, stop);
}


/*//////////////////////////////
            Utils
//////////////////////////////*/

void storeTime(double clocks, double& time) {
    time = clocks / double(REPS);
    time /= double(CLOCKS_PER_SEC);
}

float getRandom() {
    return float(rand() / 100000);
}

void fillRandom() {
    for (int i=0; i<SIZE; i++) {
        Arr1[i].a = getRandom();
        Arr1[i].b = getRandom();
        Arr1[i].c = getRandom();
        Arr1[i].d = getRandom();
        Arr2[i].a = getRandom();
        Arr2[i].b = getRandom();
        Arr2[i].c = getRandom();
        Arr2[i].d = getRandom();
    }
}

double getClocks(time_t start, time_t stop) {
    return double(stop - start);
}

void logResultsStream(ostream& stream, const string type) {
    stream << "Typ obliczen: " << type << "\n";
    stream << "Liczba liczb: " << SIZE << "\n";
    stream << "Sredni czas: [s]:" << "\n";
    stream << "+ " << avgTimeAdd << "\n";
    stream << "- " << avgTimeSub << "\n";
    stream << "* " << avgTimeMul << "\n";
    stream << "/ " << avgTimeDiv << "\n";
}

void logResultsConsole(const string type) {
    logResultsStream(cout, type);
}

void logResultsFile(const string type) {
    fstream file;
    string filename = type + "_" + to_string(SIZE) + ".txt";
    file.open(filename, fstream::out);
    if(file.good()){
        logResultsStream(file, type);
    }
    else cout << "Error while opening file" << endl;
    
    file.close();
}

void logResults(const string type) {
    logResultsConsole(type);
    logResultsFile(type);
}

string toString(Vector vector) {    
    string text = "a: ";
    text += to_string(vector.a);
    text += " , b: ";
    text += to_string(vector.b);
    text += ", c: ";
    text += to_string(vector.c);
    text += ", d: ";
    text += to_string(vector.d);
    return  text;
}
