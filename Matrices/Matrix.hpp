#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <deque>
#include <functional>

class bad_matrix : public std::exception 
{
protected:
	std::string messege = "Unexpected error";
public:
	bad_matrix(std::string messege) : exception(), messege(messege) {}
	bad_matrix() : exception() {}
	const char * what()
	{
		return messege.c_str();
	}
};

class bad_matrix_size : public bad_matrix 
{	
public:
	bad_matrix_size(std::string messege) : bad_matrix(messege) {}
	bad_matrix_size() : bad_matrix() {}
};


template <class my_T = double>
class Matrix
{
	template <class>
	friend class Matrix;

	size_t my_r = 0, my_c = 0;
	std::deque<std::deque<my_T>> my_values;

	//Initializes this matrix from initializer list
	template <class o_T>
	void init(size_t rows, size_t columns, const std::initializer_list<std::initializer_list<o_T>> &l, my_T def_value = my_T(0))
	{
		if (rows == 0 || columns == 0) throw bad_matrix_size("Number of rows or columns of matrix cannot be zero");
		resize(rows, columns);
		fill(def_value);
		for (size_t i = 0; i < my_r; i++)
		{
			if (i >= l.size()) break;
			for (size_t j = 0; j < my_c; j++)
			{
				if (j >= (*(l.begin() + i)).size()) break;
				my_values[i][j] = *((*(l.begin() + i)).begin() + j);
			}
		}
	}

	//Initializes this matrix from another matrix
	template<class o_T>
	void init(size_t rows, size_t columns, const Matrix<o_T>& o_m, my_T def_value = my_T(0))
	{
		if (rows == 0 || columns == 0) throw bad_matrix_size("umber of rows or columns of matrix cannot be zero");
		resize(rows, columns);
		fill(def_value);
		for (size_t i = 0; i < my_r; i++)
		{
			if (i >= o_m.my_r) break;
			for (size_t j = 0; j < my_c; j++)
			{
				if (j >= o_m.my_c) break;
				my_values[i][j] = o_m(i, j);
			}
		}
	}

public:

	//Resizes this matrix
	void resize(size_t rows, size_t columns, my_T def_value = my_T(0))
	{
		my_values.resize(rows, {});
		for (auto &row : my_values) row.resize(columns, def_value);
		my_r = rows;
		my_c = columns;
	}

	//Returns number of rows of this matrix
	size_t get_rows_count() 
	{ 
		return my_r; 
	}

	//Returns number of columns of this matrix
	size_t get_columns_count() 
	{ 
		return my_c; 
	}

	//Fills this matrix with some value
	void fill(my_T fill_value)
	{
		for (auto &row : my_values) for (auto &element : row) element = fill_value;
	}



	//Fills this matrix with some value
	Matrix(size_t rows, size_t columns, my_T fill_value = my_T(0))
	{
		resize(rows, columns);
		fill(fill_value);
	}

	//Initializes this matrix from initializer list
	template<class o_T>
	Matrix(size_t rows, size_t columns, const std::initializer_list<std::initializer_list<o_T>> &l, my_T def_value = my_T(0))
	{
		init(rows, columns, l, def_value);
	}

	//Initializes this matrix from another matrix
	template<class o_T>
	Matrix(const Matrix<o_T>& o_m, my_T def_value = my_T(0))
	{
		init(o_m.my_r, o_m.my_c, o_m, def_value);
	}

	//Initializes this matrix from another matrix
	template<class o_T>
	Matrix(size_t rows, size_t columns, const Matrix<o_T>& o_m, my_T def_value = my_T(0))
	{
		init(rows, columns, o_m, def_value);
	}

	//Copy constructor
	Matrix(const Matrix<my_T>& o_m)
	{
		init(o_m.my_r, o_m.my_c, o_m);
	}



	//Adds some value to all elements of this matrix
	template<class o_T = my_T>
	void add(o_T value)
	{
		for (auto &row : my_values) for (auto &element : row) element += value;
	}

	//Adds other matrix to this matrix
	template<class o_T = my_T>
	void add(Matrix<o_T> matrix)
	{
		matrix.resize(my_r, my_c);
		for (size_t i = 0; i < my_r; i++) for (size_t j = 0; j < my_c; j++) my_values[i][j] += matrix(i, j);
	}

