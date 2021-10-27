#pragma once
#include <string>


template <size_t my_R, size_t my_C, class my_T = double>
class Matrix
{
private:

    my_T my_values[my_R][my_C];



	template <class = enable_if_t<my_R == my_C>>
	static Matrix<my_R, my_C, my_T> get_Identity();


	template<class o_T>
	void init(std::initializer_list<std::initializer_list<o_T>> l, my_T def_value = my_T(0))
	{
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

	template<size_t o_R, size_t o_C, class o_T>
	void init(Matrix<o_R, o_C, o_T> o_m, my_T def_value = my_T(0))
	{
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

	void fill(my_T fill_value)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] = fill_value;
	}



    Matrix(my_T fill_value = my_T(0))
    {
		fill(fill_value);
    }

	template<class o_T>
    Matrix(std::initializer_list<std::initializer_list<o_T>> l, my_T def_value = my_T(0))
    {
		init(l, def_value);
    }

    template<size_t o_R, size_t o_C, class o_T>
    Matrix(Matrix<o_R, o_C, o_T> o_m, my_T def_value = my_T(0))
    {
		init(o_m, def_value);
    }



	template<class o_T = my_T>
	void add(o_T value)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] += value;
	}
	
	template<class o_T = my_T>
	void add(Matrix<my_R, my_C, o_T> matrix)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] += matrix(i, j);
	}

	template<class o_T = my_T>
	void substract(o_T value)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] -= value;
	}

	template<class o_T = my_T>
	void substract(Matrix<my_R, my_C, o_T> matrix)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] -= matrix(i, j);
	}

	template<class o_T = my_T>
	void multiply(o_T value)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] *= value;
	}

	template<class o_T = my_T>
	void divide(o_T value)
	{
		for (size_t i = 0; i < my_R; i++) for (size_t j = 0; j < my_C; j++) my_values[i][j] /= value;
	}



    my_T& operator()(size_t r, size_t c)
    {
        r = (r >= my_R) ? my_R - 1 : r;
        c = (c >= my_C) ? my_C - 1 : c;
        return my_values[r][c];
    }

	Matrix<my_R, my_C, my_T> operator-()
	{
		return Zero - (*this);
	}

	Matrix<my_R, my_C, my_T> operator+()
	{
		return (*this);
	}

	template<class oT>
	void operator+=(oT val)
	{
		add(val);
	}

	template<class oT>
	void operator+=(Matrix<my_R, my_C, oT> mat)
	{
		add(mat);
	}

	template<class oT>
	void operator-=(oT val)
	{
		substract(val);
	}

	template<class oT>
	void operator-=(Matrix<my_R, my_C, oT> mat)
	{
		substract(mat);
	}

	template<class oT>
	void operator*=(oT val)
	{
		multiply(val);
	}

	template<class oT>
	void operator/=(oT val)
	{
		divide(val);
	}

	template<size_t o_R, size_t o_C, class o_T>
	Matrix<my_R, my_C, my_T>& operator=(Matrix<o_R, o_C, o_T> o_m)
	{
		init(o_m);
		return (*this);
	}

	template<class o_T>
	Matrix<my_R, my_C, my_T>& operator=(std::initializer_list<std::initializer_list<o_T>> l)
	{
		init(l);
	}



    void print(std::string horizontal_sep = " ", std::string vertical_sep = "\n")
    {
        for(size_t i = 0; i < my_R; i++) 
        {
            for(size_t j = 0; j < my_C; j++) std::cout << my_values[i][j] << ((j == my_C - 1) ? "" : horizontal_sep);
            std::cout << ((i == my_R - 1) ? "" : vertical_sep);
        }
    }



	static Matrix<my_R, my_C, my_T> Zero;

	Matrix<my_C, my_R, my_T> get_Transpose()
	{
		Matrix<my_C, my_R, my_T> res;
		for (int j = 0; j < my_C; j++) for (int i = 0; i < my_R; i++) res(j, i) = my_values[i][j];
		return res;
	}

	template <class = enable_if_t<my_R == my_C>>
	static Matrix<my_R, my_C, my_T> Identity;

	template <class = enable_if_t<my_R != 1 && my_C != 1>>
	Matrix<my_R - 1, my_C - 1, my_T> get_Submatrix(size_t delete_r_ind, size_t delete_c_ind)
	{
		delete_r_ind = (delete_r_ind > my_R - 1) ? (my_R - 1) : delete_r_ind;
		delete_c_ind = (delete_c_ind > my_C - 1) ? (my_C - 1) : delete_c_ind;
		Matrix<my_R - 1, my_C - 1, my_T> res;
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

	template <class = enable_if_t<my_R == my_C>>
	Matrix<my_R, my_C, my_T> get_Inverse()
	{
		my_T det = get_Determinant();
		if (det == 0) return Identity<>;
		Matrix<my_R, my_C, my_T> res;
		for (int i = 0; i < my_R; i++) for (int j = 0; j < my_R; j++) res(i, j) = get_Submatrix(i, j).get_Determinant();
		for (int i = 0; i < my_R; i++) for (int j = 0; j < my_R; j++) if((i + j) % 2 == 1)res(i, j) = -res(i, j);
		res = res.get_Transpose();
		res /= det;
		return res;
	}
};

