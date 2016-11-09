#ifndef MATRIX_H__
#define MATRIX_H__

#include <array>
#include <ostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <initializer_list>

template<typename T, int R, int C>
class Matrix
{
    std::array< std::array<T, C>, R > m_storage;
   
public:
    Matrix(const Matrix& m)
    {
        for(int r = 0; r < R; ++r)
            for(int c = 0; c < C; ++c)
                (*this)(r,c) = m(r,c);
    }
    
    Matrix(Matrix&& m)
        : m_storage( std::move(m.m_storage) )
    {
    }
    
    Matrix(std::initializer_list< std::initializer_list<T> > LL)
    {
        auto LL_i = LL.begin();

        for(int r = 0; r < R; ++r)
        {
            if(LL_i != LL.end())
            {
                auto L_i = LL_i->begin();

                for(int c = 0; c < C; ++c)
                {
                    (*this)(r,c) = L_i != LL_i->end() ? *L_i++ : T();
                }
                
                ++LL_i;
            }
            else
            {
                for(int c = 0; c < C; ++c)
                {
                    (*this)(r,c) = T();
                }
            }
        }
    }
    
    T& operator()(std::size_t i, std::size_t j)
    {
        return m_storage[i][j];
    }

    const T& operator()(std::size_t i, std::size_t j) const
    {
        return m_storage[i][j];
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m)
    {
        size_t max_len = 0;
       
        for(std::size_t r = 0; r < R; ++r)
        {
            for(std::size_t c = 0; c < C; ++c)
            {
                std::ostringstream oss;
                oss << m(r, c);

                max_len = std::max( oss.str().length(), max_len );
            }
        }

        for(std::size_t r = 0; r < R; ++r)
        {
            os << "[ ";
            std::string sep;
            for(std::size_t c = 0; c < C; ++c)
            {
                os << sep << std::setw(max_len) << m(r, c) << std::setw(0);
                if(sep.empty()) sep = ", ";
            }
            os << " ]\n";
        }
       
        return os;
    }

    Matrix& operator *= (const T& val)
    {
        for(int r = 0; r < R; ++r)
            for(int c = 0; c < C; ++c)
                (*this)(r,c) *= val;
            
        return *this;
    }

    friend Matrix operator*(const T& val, const Matrix& rhs)
    {
        Matrix result = rhs;
        result *= val;
        return result;
    }

    friend Matrix operator*(const Matrix& lhs, const T& val)
    {
        Matrix result = lhs;
        result *= val;
        return result;
    }

    template<typename U>
    Matrix& operator+=( const Matrix<U, R, C>& rhs )
    {
        for(int r = 0; r < R; ++r)
            for(int c = 0; c < C; ++c)
                (*this)(r,c) += rhs(r,c);
            
        return *this;
    }

    friend Matrix operator+(const Matrix& lhs, const Matrix& rhs)
    {
        Matrix result = lhs;
        result += rhs;
        return result;
    }

    template<int X>
    friend Matrix<T, R, X> operator*(const Matrix& lhs, const Matrix<T, C, X>& rhs)
    {
        Matrix<T, R, X> result = {};
      
        for(int r = 0; r < R; ++r)
        {
            for(int x = 0; x < X; ++x)
            {
                for(int c = 0; c < C; ++c)
                {
                    result(r,x) += lhs(r,c) * rhs(c,x);
                }
            }
        }
        
        return result;
    }
};

#endif
