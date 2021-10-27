#include <iostream>
#include "Matrix.hpp"
using namespace std; 

int main()
{
    Matrix<4, 2, int> a(
        {
            {1, 2, 3},
            {4, 5},
            {6, 7, 8, 9}
        }
    );
    Matrix<3, 3, double> b(a);
    b.print();
	IDENTITY(3, double);
	system("pause");
    return 0;
}