	//Substracts some value from all elements of this matrix
	template<class o_T = my_T>
	void substract(o_T value)
	{
		for (auto &row : my_values) for (auto &element : row) element -= value;
	}

	//Substracts other matrix from this matrix
	template<class o_T = my_T>
	void substract(Matrix<o_T> matrix)
	{
		matrix.resize(my_r, my_c);
		for (size_t i = 0; i < my_r; i++) for (size_t j = 0; j < my_c; j++) my_values[i][j] -= matrix(i, j);
	}

	//Multiplies all elements of this matrix by some value
	template<class o_T = my_T>
	void multiply(o_T value)
	{
		for (auto &row : my_values) for (auto &element : row) element *= value;
	}

	//Multiplies this matrix by other matrix
	template<class o_T = my_T>
	void multiply(Matrix<o_T> matrix)
	{
		(*this) = (*this) * matrix;
	}

	//Divides all elements of this matrix by some value
	template<class o_T = my_T>
	void divide(o_T value)
	{
		for (auto &row : my_values) for (auto &element : row) element /= value;
	}



	//Returns element of matrix in r-th row and c-th column
	my_T& operator()(size_t r, size_t c)
	{
		r = (r >= my_r) ? my_r - 1 : r;
		c = (c >= my_c) ? my_c - 1 : c;
		return my_values[r][c];
	}

	//Returns element of matrix in r-th row and c-th column
	const my_T& operator()(size_t r, size_t c) const
	{
		r = (r >= my_r) ? my_r - 1 : r;
		c = (c >= my_c) ? my_c - 1 : c;
		return my_values[r][c];
	}



	//Unary operator- (Returns this matrix with changed signs of all elements)
	Matrix<my_T> operator-()
	{
		return get_Zero(my_r, my_c) - (*this);
	}

	//Unary operator- (Returns this matrix with changed signs of all elements)
	Matrix<my_T> operator-() const
	{
		return get_Zero(my_r, my_c) - (*this);
	}

	//Unary operator+ (Returns this matrix without doing something :) )
	Matrix<my_T> operator+()
	{
		return (*this);
	}

	//Unary operator+ (Returns this matrix without doing something :) )
	Matrix<my_T> operator+() const
	{
		return (*this);
	}



	//Adds some value to all elements of this matrix
	template<class oT>
	void operator+=(oT val)
	{
		add(val);
	}

	//Adds other matrix to this matrix
	template<class oT>
	void operator+=(const Matrix<oT>& mat)
	{
		add(mat);
	}

	//Substracts some value from all elements of this matrix
	template<class oT>
	void operator-=(oT val)
	{
		substract(val);
	}

	//Substracts other matrix from this matrix
	template<class oT>
	void operator-=(const Matrix<oT>& mat)
	{
		substract(mat);
	}

	//Multiplies all elements of this matrix by some value
	template<class oT>
	void operator*=(oT val)
	{
		multiply(val);
	}

	//Multiplies all elements of this matrix by some value
	template<class oT>
	void operator*=(const Matrix<oT>& mat)
	{
		multiply(mat);
	}

	//Divides all elements of this matrix by some value
	template<class oT>
	void operator/=(oT val)
	{
		divide(val);
	}



	//Copy assignment operator
	Matrix<my_T>& operator=(const Matrix<my_T>& o_m)
	{
		init(o_m.my_r, o_m.my_c, o_m);
		return (*this);
	}

	//Initializes this matrix from another matrix
	template<class o_T>
	Matrix<my_T>& operator=(const Matrix<o_T>& o_m)
	{
		init(o_m.my_r, o_m.my_c, o_m);
		return (*this);
	}

	//Initializes this matrix from initializer list
	template<class o_T>
	Matrix<my_T>& operator=(const std::initializer_list<std::initializer_list<o_T>> &l)
	{
		init(my_r, my_c, l);
		return (*this);
	}



	//Applies function on every element of this matrix 
	void apply_func(std::function<my_T(my_T)> f)
	{
		for (auto &row : my_values) for (auto &element : row) element = f(element);
	}



	//Prints this matrix in console
	void print(std::string horizontal_sep = " ", std::string vertical_sep = "\n")
	{
		for (size_t i = 0; i < my_r; i++)
		{
			for (size_t j = 0; j < my_c; j++) std::cout << my_values[i][j] << ((j == my_c - 1) ? "" : horizontal_sep);
			std::cout << ((i == my_r - 1) ? "" : vertical_sep);
		}
	}

