#include <iostream>
#include <fstream>

#include <time.h>
#include <unistd.h>
#include <ostream>
#include <string>
#include <random>

#define VECTOR_SIZE 8192
#define NR_OF_MEASURES 10

using namespace std;

struct Vector {
    float a, b, c, d;
};

struct Vector Arr1[VECTOR_SIZE];
struct Vector Arr2[VECTOR_SIZE];

double avgTimeAdd = 0;
double avgTimeSub = 0;
double avgTimeMul = 0;
double avgTimeDiv = 0;

struct Vector addResults[VECTOR_SIZE];
struct Vector subResults[VECTOR_SIZE];
struct Vector mulResults[VECTOR_SIZE];
struct Vector divResults[VECTOR_SIZE];

random_device rd;
mt19937 generator(rd());
uniform_real_distribution<> dis(-1000.0, 1000.0);

void logResults(const string type);
void fillArr();

void testSIMD();
void testSISD();

int main() {
    // init pseudo-random number generator with current time 
    // and fill Arr1 and Arr2 with random values
    srand(time(NULL));
    fillArr();

    testSIMD();
    logResults("SIMD");

    testSISD();
    logResults("SISD");
}

float getRand() { return dis(generator); }

void fillArr() {
    for (int i=0; i<VECTOR_SIZE; i++) {
        Arr1[i].a = getRand();
        Arr1[i].b = getRand();
        Arr1[i].c = getRand();
        Arr1[i].d = getRand();
        Arr2[i].a = getRand();
        Arr2[i].b = getRand();
        Arr2[i].c = getRand();
        Arr2[i].d = getRand();
    }
}

void addSIMD(int i);
void subSIMD(int i);
void mulSIMD(int i);
void divSIMD(int i);

void testSIMD() {
    time_t start, end;
    double sumTime = 0;

    for (int i=0; i<NR_OF_MEASURES; i++) {
        for (int j=0; j<VECTOR_SIZE; j++) {
            start = clock();
            addSIMD(j);
            end = clock();
            sumTime += double(end - start);
        }
    }
    avgTimeAdd = sumTime / double(NR_OF_MEASURES);
    avgTimeAdd /= double(CLOCKS_PER_SEC);
    
    sumTime = 0;
    for (int i=0; i<NR_OF_MEASURES; i++) {
        for (int j=0; j<VECTOR_SIZE; j++) {
            start = clock();
            subSIMD(j);
            end = clock();
            sumTime += double(end - start);
        }
    }
    avgTimeSub = sumTime / double(NR_OF_MEASURES);
    avgTimeSub /= double(CLOCKS_PER_SEC);

    sumTime = 0;
    for (int i=0; i<NR_OF_MEASURES; i++) {
        for (int j=0; j<VECTOR_SIZE; j++) {
            start = clock();
            mulSIMD(j);
            end = clock();
            sumTime += double(end - start);
        }
    }
    avgTimeMul = sumTime / double(NR_OF_MEASURES);
    avgTimeMul /= double(CLOCKS_PER_SEC);

    sumTime = 0;
    for (int i=0; i<NR_OF_MEASURES; i++) {
        for (int j=0; j<VECTOR_SIZE; j++) {
            start = clock();
            divSIMD(j);
            end = clock();
            sumTime += double(end - start);
        }
    }
    avgTimeDiv = sumTime / double(NR_OF_MEASURES);
    avgTimeDiv /= double(CLOCKS_PER_SEC);
}

void addSISD(int i);
void subSISD(int i);
void mulSISD(int i);
void divSISD(int i);

void testSISD() {
    time_t start, end;
    double sumTime = 0;

    for (int i=0; i<NR_OF_MEASURES; i++) {
        for (int j=0; j<VECTOR_SIZE; j++) {
            start = clock();
            addSISD(j);
            end = clock();
            sumTime += double(end - start);
        }
    }
    avgTimeAdd = sumTime / double(NR_OF_MEASURES);
    avgTimeAdd /= double(CLOCKS_PER_SEC);
    
    sumTime = 0;
    for (int i=0; i<NR_OF_MEASURES; i++) {
        for (int j=0; j<VECTOR_SIZE; j++) {
            start = clock();
            subSISD(j);
            end = clock();
            sumTime += double(end - start);
        }
    }
    avgTimeSub = sumTime / double(NR_OF_MEASURES);
    avgTimeSub /= double(CLOCKS_PER_SEC);

    sumTime = 0;
    for (int i=0; i<NR_OF_MEASURES; i++) {
        for (int j=0; j<VECTOR_SIZE; j++) {
            start = clock();
            mulSISD(j);
            end = clock();
            sumTime += double(end - start);
        }
    }
    avgTimeMul = sumTime / double(NR_OF_MEASURES);
    avgTimeMul /= double(CLOCKS_PER_SEC);

    sumTime = 0;
    for (int i=0; i<NR_OF_MEASURES; i++) {
        for (int j=0; j<VECTOR_SIZE; j++) {
            start = clock();
            divSISD(j);
            end = clock();
            sumTime += double(end - start);
        }
    }
    avgTimeDiv = sumTime / double(NR_OF_MEASURES);
    avgTimeDiv /= double(CLOCKS_PER_SEC);
}

