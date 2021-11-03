#pragma once
#include <string>
#include <iostream>
#include <fstream>


//if my_R or my_C is 0 this class will be completely empty
template <size_t my_R, size_t my_C, class my_T = double>
class Matrix_T
{

	template <size_t, size_t, class> 
	friend class Matrix_T;
private:

    my_T **my_values = nullptr;



	//Returns an Identity matrix of size my_R
	//If my_R != my_C, it will be compile error
	template <class = enable_if_t<my_R == my_C>>
	static Matrix_T<my_R, my_C, my_T> get_Identity();


	//Allocate memory in heap for values
	void allocate()
	{
		my_values = new my_T*[my_R];
		for (int i = 0; i < my_R; i++) my_values[i] = new my_T[my_C];
	}

	//Dealocates memory from heap
	void deallocate()
	{
		for (int i = 0; i < my_R; i++) delete[] my_values[i];
		delete[] my_values;
		my_values = nullptr;
	}

	//Initializes this matrix from initializer list
	template<class o_T>
	void init(std::initializer_list<std::initializer_list<o_T>> l, my_T def_value = my_T(0))
	{
		allocate();
		fill(def_value);
		for (size_t i = 0; i < my_R; i++)
		{
			if (i >= l.size()) break;
			for (size_t j = 0; j < my_C; j++)
			{
				if (j >= (*(l.begin() + i)).size()) break;
				my_values[i][j] = *((*(l.begin() + i)).begin() + j);
			}
		}
	}

	//Initializes this matrix from another matrix
	template<size_t o_R, size_t o_C, class o_T>
	void init(const Matrix_T<o_R, o_C, o_T>& o_m, my_T def_value = my_T(0))
	{
		if (!o_m.is_valid()) return;
		allocate();
		fill(def_value);
		for (size_t i = 0; i < my_R; i++)
		{
			if (i >= o_R) break;
			for (size_t j = 0; j < my_C; j++)
			{
				if (j >= o_C) break;
				my_values[i][j] = o_m(i, j);
			}
		}
	}


public:

