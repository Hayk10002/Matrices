#include <iostream>
#include <fstream>
#include "Matrix.hpp"
using namespace std;

#define PRINT(bt, text, at, exp, ae) cout << bt##text##at; (exp); cout << ##ae
#define PRINTM(text, exp) PRINT("\n\n", text, "\n\n", (exp).print("\t", "\n\n"), "\n\n")
#define PRINTO(text, exp) PRINT("\n\n", text, "", cout << (exp), "\n\n")

int main()
{
	Matrix<double> a(3, 3, 
		{
			{1, 3, 4},
			{-1, -5, 6},
			{5, 2, 3}
		}
	);
	Matrix<double> b(3, 3,
		{
			{2, 0, 0},
			{1, 0, -4},
			{0, 0, 2}
		}
	);
	Matrix<double> c(4, 2);

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
	cout << "\n\nIf you try to calculate determinant for non-square matrix it will be an exception error\n\n";
	PRINTM("Inverse matrix of A is ", a.get_Inverse());
	cout << "\n\nIf you try to calculate inverse matrix of non-square matrix it will be an exception error and if you try to calculate inverse matrix of matrix which has determinant equal to 0, it will be an exception error \n\n";
	try
	{
		auto i = b.get_Inverse();
		PRINTM("Inverse matrix of B is ", i);
	}
	catch (bad_matrix& err)
	{
		cout << "Inverse matrix of B can't be calculated => catched an exception of type bad_matrix\n" << err.what();
	}
	a.apply_func([](double a) -> double {return a / 3.0; });
	PRINTM("There is a method called 'apply_func' that applies any function for all elements of the matrix\nAfter applying function that divides number by 3 on matrix A, all elements of A become divided by 3\nA = ", a);
	cout << "There are functions for Console I/O and File I/O\n\nEnter matrix 3x3 without commas: \n";
	cin >> a;
	cout << "Your Matrix: \n" << a << "\n\n";
	cout << "Saving your matrix in file output2.txt\n";
	ofstream fout("output2.txt");
	fout << a;
	fout.close();
	cout << "Reading matrix from file output2.txt and printing it in console \n\n";
	ifstream fin("output2.txt");
	fin >> c;
	fin.close();
	cout << c << "\n\n";

	system("pause");
	return 0;
}