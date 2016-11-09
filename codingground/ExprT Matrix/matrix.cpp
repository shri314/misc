#ifndef MATRIX_H__
#define MATRIX_H__

#include <array>
#include <ostream>
#include <iomanip>
#include <utility>
#include <algorithm>
#include <functional>
#include <initializer_list>

template<class Impl, std::size_t R, std::size_t C>
struct MatrixExpression
{
    auto operator()(std::size_t i, std::size_t j) const
    {
        return to_impl() ( i, j );
    }
    
    Impl& to_impl() &
    {
        return *(static_cast<Impl*>(this));
    }

    /*    
    Impl&& to_impl() &&
    {
        return std::move( *(static_cast<Impl*>(this)) );
    }
    */

    const Impl& to_impl() const&
    {
        return *(static_cast<const Impl*>(this));
    }
    
    friend std::ostream& operator<<(std::ostream& os, const MatrixExpression& m)
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
};



template<typename T, std::size_t R, std::size_t C>
class Matrix : public MatrixExpression<Matrix<T, R, C>, R, C>
{
    std::array< std::array<T, C>, R > m_storage;
   
public:
    Matrix(const Matrix& m) = delete; /*
    {
        for(int r = 0; r < R; ++r)
            for(int c = 0; c < C; ++c)
                (*this)(r, c) = m(r, c);
    } */
    
    Matrix(Matrix&& m) = delete; /*
        : m_storage( std::move(m.m_storage) )
    {
    } */
    
    Matrix(std::initializer_list< std::initializer_list<T> > LL)
    {
        auto LL_i = LL.begin();

        for(std::size_t r = 0; r < R; ++r)
        {
            if(LL_i != LL.end())
            {
                auto L_i = LL_i->begin();

                for(std::size_t c = 0; c < C; ++c)
                {
                    (*this)(r,c) = L_i != LL_i->end() ? *L_i++ : T();
                }
                
                ++LL_i;
            }
            else
            {
                for(std::size_t c = 0; c < C; ++c)
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
};


template<class LHS, class RHS, std::size_t R, std::size_t X, std::size_t C>
struct MatrixMultiplicationExpression : MatrixExpression< MatrixMultiplicationExpression<LHS, RHS, R, X, C>, R, C >
{
private:
    LHS lhs;
    RHS rhs;

public:    
    MatrixMultiplicationExpression( LHS lhs, RHS rhs )
       : lhs{lhs}
       , rhs{rhs}
    {
    }

    double operator()(std::size_t i, std::size_t j) const
    {
        double r = 0;
        for(std::size_t x = 0; x < X; ++x)
           r += lhs(i,x) * rhs(x,j);

        return r;
    }
};


template<class LHS, class RHS, std::size_t R, std::size_t C>
struct MatrixScaledExpression : MatrixExpression< MatrixScaledExpression<LHS, RHS, R, C>, R, C >
{
private:
    LHS lhs;
    RHS rhs;

public:
    MatrixScaledExpression( LHS lhs, RHS rhs )
       : lhs{lhs}
       , rhs{rhs}
    {
    }
    
    double operator()(std::size_t i, std::size_t j) const
    {
        return lhs(i,j) * rhs;
    }
};

///////////////////////////////

template<typename LHS, typename RHS, std::size_t R, std::size_t X, std::size_t C>
auto operator*(const MatrixExpression<LHS, R, X>& m, const MatrixExpression<RHS, X, C>& n)
{
    return MatrixMultiplicationExpression<const LHS&, const RHS&, R, X, C>{m.to_impl(), n.to_impl()};
}

template<typename LHS, typename RHS, std::size_t R, std::size_t X, std::size_t C>
auto operator*(MatrixExpression<LHS, R, X>&& m, const MatrixExpression<RHS, X, C>& n)
{
    return MatrixMultiplicationExpression<LHS, const RHS&, R, X, C>{m.to_impl(), n.to_impl()};
}

template<typename LHS, typename RHS, std::size_t R, std::size_t X, std::size_t C>
auto operator*(const MatrixExpression<LHS, R, X>& m, MatrixExpression<RHS, X, C>&& n)
{
    return MatrixMultiplicationExpression<const LHS&, RHS, R, X, C>{m.to_impl(), n.to_impl()};
}

template<typename LHS, typename RHS, std::size_t R, std::size_t X, std::size_t C>
auto operator*(MatrixExpression<LHS, R, X>&& m, MatrixExpression<RHS, X, C>&& n)
{
    return MatrixMultiplicationExpression<LHS, RHS, R, X, C>{m.to_impl(), n.to_impl()};
}

///////////////////////////////

template<typename LHS, std::size_t R, std::size_t C>
auto operator*(const MatrixExpression<LHS, R, C>& m, double n)
{
    return MatrixScaledExpression<const LHS&, double, R, C>{m.to_impl(), n};
}

template<typename RHS, std::size_t R, std::size_t C>
auto operator*(double n, MatrixExpression<RHS, R, C>&& m)
{
    return MatrixScaledExpression<RHS, double, R, C>{m.to_impl(), n};
}

template<typename LHS, std::size_t R, std::size_t C>
auto operator*(MatrixExpression<LHS, R, C>&& m, double n)
{
    return MatrixScaledExpression<LHS, double, R, C>{m.to_impl(), n};
}

template<typename RHS, std::size_t R, std::size_t C>
auto operator*(double n, const MatrixExpression<RHS, R, C>& m)
{
    return MatrixScaledExpression<const RHS&, double, R, C>{m.to_impl(), n};
}

#endif
