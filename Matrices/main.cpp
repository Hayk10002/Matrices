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
	Matrix<4, 2> c;

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
	PRINTM("Submatrix of A without first row is ", a.delete_Row(0));
	PRINTM("Submatrix of A without second column is ", a.delete_Column(1));
	PRINTO("Determinant of A is ", a.get_Determinant());
	PRINTO("Determinant of B is ", b.get_Determinant());
	cout << "\n\nIf you try to calculate determinant for non-square matrix it will be a compile error\n\n";
	PRINTM("Inverse matrix of A is ", a.get_Inverse());
	cout << "\n\nIf you try to calculate inverse matrix of non-square matrix it will be a compile error and if you try to calculate inverse matrix of matrix which has determinant equal to 0, function will return Identity matrix of size of your matrix \n\n";
	PRINTM("Inverse matrix of B can't be calculated => function returns Idenitity matrix ", b.get_Inverse());
	cout << "There are functions for Console I/O and File I/O\n\nEnter matrix 3x3 without commas: \n";
	cin >> a;
	cout << "Your Matrix: \n" << a << "\n\n";
	cout << "Saving your matrix in file output.txt\n";
	ofstream fout("output.txt");
	fout << a;
	fout.close();
	cout << "Reading matrix from file output.txt but with other dimensions (4x2) and printing it in console \n\n";
	ifstream fin("output.txt");
	fin >> c;
	fin.close();
	cout << c << "\n\n";
    
	system("pause");
    return 0;
}