	//Prints this matrix in console
	void print(std::string horizontal_sep = " ", std::string vertical_sep = "\n") const
	{
		for (size_t i = 0; i < my_r; i++)
		{
			for (size_t j = 0; j < my_c; j++) std::cout << my_values[i][j] << ((j == my_c - 1) ? "" : horizontal_sep);
			std::cout << ((i == my_r - 1) ? "" : vertical_sep);
		}
	}



	//Returns an zero matrix of size rows x columns 
	static Matrix<my_T> get_Zero(size_t rows, size_t columns)
	{
		return Matrix<my_T>(rows, columns);
	}

	//Returns an Identity matrix of size s x s
	static Matrix<my_T> get_Identity(size_t s)
	{
		Matrix<my_T> Id(s, s);
		for (size_t i = 0; i < s; i++) Id(i, i) = my_T(1);
		return Id;
	}



	//Returns Transpose matrix of this matrix
	Matrix<my_T> get_Transpose()
	{
		Matrix<my_T> res(my_c, my_r);
		for (size_t j = 0; j < my_c; j++) for (size_t i = 0; i < my_r; i++) res(j, i) = my_values[i][j];
		return res;
	}

	//Returns Transpose matrix of this matrix
	Matrix<my_T> get_Transpose() const
	{
		Matrix<my_T> res(my_c, my_r);
		for (size_t j = 0; j < my_c; j++) for (size_t i = 0; i < my_r; i++) res(j, i) = my_values[i][j];
		return res;
	}



	//Returns submatrix of this matrix without (delete_r_ind)-th row and (delete_c_ind)-th column
	//If number of rows or columns of this matrix is equal to 1, it will be an exception error
	Matrix<my_T> get_Submatrix(size_t delete_r_ind, size_t delete_c_ind)
	{
		delete_r_ind = (delete_r_ind > my_r - 1) ? (my_r - 1) : delete_r_ind;
		delete_c_ind = (delete_c_ind > my_c - 1) ? (my_c - 1) : delete_c_ind;
		Matrix<my_T> res(my_r - 1, my_c - 1);
		for (size_t i = 0, ri = 0; i < my_r; i++)
		{
			if (i == delete_r_ind) continue;
			for (size_t j = 0, rj = 0; j < my_c; j++)
			{
				if (j == delete_c_ind) continue;
				res(ri, rj) = my_values[i][j];
				rj++;
			}
			ri++;
		}
		return res;
	}

	//Returns submatrix of this matrix without (delete_r_ind)-th row and (delete_c_ind)-th column
	//If number of rows or columns of this matrix is equal to 1, it will be an exception error
	Matrix<my_T> get_Submatrix(size_t delete_r_ind, size_t delete_c_ind) const
	{
		delete_r_ind = (delete_r_ind > my_r - 1) ? (my_r - 1) : delete_r_ind;
		delete_c_ind = (delete_c_ind > my_c - 1) ? (my_c - 1) : delete_c_ind;
		Matrix<my_T> res(my_r - 1, my_c - 1);
		for (size_t i = 0, ri = 0; i < my_r; i++)
		{
			if (i == delete_r_ind) continue;
			for (size_t j = 0, rj = 0; j < my_c; j++)
			{
				if (j == delete_c_ind) continue;
				res(ri, rj) = my_values[i][j];
				rj++;
			}
			ri++;
		}
		return res;
	}

	//Returns submatrix of this matrix without (delete_r_ind)-th row
	//If my_R is equal to 1, it will be compile error
	Matrix<my_T> delete_Row(size_t delete_r_ind)
	{
		delete_r_ind = (delete_r_ind > my_r - 1) ? (my_r - 1) : delete_r_ind;
		Matrix<my_T> res(my_r - 1, my_c);
		for (size_t i = 0, ri = 0; i < my_r; i++)
		{
			if (i == delete_r_ind) continue;
			for (size_t j = 0; j < my_c; j++) res(ri, j) = my_values[i][j];
			ri++;
		}
		return res;
	}