	//Fills this matrix with some value
	void fill(my_T fill_value)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] = fill_value;
	}


	//Fills this matrix with some value
    Matrix_T(my_T fill_value = my_T(0))
    {
		allocate();
		fill(fill_value);
    }

	//Initializes this matrix from initializer list
	template<class o_T>
    Matrix_T(std::initializer_list<std::initializer_list<o_T>> l, my_T def_value = my_T(0))
    {
		init(l, def_value);
    }

	//Initializes this matrix from another matrix
    template<size_t o_R, size_t o_C, class o_T>
    Matrix_T(const Matrix_T<o_R, o_C, o_T>& o_m, my_T def_value = my_T(0))
    {
		init(o_m, def_value);
    }

	//Copy constructor
	Matrix_T(const Matrix_T<my_R, my_C, my_T>& o_m)
	{
		init(o_m);
	}

	//Dealocates memory from heap
	~Matrix_T()
	{
		if(is_valid()) deallocate();
	}



	//Adds some value to all elements of this matrix
	template<class o_T = my_T>
	void add(o_T value)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] += value;
	}
	
	//Adds other matrix to this matrix
	template<class o_T = my_T>
	void add(const Matrix_T<my_R, my_C, o_T>& matrix)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] += matrix(i, j);
	}

	//Substracts some value from all elements of this matrix
	template<class o_T = my_T>
	void substract(o_T value)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] -= value;
	}

	//Substracts other matrix from this matrix
	template<class o_T = my_T>
	void substract(const Matrix_T<my_R, my_C, o_T>& matrix)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] -= matrix(i, j);
	}

	//Multiplies all elements of this matrix by some value
	template<class o_T = my_T>
	void multiply(o_T value)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] *= value;
	}

	//Divides all elements of this matrix by some value
	template<class o_T = my_T>
	void divide(o_T value)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] /= value;
	}



	//Returns element of matrix in r-th row and c-th column
    my_T& operator()(size_t r, size_t c)
    {
        r = (r >= my_R) ? my_R - 1 : r;
        c = (c >= my_C) ? my_C - 1 : c;
        return my_values[r][c];
    }

	//Returns element of matrix in r-th row and c-th column
	my_T& operator()(size_t r, size_t c) const
	{
		r = (r >= my_R) ? my_R - 1 : r;
		c = (c >= my_C) ? my_C - 1 : c;
		return my_values[r][c];
	}

	//Unary operator- (Returns this matrix with changed signs of all elements)
	Matrix_T<my_R, my_C, my_T> operator-()
	{
		return Zero - (*this);
	}

	//Unary operator- (Returns this matrix with changed signs of all elements)
	Matrix_T<my_R, my_C, my_T> operator-() const
	{
		return Zero - (*this);
	}

	//Unary operator+ (Returns this matrix without doing something :) )
	Matrix_T<my_R, my_C, my_T> operator+()
	{
		return (*this);
	}

	//Unary operator+ (Returns this matrix without doing something :) )
	Matrix_T<my_R, my_C, my_T> operator+() const
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
	void operator+=(const Matrix_T<my_R, my_C, oT>& mat)
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
	void operator-=(const Matrix_T<my_R, my_C, oT>& mat)
	{
		substract(mat);
	}

	//Multiplies all elements of this matrix by some value
	template<class oT>
	void operator*=(oT val)
	{
		multiply(val);
	}

	//Divides all elements of this matrix by some value
	template<class oT>
	void operator/=(oT val)
	{
		divide(val);
	}

	//Type casting operator to matrix_holder
	operator matrix_holder<my_T>()
	{
		return matrix_holder<my_T>(*this);
	}

	//Type casting operator to matrix_holder
	operator matrix_holder<my_T>() const
	{
		return matrix_holder<my_T>(*this);
	}

	//Type casting operator to bool(true if matrix is valid)
	operator bool()
	{
		return is_valid();
	}

	//Type casting operator to bool(true if matrix is valid)
	operator bool() const
	{
		return is_valid();
	}

	//Returns true if matrix is valid
	bool is_valid()
	{
		return my_values != nullptr;
	}

	//Returns true if matrix is valid
	bool is_valid() const
	{
		return my_values != nullptr;
	}



	Matrix_T<my_R, my_C, my_T>& operator=(const Matrix_T<my_R, my_C, my_T>& o_m)
	{
		init(o_m);
		return (*this);
	}
	
	//Initializes this matrix from another matrix
	template<size_t o_R, size_t o_C, class o_T>
	Matrix_T<my_R, my_C, my_T>& operator=(const Matrix_T<o_R, o_C, o_T>& o_m)
	{
		init(o_m);
		return (*this);
	}

	//Initializes this matrix from initializer list
	template<class o_T>
	Matrix_T<my_R, my_C, my_T>& operator=(std::initializer_list<std::initializer_list<o_T>> l)
	{
		init(l);
	}



	//Prints this matrix in console
    void print(std::string horizontal_sep = " ", std::string vertical_sep = "\n")
    {
        for(size_t i = 0; i < my_R; i++) 
        {
            for(size_t j = 0; j < my_C; j++) std::cout << my_values[i][j] << ((j == my_C - 1) ? "" : horizontal_sep);
            std::cout << ((i == my_R - 1) ? "" : vertical_sep);
        }
    }

	//Prints this matrix in console
	void print(std::string horizontal_sep = " ", std::string vertical_sep = "\n") const
	{
		for (size_t i = 0; i < my_R; i++)
		{
			for (size_t j = 0; j < my_C; j++) std::cout << my_values[i][j] << ((j == my_C - 1) ? "" : horizontal_sep);
			std::cout << ((i == my_R - 1) ? "" : vertical_sep);
		}
	}



	//Zero matrix of size my_R x my_C
	static Matrix_T<my_R, my_C, my_T> Zero;

	//Returns Transpose matrix of this matrix
	Matrix_T<my_C, my_R, my_T> get_Transpose()
	{
		Matrix_T<my_C, my_R, my_T> res;
		for (int j = 0; j < my_C; j++) for (int i = 0; i < my_R; i++) res(j, i) = my_values[i][j];
		return res;
	}

	//Returns Transpose matrix of this matrix
	Matrix_T<my_C, my_R, my_T> get_Transpose() const
	{
		Matrix_T<my_C, my_R, my_T> res;
		for (int j = 0; j < my_C; j++) for (int i = 0; i < my_R; i++) res(j, i) = my_values[i][j];
		return res;
	}

	//An Identity matrix of size my_R
	//If my_R != my_C, it will be compile error
	template <class = enable_if_t<my_R == my_C>>
	static Matrix_T<my_R, my_C, my_T> Identity;

	//Returns submatrix of this matrix without (delete_r_ind)-th row and (delete_c_ind)-th column
	//If my_R or my_C is equal to 1, it will be compile error
	template <class = enable_if_t<my_R != 1 && my_C != 1>>
	Matrix_T<my_R - 1, my_C - 1, my_T> get_Submatrix(size_t delete_r_ind, size_t delete_c_ind)
	{
		delete_r_ind = (delete_r_ind > my_R - 1) ? (my_R - 1) : delete_r_ind;
		delete_c_ind = (delete_c_ind > my_C - 1) ? (my_C - 1) : delete_c_ind;
		Matrix_T<my_R - 1, my_C - 1, my_T> res;
		for (int i = 0, ri = 0; i < my_R; i++)
		{
			if (i == delete_r_ind) continue;
			for (int j = 0, rj = 0; j < my_C; j++)
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
	//If my_R or my_C is equal to 1, it will be compile error
	template <class = enable_if_t<my_R != 1 && my_C != 1>>
	Matrix_T<my_R - 1, my_C - 1, my_T> get_Submatrix(size_t delete_r_ind, size_t delete_c_ind) const
	{
		delete_r_ind = (delete_r_ind > my_R - 1) ? (my_R - 1) : delete_r_ind;
		delete_c_ind = (delete_c_ind > my_C - 1) ? (my_C - 1) : delete_c_ind;
		Matrix_T<my_R - 1, my_C - 1, my_T> res;
		for (int i = 0, ri = 0; i < my_R; i++)
		{
			if (i == delete_r_ind) continue;
			for (int j = 0, rj = 0; j < my_C; j++)
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
	template <class = enable_if_t<my_R != 1>>
	Matrix_T<my_R - 1, my_C , my_T> delete_Row(size_t delete_r_ind)
	{
		delete_r_ind = (delete_r_ind > my_R - 1) ? (my_R - 1) : delete_r_ind;
		Matrix_T<my_R - 1, my_C, my_T> res;
		for (int i = 0, ri = 0; i < my_R; i++)
		{
			if (i == delete_r_ind) continue;
			for (int j = 0; j < my_C; j++) res(ri, j) = my_values[i][j];
			ri++;
		}
		return res;
	}

	//Returns submatrix of this matrix without (delete_r_ind)-th row
	//If my_R is equal to 1, it will be compile error
	template <class = enable_if_t<my_R != 1>>
	Matrix_T<my_R - 1, my_C, my_T> delete_Row(size_t delete_r_ind) const
	{
		delete_r_ind = (delete_r_ind > my_R - 1) ? (my_R - 1) : delete_r_ind;
		Matrix_T<my_R - 1, my_C, my_T> res;
		for (int i = 0, ri = 0; i < my_R; i++)
		{
			if (i == delete_r_ind) continue;
			for (int j = 0; j < my_C; j++) res(ri, j) = my_values[i][j];
			ri++;
		}
		return res;
	}

	//Returns submatrix of this matrix without (delete_c_ind)-th column
	//If my_C is equal to 1, it will be compile error
	template <class = enable_if_t<my_C != 1>>
	Matrix_T<my_R, my_C - 1, my_T> delete_Column(size_t delete_c_ind)
	{
		delete_c_ind = (delete_c_ind > my_C - 1) ? (my_C - 1) : delete_c_ind;
		Matrix_T<my_R, my_C - 1, my_T> res;
		for (int i = 0; i < my_R; i++) for (int j = 0, rj = 0; j < my_C; j++)
		{
			if (j == delete_c_ind) continue;
			res(i, rj) = my_values[i][j];
			rj++;
		}
		return res;
	}

	//Returns submatrix of this matrix without (delete_c_ind)-th column
	//If my_C is equal to 1, it will be compile error
	template <class = enable_if_t<my_C != 1>>
	Matrix_T<my_R, my_C - 1, my_T> delete_Column(size_t delete_c_ind) const
	{
		delete_c_ind = (delete_c_ind > my_C - 1) ? (my_C - 1) : delete_c_ind;
		Matrix_T<my_R, my_C - 1, my_T> res;
		for (int i = 0; i < my_R; i++) for (int j = 0, rj = 0; j < my_C; j++)
		{
			if (j == delete_c_ind) continue;
			res(i, rj) = my_values[i][j];
			rj++;
		}
		return res;
	}

	//Returns determinant of this matrix
	//If my_R != my_C, it will be compile error
	template <class = enable_if_t<my_R == my_C>>
	my_T get_Determinant()
	{
		if constexpr (my_R == 1) return my_values[0][0];
		else
		{
			my_T res(0);
			bool sign = 1;
			for (int i = 0; i < my_R; i++)
			{
				my_T det = get_Submatrix(0, i).get_Determinant();
				res += (sign - !sign) * det * my_values[0][i];
				sign = !sign;
			}
			return res;
		}
	}

	//Returns determinant of this matrix
	//If my_R != my_C, it will be compile error
	template <class = enable_if_t<my_R == my_C>>
	my_T get_Determinant() const
	{
		if constexpr (my_R == 1) return my_values[0][0];
		else
		{
			my_T res(0);
			bool sign = 1;
			for (int i = 0; i < my_R; i++)
			{
				my_T det = get_Submatrix(0, i).get_Determinant();
				res += (sign - !sign) * det * my_values[0][i];
				sign = !sign;
			}
			return res;
		}
	}

	//Returns determinant of this matrix
	//If my_R != my_C, it will be compile error
	//If determinant of this matrixis equal to 0, function returns an invalid matrix of size my_R
	template <class = enable_if_t<my_R == my_C>>
	Matrix_T<my_R, my_C, my_T> get_Inverse()
	{
		my_T det = get_Determinant();
		Matrix_T<my_R, my_C, my_T> res;
		if(det == 0)
		{
			res.deallocate();
			return res;
		}
		for (int i = 0; i < my_R; i++) for (int j = 0; j < my_R; j++) res(i, j) = get_Submatrix(i, j).get_Determinant();
		for (int i = 0; i < my_R; i++) for (int j = 0; j < my_R; j++) if((i + j) % 2 == 1)res(i, j) = -res(i, j);
		res = res.get_Transpose();
		res /= det;
		return res;
	}

	//Returns determinant of this matrix
	//If my_R != my_C, it will be compile error
	//If determinant of this matrixis equal to 0, function returns an invalid matrix of size my_R
	template <class = enable_if_t<my_R == my_C>>
	Matrix_T<my_R, my_C, my_T> get_Inverse() const
	{
		my_T det = get_Determinant();
		Matrix_T<my_R, my_C, my_T> res;
		if (det == 0)
		{
			res.deallocate();
			return res;
		}
		for (int i = 0; i < my_R; i++) for (int j = 0; j < my_R; j++) res(i, j) = get_Submatrix(i, j).get_Determinant();
		for (int i = 0; i < my_R; i++) for (int j = 0; j < my_R; j++) if ((i + j) % 2 == 1)res(i, j) = -res(i, j);
		res = res.get_Transpose();
		res /= det;
		return res;
	}
};

template <size_t my_R, size_t my_C, class my_T>
Matrix_T<my_R, my_C, my_T> Matrix_T<my_R, my_C, my_T>::Zero = Matrix_T<my_R, my_C, my_T>();

template <size_t my_R, size_t my_C, class my_T>
template <class>
Matrix_T<my_R, my_C, my_T> Matrix_T<my_R, my_C, my_T>::get_Identity()
{
	Matrix_T<my_R, my_C, my_T> Id = Matrix_T<my_R, my_C, my_T>::Zero;
	for (size_t i = 0; i < my_R; i++) Id(i, i) = my_T(1);
	return Id;
}

template <size_t my_R, size_t my_C, class my_T>
template <class>
Matrix_T<my_R, my_C, my_T> Matrix_T<my_R, my_C, my_T>::Identity = Matrix_T<my_R, my_C, my_T>::get_Identity();

#define IDENTITY(S, T) Matrix_T<S, S, T>::Identity<>



template<size_t R, size_t C, class T, class oT>
Matrix_T<R, C, T> operator+(Matrix_T<R, C, T> mat, oT val)
{
	mat.add(val);
	return mat;
}

template<size_t R, size_t C, class T, class oT>
Matrix_T<R, C, T> operator+(oT val, Matrix_T<R, C, T> mat)
{
	mat.add(val);
	return mat;
}

template<size_t R, size_t C, class T, class oT>
Matrix_T<R, C, T> operator+(Matrix_T<R, C, T> mat1, Matrix_T<R, C, oT> mat2)
{
	mat1.substract(mat2);
	return mat1;
}



template<size_t R, size_t C, class T, class oT>
Matrix_T<R, C, T> operator-(Matrix_T<R, C, T> mat, oT val)
{
	mat.substract(val);
	return mat;
}

template<size_t R, size_t C, class T, class oT>
Matrix_T<R, C, T> operator-(oT val, Matrix_T<R, C, T> mat)
{
	Matrix_T<R, C, T> res(-mat);
	res.add(val);
	return res;
}

template<size_t R, size_t C, class T, class oT>
Matrix_T<R, C, T> operator-(Matrix_T<R, C, T> mat1, Matrix_T<R, C, oT> mat2)
{
	mat1.substract(mat2);
	return mat1;
}



template<size_t R, size_t C, class T, class oT>
Matrix_T<R, C, T> operator*(Matrix_T<R, C, T> mat, oT val)
{
	mat.multiply(val);
	return mat;
}

template<size_t R, size_t C, class T, class oT>
Matrix_T<R, C, T> operator*(oT val, Matrix_T<R, C, T> mat)
{
	mat.multiply(val);
	return mat;
}

template<size_t R, size_t C, size_t oC, class T, class oT>
Matrix_T<R, oC, T> operator*(Matrix_T<R, C, T> mat1, Matrix_T<C, oC, oT> mat2)
{
	Matrix_T<R, oC, T> res;
	for (int i = 0; i < R; i++) for (int j = 0; j < oC; j++)
	{
		T sum(0);
		for (int k = 0; k < C; k++) sum += mat1(i, k) * mat2(k, j);
		res(i, j) = sum;
	} 
	return res;
}



template<size_t R, size_t C, class T, class oT>
Matrix_T<R, C, T> operator/(Matrix_T<R, C, T> mat, oT val)
{
	mat.divide(val);
	return mat;
}



template<size_t R, size_t C, class T, size_t oR, size_t oC, class oT>
bool operator==(Matrix_T<R, C, T> mat1, Matrix_T<oR, oC, oT> mat2)
{
	return 0;
}

template<size_t R, size_t C, class T, class oT>
bool operator==(Matrix_T<R, C, T> mat1, Matrix_T<R, C, oT> mat2)
{
	for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) if (mat1(i, j) != mat2(i, j)) return 0;
	return 1;
}



template<size_t R, size_t C, class T, size_t oR, size_t oC, class oT>
bool operator!=(Matrix_T<R, C, T> mat1, Matrix_T<oR, oC, oT> mat2)
{
	return !(mat1 == mat2);
}


//Renames matrix with equal number of rows and columns to square_matrix
template<size_t S, class T = double>
using Square_Matrix = Matrix_T<S, S, T>;

//Renames matrix with one row to row_matrix
template<size_t S, class T = double>
using Row_Matrix = Matrix_T<1, S, T>;

//Renames matrix with one column to column_matrix
template<size_t S, class T = double>
using Column_Matrix = Matrix_T<S, 1, T>;


//Makes Matrix class with 0 rows completely empty
template<size_t S, class T>
class Matrix_T<0, S, T> {};

//Makes Matrix class with 0 columns completely empty
template<size_t S, class T>
class Matrix_T<S, 0, T> {};



//Operator for writing in console
template<size_t R, size_t C, class T>
std::ostream& operator<<(std::ostream& cout, Matrix_T<R, C, T>& mat)
{
	mat.print();
	return cout;
}

//Operator for reading from console
template<size_t R, size_t C, class T>
std::istream& operator>>(std::istream& cin, Matrix_T<R, C, T>& mat)
{
	for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) cin >> mat(i, j);
	return cin;
}

//Operator for writing in file
template<size_t R, size_t C, class T>
std::ofstream& operator<<(std::ofstream& fout, Matrix_T<R, C, T>& mat)
{
	fout << ' ' << R << ' ' << C << ' ';
	for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) fout << mat(i, j) << ' ';
	return fout;
}

//Operator for reading from file
template<size_t R, size_t C, class T>
std::ifstream& operator>>(std::ifstream& fin, Matrix_T<R, C, T>& mat)
{
	size_t oR, oC;
	fin >> oR >> oC;
	for (size_t i = 0; i < oR; i++) for (size_t j = 0; j < oC; j++)
	{
		T val;
		fin >> val;
		if (i >= R || j >= C) continue;
		mat(i, j) = val;
	}
	return fin;
}

