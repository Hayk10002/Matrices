#pragma once



template <size_t my_R, size_t my_C, class my_T = double>
class Matrix
{
private:
    my_T my_values[my_R][my_C];

	template <class = enable_if_t<my_R == my_C>>
	static Matrix<my_R, my_C, my_T> get_Identity();

public:

    Matrix(my_T fill_value = my_T(0))
    {
        for(size_t i = 0; i < my_R; i++) for(size_t j = 0; j < my_C; j++) my_values[i][j] = fill_value;
    }

    Matrix(std::initializer_list<std::initializer_list<my_T>> l, my_T def_value = my_T(0)) : Matrix(def_value)
    {
        for(size_t i = 0; i < my_R; i++) 
        {
            if(i >= l.size()) break;
            for(size_t j = 0; j < my_C; j++) 
            {
                if(j >= (*(l.begin() + i)).size()) break; 
                my_values[i][j] = *((*(l.begin() + i)).begin() + j);
            }
        }
    }

    template<size_t o_R, size_t o_C, class o_T>
    Matrix(Matrix<o_R, o_C, o_T> o_m, my_T def_value = my_T(0)) : Matrix(def_value)
    {
        for(size_t i = 0; i < my_R; i++) 
        {
            if(i >= o_R) break;
            for(size_t j = 0; j < my_C; j++) 
            {
                if(j >= o_C) break; 
                my_values[i][j] = o_m(i, j);
            }
        }
    }

    my_T& operator()(size_t r, size_t c)
    {
        r = (r >= my_R) ? my_R - 1 : r;
        c = (c >= my_C) ? my_C - 1 : c;
        return my_values[r][c];
    }

    void print()
    {
        for(size_t i = 0; i < my_R; i++) 
        {
            for(size_t j = 0; j < my_C; j++) std::cout << my_values[i][j] << ' ';
            std::cout << std::endl;
        }
    }

	static Matrix<my_R, my_C, my_T> Zero;
	template <class = enable_if_t<my_R == my_C>>
	static Matrix<my_R, my_C, my_T> Identity;
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