void logResultsStream(ostream& stream, const string type) {
    stream << "Typ obliczen: " << type << "\n";
    stream << "Liczba liczb: " << VECTOR_SIZE << "\n";
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
    string filename = type + "_" + to_string(VECTOR_SIZE) + ".txt";
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

/* ////////////////////////////////
           Assembly 
*/ ///////////////////////////////

void addSIMD(int i) {
    asm(R"(
        movaps %1, %%xmm0
        movaps %2, %%xmm1
        addps %%xmm1, %%xmm0
        movaps %%xmm0, %0
    )"
        :"=rm" (addResults[i])  // output
        :"rm" (Arr1[i]),        // inputs
        "rm" (Arr2[i]));
}

void subSIMD(int i) {
    asm(R"(
        movaps %1, %%xmm0
        movaps %2, %%xmm1
        subps %%xmm1, %%xmm0
        movaps %%xmm0, %0
    )"
        :"=rm" (subResults[i])
        :"rm" (Arr1[i]),
        "rm" (Arr2[i]));
}

void mulSIMD(int i) {
    asm(R"(
        movaps %1, %%xmm0
        movaps %2, %%xmm1
        mulps %%xmm1, %%xmm0
        movaps %%xmm0, %0
    )"        
        :"=rm" (mulResults[i])
        :"rm" (Arr1[i]),
        "rm" (Arr2[i]));
}

void divSIMD(int i) {
    asm(R"(
        movaps %1, %%xmm0
        movaps %2, %%xmm1
        subps %%xmm1, %%xmm0
        movaps %%xmm0, %0
    )" 
        :"=rm" (divResults[i])
        :"rm" (Arr1[i]),
        "rm" (Arr2[i]));
}

void addSISD(int i) {
    asm(R"(
        fldl %4
        faddl %5
        fstpl %0
        fldl %6
        faddl %7
        fstpl %1
        fldl %8
        faddl %9
        fstpl %2
        fldl %10
        faddl %11
        fstpl %3
    )"
        :
        "=m" (addResults[i].a),
        "=m" (addResults[i].b),
        "=m" (addResults[i].c),
        "=m" (addResults[i].d)
        :
        "g" (Arr1[i].a),
        "g" (Arr2[i].a),
        "g" (Arr1[i].b),
        "g" (Arr2[i].b),
        "g" (Arr1[i].c),
        "g" (Arr2[i].c),
        "g" (Arr1[i].d),
        "g" (Arr2[i].d));
}


void subSISD(int i) {
    asm(R"(
        fldl %4
        fsubl %5
        fstpl %0
        fldl %6
        fsubl %7
        fstpl %1
        fldl %8
        fsubl %9
        fstpl %2
        fldl %10
        fsubl %11
        fstpl %3
    )"
        :
        "=m" (subResults[i].a),
        "=m" (subResults[i].b),
        "=m" (subResults[i].c),
        "=m" (subResults[i].d)
        :
        "g" (Arr1[i].a),
        "g" (Arr2[i].a),
        "g" (Arr1[i].b),
        "g" (Arr2[i].b),
        "g" (Arr1[i].c),
        "g" (Arr2[i].c),
        "g" (Arr1[i].d),
        "g" (Arr2[i].d));
}

void mulSISD(int i) {
    asm(R"(
        fldl %4
        fmull %5
        fstpl %0
        fldl %6
        fmull %7
        fstpl %1
        fldl %8
        fmull %9
        fstpl %2
        fldl %10
        fmull %11
        fstpl %3
    )"
        :
        "=m" (subResults[i].a),
        "=m" (subResults[i].b),
        "=m" (subResults[i].c),
        "=m" (subResults[i].d)
        :
        "g" (Arr1[i].a),
        "g" (Arr2[i].a),
        "g" (Arr1[i].b),
        "g" (Arr2[i].b),
        "g" (Arr1[i].c),
        "g" (Arr2[i].c),
        "g" (Arr1[i].d),
        "g" (Arr2[i].d));
}


void divSISD(int i) {
    asm(R"(
        fldl %4
        fdivl %5
        fstpl %0
        fldl %6
        fdivl %7
        fstpl %1
        fldl %8
        fdivl %9
        fstpl %2
        fldl %10
        fdivl %11
        fstpl %3
    )"
        :
        "=m" (divResults[i].a),
        "=m" (divResults[i].b),
        "=m" (divResults[i].c),
        "=m" (divResults[i].d)
        :
        "g" (Arr1[i].a),
        "g" (Arr2[i].a),
        "g" (Arr1[i].b),
        "g" (Arr2[i].b),
        "g" (Arr1[i].c),
        "g" (Arr2[i].c),
        "g" (Arr1[i].d),
        "g" (Arr2[i].d));
}