	//Returns submatrix of this matrix without (delete_r_ind)-th row
	//If number of rows of this matrix is equal to 1, it will be an exception error
	Matrix<my_T> delete_Row(size_t delete_r_ind) const
	{
		delete_r_ind = (delete_r_ind > my_r - 1) ? (my_r - 1) : delete_r_ind;
		Matrix<my_T> res(my_r - 1, my_c);
		for (size_t i = 0, ri = 0; i < my_r; i++)
		{
			if (i == delete_r_ind) continue;
			for (size_t j = 0; j < my_c; j++) res(ri, j) = my_values[i][j];
			ri++;
		}
		return res;
	}

	//Returns submatrix of this matrix without (delete_c_ind)-th column
	//If number of columns of this matrix is equal to 1, it will be an exception error
	Matrix<my_T> delete_Column(size_t delete_c_ind)
	{
		delete_c_ind = (delete_c_ind > my_c - 1) ? (my_c - 1) : delete_c_ind;
		Matrix<my_T> res(my_r, my_c - 1);
		for (size_t i = 0; i < my_r; i++) for (size_t j = 0, rj = 0; j < my_c; j++)
		{
			if (j == delete_c_ind) continue;
			res(i, rj) = my_values[i][j];
			rj++;
		}
		return res;
	}

	//Returns submatrix of this matrix without (delete_c_ind)-th column
	//If number of columns of this matrix is equal to 1, it will be an exception error
	Matrix<my_T> delete_Column(size_t delete_c_ind) const
	{
		delete_c_ind = (delete_c_ind > my_c - 1) ? (my_c - 1) : delete_c_ind;
		Matrix<my_T> res(my_r, my_c - 1);
		for (size_t i = 0; i < my_r; i++) for (size_t j = 0, rj = 0; j < my_c; j++)
		{
			if (j == delete_c_ind) continue;
			res(i, rj) = my_values[i][j];
			rj++;
		}
		return res;
	}



	//Returns determinant of this matrix
	//If this matrix is not a square matrix, it will be an exception error
	my_T get_Determinant()
	{
		if (my_r != my_c) throw bad_matrix_size("This matrix is not a square matrix. Cannot compute determinant.");
		if (my_r == 1) return my_values[0][0];
		my_T res(0);
		bool sign = 1;
		for (size_t i = 0; i < my_r; i++)
		{
			my_T det = get_Submatrix(0, i).get_Determinant();
			res += (sign - !sign) * det * my_values[0][i];
			sign = !sign;
		}
		return res;
	}

	//Returns determinant of this matrix
	//If this matrix is not a square matrix, it will be an exception error
	my_T get_Determinant() const
	{
		if (my_r != my_c) throw bad_matrix_size("This matrix is not a square matrix. Cannot compute determinant.");
		if (my_r == 1) return my_values[0][0];
		my_T res(0);
		bool sign = 1;
		for (size_t i = 0; i < my_r; i++)
		{
			my_T det = get_Submatrix(0, i).get_Determinant();
			res += (sign - !sign) * det * my_values[0][i];
			sign = !sign;
		}
		return res;
	}



	//Returns determinant of this matrix
	//If this matrix is not a square matrix, it will be an exception error
	//If determinant of this matrixis equal to 0, it will be an exception error
	Matrix<my_T> get_Inverse()
	{
		my_T det = get_Determinant();
		if (det == 0) throw bad_matrix("Determinant of this matrix is 0. Cannot compute inverse matrix.");
		Matrix<my_T> res(my_r, my_c);
		for (size_t i = 0; i < my_r; i++) for (size_t j = 0; j < my_r; j++) res(i, j) = get_Submatrix(i, j).get_Determinant();
		for (size_t i = 0; i < my_r; i++) for (size_t j = 0; j < my_r; j++) if ((i + j) % 2 == 1) res(i, j) = -res(i, j);
		res = res.get_Transpose();
		res /= det;
		return res;
	}

	//Returns determinant of this matrix
	//If this matrix is not a square matrix, it will be an exception error
	//If determinant of this matrixis equal to 0, it will be an exception error
	Matrix<my_T> get_Inverse() const
	{
		my_T det = get_Determinant();
		if (det == 0) throw bad_matrix("Determinant of this matrix is 0. Cannot compute inverse matrix.");
		Matrix<my_T> res;
		for (size_t i = 0; i < my_r; i++) for (size_t j = 0; j < my_r; j++) res(i, j) = get_Submatrix(i, j).get_Determinant();
		for (size_t i = 0; i < my_r; i++) for (size_t j = 0; j < my_r; j++) if ((i + j) % 2 == 1) res(i, j) = -res(i, j);
		res = res.get_Transpose();
		res /= det;
		return res;
	}

};


