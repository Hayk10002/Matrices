#include <iostream>
#include "Matrix.hpp"
using namespace std; 

#define PRINT(bt, text, at, eq, ae) cout << bt##text##at; (eq); cout << ##ae
#define PRINTM(text, eq) PRINT("\n\n", text, "\n\n", (eq).print("\t", "\n\n"), "\n\n")
#define PRINTO(text, eq) PRINT("\n\n", text, "", cout << (eq), "\n\n")

int main()
{
    Matrix<3, 3> a(
		{
			{1, 3, 4},
			{-1, -5, 6},
			{5, 2, 3}
		}
	);
	Matrix<3, 3> b(
		{
			{2, 0, 0},
			{1, 0, -4}, 
			{0, 0, 2}
		}
	);

	PRINTM("A = ", a);
	PRINTM("B = ", b);
	PRINTM("A + 5 = ", a + 5);
	PRINTM("A + B = ", a + b);
	PRINTM("A - 5 = ", a - 5);
	PRINTM("5 - A = ", 5 - a);
	PRINTM("A - B = ", a - b);
	PRINTM("A * 5 = ", a * 5);
	PRINTM("A * B = ", a * b);
	PRINTM("A / 5 = ", a / 5);
	PRINTO("Element in first row and second column of matrix A (or A(0, 1) ) is ", a(0, 1));
	PRINTM("Transpose matrix of A is ", a.get_Transpose());
	PRINTM("Submatrix of A without first row and second column is ", a.get_Submatrix(0, 1));
	PRINTO("Determinant of A is ", a.get_Determinant());
	PRINTO("Determinant of B is ", b.get_Determinant());
	cout << "\n\nIf you try to calculate determinant for non-square matrix it will be a compile error\n\n";
	PRINTM("Inverse matrix of A is ", a.get_Inverse());
	cout << "\n\nIf you try to calculate inverse matrix of non-square matrix it will be a compile error and if you try to calculate inverse matrix of matrix which has determinant equal to 0, function will return Identity matrix of size of your matrix \n\n";
	PRINTM("Inverse matrix of B can't be calculated => function returns Idenitiy matrix ", b.get_Inverse());


    
	system("pause");
    return 0;
}