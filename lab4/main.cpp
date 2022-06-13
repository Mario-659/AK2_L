#include <iostream>
#include <time.h>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
 
struct Vector{
    float a, b, c, d;
};

Vector Matrix[4];

float getDeterminant(Vector A, Vector B, Vector C, Vector D);

void testMatrixDet();
void printMatrix();


int main(int argc, char const *argv[]) {
    cout << "Podaj liczby do macierzy oddzielajac ENTER:\n";

    for (int i=0; i<4; i++) {
        cin >> Matrix[i].a;
        cin >> Matrix[i].b;
        cin >> Matrix[i].c;
        cin >> Matrix[i].d;
    }

    printMatrix();
    float det = getDeterminant(Matrix[0], Matrix[1], Matrix[2], Matrix[3]);
    cout << "Obliczony wyznacznik macierzy: " << det << endl;

    // testMatrixDet();
    return 0;
}

float getDeterminant(Vector A, Vector B, Vector C, Vector D) {
    Vector result;

    /* 
                       I               II
             +-------------+ +-------------+
     detM1 = |(a0*b1-a1*b0)|*|(c2*d3-c3*d2)| +
             |(a2*b0-a0*b2)|*|(c1*d3-c3*d1)| +  => xmm3 4
             |(a0*b3-a3*b0)|*|(c1*d2-c2*d1)| +
             |(a1*b2-a2*b1)|*|(c0*d3-c3*d0)| +
             +-------------+ +-------------+
                   III              IV
             +-------------+ +-------------+
             |(a2*b3-a3*b2)|*|(c0*d1-c1*d0)| +
             |(a3*b1-a1*b3)|*|(c0*d2-c2*d0)|
    */


    asm(R"(
        movups %1, %%xmm0 # A
        movups %2, %%xmm1 # B
        shufps $0x48, %%xmm0, %%xmm0 # (a0, a1, a2, a3) -> (a0, a2, a0, a1)
        shufps $0xB1, %%xmm1, %%xmm1 # (b0, b1, b2, b3) -> (b1, b0, b3, b2)
        mulps %%xmm1, %%xmm0         # (a0, a1, a2, a3) x (b1, b0, b3, b2)

        movups %1, %%xmm1
        movups %2, %%xmm2
        shufps $0xB1, %%xmm1, %%xmm1
        shufps $0x48, %%xmm2, %%xmm2
        mulps %%xmm2, %%xmm1

        subps %%xmm1, %%xmm0

        movups %3, %%xmm1
        movups %4, %%xmm2
        shufps $0x16, %%xmm1, %%xmm1
        shufps $0xEF, %%xmm2, %%xmm2
        mulps %%xmm2, %%xmm1

        movups %3, %%xmm2
        movups %4, %%xmm3
        shufps $0xEF, %%xmm2, %%xmm2
        shufps $0x16, %%xmm3, %%xmm3
        mulps %%xmm3, %%xmm2

        subps %%xmm2, %%xmm1
        mulps %%xmm1, %%xmm0

        movups %1, %%xmm1
        movups %2, %%xmm2
        shufps $0xFE, %%xmm1, %%xmm1
        shufps $0xF7, %%xmm2, %%xmm2
        mulps %%xmm2, %%xmm1

        movups %1, %%xmm2
        movups %2, %%xmm3
        shufps $0xF7, %%xmm2, %%xmm2
        shufps $0xFE, %%xmm3, %%xmm3
        mulps %%xmm3, %%xmm2

        subps %%xmm2, %%xmm1

        movups %3, %%xmm2
        movups %4, %%xmm3
        shufps $0xF0, %%xmm2, %%xmm2
        shufps $0xF9, %%xmm3, %%xmm3
        mulps %%xmm3, %%xmm2

        movups %3, %%xmm3
        movups %4, %%xmm4
        shufps $0xF9, %%xmm3, %%xmm3
        shufps $0xF0, %%xmm4, %%xmm4
        mulps %%xmm4, %%xmm3

        subps %%xmm3, %%xmm2
        mulps %%xmm2, %%xmm1
        addps %%xmm1,%%xmm0

        movups %%xmm0, %0
    )"
        : "=m"(result)
        : "m"(A), "m"(B), "m"(C), "m"(D)
    );

    float det = 0.f;
    det += result.a;
    det += result.b;
    det += result.c;
    det += result.d;

    return det;
}

void testMatrixDet() {
    Matrix[0].a = 10.f;
    Matrix[0].b = 80.f;
    Matrix[0].c = 30.f;
    Matrix[0].d = 11.f;
    Matrix[1].a = 45.f;
    Matrix[1].b = 51.f;
    Matrix[1].c = 12.f;
    Matrix[1].d = 12.f;
    Matrix[2].a = 11.f;
    Matrix[2].b = 0.f;
    Matrix[2].c = 4.f;
    Matrix[2].d = 2.f;
    Matrix[3].a = 22.f;
    Matrix[3].b = 7.f;
    Matrix[3].c = 10.f;
    Matrix[3].d = 3.f;

    float expected = 33048.f;
    printMatrix();
    float det = getDeterminant(Matrix[0], Matrix[1], Matrix[2], Matrix[3]);
    if (det == expected) cout << "Passed. Determinant: " << det << endl;
    else cout << "Failed. Expected:" << expected << " Actual: " << det << endl;
}

void printMatrix() {
    cout << setw(6);
    cout << "Matrix:\n";
    for (int i=0; i<4; i++) {
        cout << setw(4) << Matrix[i].a << " | " << setw(4) << Matrix[i].b << " | " << setw(4) << Matrix[i].c << " | " << setw(4) << Matrix[i].d << endl;
    } 
}