template <size_t my_R, size_t my_C, class my_T>
Matrix<my_R, my_C, my_T> Matrix<my_R, my_C, my_T>::Zero = Matrix<my_R, my_C, my_T>();


template <size_t my_R, size_t my_C, class my_T>
template <class>
Matrix<my_R, my_C, my_T> Matrix<my_R, my_C, my_T>::get_Identity()
{
	Matrix<my_R, my_C, my_T> Id = Matrix<my_R, my_C, my_T>::Zero;
	for (size_t i = 0; i < my_R; i++) Id(i, i) = my_T(1);
	return Id;
}


template <size_t my_R, size_t my_C, class my_T>
template <class>
Matrix<my_R, my_C, my_T> Matrix<my_R, my_C, my_T>::Identity = Matrix<my_R, my_C, my_T>::get_Identity();

#define IDENTITY(S, T) Matrix<(S), (S), T>::Identity<>



template<size_t R, size_t C, class T, class oT>
Matrix<R, C, T> operator+(Matrix<R, C, T> mat, oT val)
{
	mat.add(val);
	return mat;
}

template<size_t R, size_t C, class T, class oT>
Matrix<R, C, T> operator+(oT val, Matrix<R, C, T> mat)
{
	mat.add(val);
	return mat;
}

template<size_t R, size_t C, class T, class oT>
Matrix<R, C, T> operator+(Matrix<R, C, T> mat1, Matrix<R, C, oT> mat2)
{
	mat1.substract(mat2);
	return mat1;
}



template<size_t R, size_t C, class T, class oT>
Matrix<R, C, T> operator-(Matrix<R, C, T> mat, oT val)
{
	mat.substract(val);
	return mat;
}

template<size_t R, size_t C, class T, class oT>
Matrix<R, C, T> operator-(oT val, Matrix<R, C, T> mat)
{
	Matrix<R, C, T> res(-mat);
	res.add(val);
	return res;
}

template<size_t R, size_t C, class T, class oT>
Matrix<R, C, T> operator-(Matrix<R, C, T> mat1, Matrix<R, C, oT> mat2)
{
	mat1.substract(mat2);
	return mat1;
}



template<size_t R, size_t C, class T, class oT>
Matrix<R, C, T> operator*(Matrix<R, C, T> mat, oT val)
{
	mat.multiply(val);
	return mat;
}

template<size_t R, size_t C, class T, class oT>
Matrix<R, C, T> operator*(oT val, Matrix<R, C, T> mat)
{
	mat.multiply(val);
	return mat;
}

template<size_t R, size_t C, size_t oC, class T, class oT>
Matrix<R, oC, T> operator*(Matrix<R, C, T> mat1, Matrix<C, oC, oT> mat2)
{
	Matrix<R, oC, T> res;
	for (int i = 0; i < R; i++) for (int j = 0; j < oC; j++)
	{
		T sum(0);
		for (int k = 0; k < C; k++) sum += mat1(i, k) * mat2(k, j);
		res(i, j) = sum;
	} 
	return res;
}



template<size_t R, size_t C, class T, class oT>
Matrix<R, C, T> operator/(Matrix<R, C, T> mat, oT val)
{
	mat.divide(val);
	return mat;
}



template<size_t R, size_t C, class T, size_t oR, size_t oC, class oT>
bool operator==(Matrix<R, C, T> mat1, Matrix<oR, oC, oT> mat2)
{
	return 0;
}

template<size_t R, size_t C, class T, class oT>
bool operator==(Matrix<R, C, T> mat1, Matrix<R, C, oT> mat2)
{
	for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) if (mat1(i, j) != mat2(i, j)) return 0;
	return 1;
}



template<size_t R, size_t C, class T, size_t oR, size_t oC, class oT>
bool operator!=(Matrix<R, C, T> mat1, Matrix<oR, oC, oT> mat2)
{
	return !(mat1 == mat2);
}