template<class T, class oT>
Matrix<T> operator+(Matrix<T> mat, oT val)
{
	mat.add(val);
	return mat;
}

template<class T, class oT>
Matrix<T> operator+(oT val, Matrix<T> mat)
{
	mat.add(val);
	return mat;
}

template<class T, class oT>
Matrix<T> operator+(Matrix<T> mat1, Matrix<oT> mat2)
{
	mat1.add(mat2);
	return mat1;
}



template<class T, class oT>
Matrix<T> operator-(Matrix<T> mat, oT val)
{
	mat.substract(val);
	return mat;
}

template<class T, class oT>
Matrix<T> operator-(oT val, Matrix<T> mat)
{
	return -mat + val;
}

template<class T, class oT>
Matrix<T> operator-(Matrix<T> mat1, Matrix<oT> mat2)
{
	mat1.substract(mat2);
	return mat1;
}



template<class T, class oT>
Matrix<T> operator*(Matrix<T> mat, oT val)
{
	mat.multiply(val);
	return mat;
}

template<class T, class oT>
Matrix<T> operator*(oT val, Matrix<T> mat)
{
	mat.multiply(val);
	return mat;
}

template<class T, class oT>
Matrix<T> operator*(Matrix<T> mat1, Matrix<oT> mat2)
{
	size_t r1 = mat1.get_rows_count();
	size_t c1 = mat1.get_columns_count();
	size_t r2 = mat2.get_rows_count();
	size_t c2 = mat2.get_columns_count();
	if (c1 != r2) throw bad_matrix_size("Number of columns of first matrix not equal number of rows of second matrix. Cannot compute multiplication of these matrices");
	Matrix<T> res(r1, c2);
	for (size_t i = 0; i < r1; i++) for (size_t j = 0; j < c2; j++)
	{
		T sum(0);
		for (size_t k = 0; k < c1; k++) sum += mat1(i, k) * mat2(k, j);
		res(i, j) = sum;
	}
	return res;
}



template<class T, class oT>
Matrix<T> operator/(Matrix<T> mat, oT val)
{
	mat.divide(val);
	return mat;
}



template<class T, class oT>
bool operator==(Matrix<T> mat1, Matrix<oT> mat2)
{
	if (mat1.get_rows_count() != mat2.get_rows_count() || mat1.get_columns_count() != mat2.get_columns_count()) return 0;
	for (size_t i = 0; i < mat1.get_rows_count(); i++) for (size_t j = 0; j < mat1.get_columns_count(); j++) if (mat1(i, j) != mat2(i, j)) return 0;
	return 1;
}

template<class T, class oT>
bool operator!=(Matrix<T> mat1, Matrix<oT> mat2)
{
	return !(mat1 == mat2);
}



//Operator for writing in console
template<class T>
std::ostream& operator<<(std::ostream& cout, Matrix<T>& mat)
{
	mat.print();
	return cout;
}

//Operator for reading from console
template<class T>
std::istream& operator>>(std::istream& cin, Matrix<T>& mat)
{
	for (size_t i = 0; i < mat.get_rows_count(); i++) for (size_t j = 0; j < mat.get_columns_count(); j++) cin >> mat(i, j);
	return cin;
}

//Operator for writing in file
template<class T>
std::ofstream& operator<<(std::ofstream& fout, Matrix<T>& mat)
{
	fout << ' ' << mat.get_rows_count() << ' ' << mat.get_columns_count() << ' ';
	for (size_t i = 0; i < mat.get_rows_count(); i++) for (size_t j = 0; j < mat.get_columns_count(); j++) fout << mat(i, j) << ' ';
	return fout;
}

//Operator for reading from file
template<class T>
std::ifstream& operator>>(std::ifstream& fin, Matrix<T>& mat)
{
	size_t o_r, o_c;
	fin >> o_r >> o_c;
	mat.resize(o_r, o_c);
	for (size_t i = 0; i < o_r; i++) for (size_t j = 0; j < o_c; j++) fin >> mat(i, j);
	return fin;
}

