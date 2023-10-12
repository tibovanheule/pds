// Your name: ...

/***************************************************************************
 *   Copyright (C) 2012-2016 Jan Fostier (jan.fostier@ugent.be)            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <cmath>

using namespace std;
using namespace std::chrono;

// ==========================================================================
// TIMING ROUTINES
// ==========================================================================

time_point<system_clock> prevTime;

void startChrono()
{
        prevTime = system_clock::now();
}

double stopChrono()
{
        duration<double> elapsed = system_clock::now() - prevTime;
        return (elapsed.count());
}


// ==========================================================================
// MATRIX-VECTOR PRODUCT IMPLEMENTATION
// ==========================================================================

/**
 * Perform a matrix-matrix multiplication C = A*B using a blocked algorithm
 * @param C Pointer to the first element of the m by n matrix C, stored in column major format (output)
 * @param A Pointer to the first element of the m by p matrix A, stored in column major format
 * @param B Pointer to the first element of the p by n matrix B, stored in column major format
 * @param m Number of rows of C and A
 * @param n Number of columns of C and B
 * @param p Number of columns of A, number of rows of B
 */
void matmatBlocked(double *C, const double *A, const double *B,
                   int m, int n, int p)
{
        // Implement a blocked matmat here (about 8 lines of code)
        // Use block size 56
memset(C, 0, m * n * sizeof(double));
for (size_t bk = 0; bk < p; bk+=56)
	for (size_t bj = 0; bj < n; bj+=56)
		for (size_t bi = 0; bi < m; bi+=56)
			for (size_t k = 0; k < 56; k++)
        			for (size_t j = 0; j < 56; j++)
                			for (size_t i = 0; i < 56; i++)
                        			C[(bj+j)*m + (i+bi)] += A[(k+bk)*m + (bi+i)] * B[(bj+j)*p + (bk+k)];

}

int main(int argc, char* argv[])
{
        const size_t m = 3360, n = 3360, p = 3360;
        cout << "Matrix dimensions: (" << m << " x " << p << ") x ("
             << p << " x " << n << ")" << endl;

        double *C = new double[m*n];
        double *A = new double[m*p];
        double *B = new double[p*n];

        // fill in the elements of A
        for (size_t i = 0; i < m*p; i++)
                A[i] = i;

        // fill in the elements of B
        for (size_t i = 0; i < p*n; i++)
                B[i] = i;

        double flops = 2 * m * n * p;

        startChrono();
        matmatBlocked(C, A, B, m, n, p);
        double elapsed = stopChrono();
        cout << "Time for a blocked matrix-matrix multiplication: "
             << elapsed << "s (" << flops / elapsed << " FLOPS/s)" << endl;

	cout << "Correctness check: C(0, 0) = " << C[0] << endl;

        delete [] C;
        delete [] A;
        delete [] B;

        exit(EXIT_SUCCESS);
}
