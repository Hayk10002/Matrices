#pragma once

template <unsigned int my_R, unsigned int my_C, class my_T = double>
class Matrix
{
    my_T my_values[my_R][my_C];
public:

    Matrix(my_T fill_value = my_T(0))
    {
        for(unsigned int i = 0; i < my_R; i++) for(unsigned int j = 0; j < my_C; j++) my_values[i][j] = fill_value;
    }

    Matrix(std::initializer_list<std::initializer_list<my_T>> l, my_T def_value = my_T(0)) : Matrix(def_value)
    {
        for(unsigned int i = 0; i < my_R; i++) 
        {
            if(i >= l.size()) break;
            for(unsigned int j = 0; j < my_C; j++) 
            {
                if(j >= (*(l.begin() + i)).size()) break; 
                my_values[i][j] = *((*(l.begin() + i)).begin() + j);
            }
        }
    }

    template<unsigned int o_R, unsigned int o_C, class o_T>
    Matrix(Matrix<o_R, o_C, o_T> o_m, my_T def_value = my_T(0)) : Matrix(def_value)
    {
        for(unsigned int i = 0; i < my_R; i++) 
        {
            if(i >= o_R) break;
            for(unsigned int j = 0; j < my_C; j++) 
            {
                if(j >= o_C) break; 
                my_values[i][j] = o_m(i, j);
            }
        }
    }

    my_T& operator()(unsigned int r, unsigned int c)
    {
        r = (r >= my_R) ? my_R - 1 : r;
        c = (c >= my_C) ? my_C - 1 : c;
        return my_values[r][c];
    }

    void print()
    {
        for(unsigned int i = 0; i < my_R; i++) 
        {
            for(unsigned int j = 0; j < my_C; j++) std::cout << my_values[i][j] << ' ';
            std::cout << std::endl;
        }
    }
};