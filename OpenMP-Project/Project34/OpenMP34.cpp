#include <omp.h>
#include <chrono>
#include <iostream>
using namespace std;

const int N = 4500000;

int* a = new int[N];
int* b = new int[N];

uint32_t state = 777;
char myRand()
{
    state = state * 1664525 + 1013904223;
    return state >> 24;
}

// ����� �������� Max(A[i]+B[i], 4A[i]-B[i]), ������� 1
void main()
{
    srand(time(0));
    int i, sum = 0, A, B, C;

    for (i = 0; i < N; i++) {
        a[i] = (int)myRand(); //a[i] = rand() % 10 - 5;
        b[i] = (int)myRand(); //b[i] = rand() % 10 - 5;
    }

    // ������������ ��������
    auto startPar = chrono::system_clock::now();
#pragma omp parallel for shared(a, b) private(i, A, B, C) reduction(+: sum)
    for (i = 0; i < N; i++)
    {
        A = a[i] + b[i];
        B = 4 * a[i] - b[i];
        C = max(A, B);
        if (C > 1) {
        //#pragma omp atomic
            sum += C;
        }
    }
    auto stopPar = chrono::system_clock::now();
    cout << "\n  SumPar: " << sum;

    sum = 0;
    // �������� ��� �����������������
    auto startNoPar = chrono::system_clock::now();
    for (i = 0; i < N; i++)
    {
        A = a[i] + b[i];
        B = 4 * a[i] - b[i];
        C = max(A, B);
        if (C > 1) {
            sum += C;
        }
        cout << "  i = " << i 
            << "  a[i]+b[i] = " << a[i] + b[i]
            << "  4*a[i]-b[i] = " << 4 * a[i] - b[i]
            << "\t  sum = " << sum << "\n";
    }
    auto stopNoPar = chrono::system_clock::now();
    cout << "\n  SumNoPar: " << sum << endl;

    chrono::duration<double> elapsPar = (stopPar - startPar);
    chrono::duration<double> elapsNoPar = (stopNoPar - startNoPar);

    cout << "\n  Parallel End: " << elapsPar.count();
    cout << "\n  NoParallel End: " << elapsNoPar.count() << endl << endl;

    delete[] a;
    delete[] b;
}