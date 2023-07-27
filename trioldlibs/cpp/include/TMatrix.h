////////////////////////////////////
/// Created by TRiOLD -l-
/// Email: TRiOLD@email.ua
///
////////////////////////////////////
#ifndef TMATRIX_H
#define TMATRIX_H

////////////////////////////////////
#include <iostream>
#include <stdexcept>
#include <utility>

////////////////////////////////////
namespace TRiOLD
{
    template<typename T>
    class Matrix
    {
//      N
//  M | 00 01 02 .. |
//    | 10 11 12 .. |
//    | 20 21 22 .. |
//    | .. .. .. mn |

    public:
        virtual ~Matrix();

        Matrix();
        Matrix(Matrix<T>&& matrix);
        Matrix(const Matrix<T>& matrix);

        Matrix(std::size_t N);  //NxN
        Matrix(std::size_t M, std::size_t N);
        Matrix(std::size_t M, std::size_t N, const T& value);
        Matrix(const T& m00, const T& m01,
               const T& m10, const T& m11);
        Matrix(const T& m00, const T& m01, const T& m02,
               const T& m10, const T& m11, const T& m12,
               const T& m20, const T& m21, const T& m22);

    protected:
        std::size_t m_M;
        std::size_t m_N;
        T** m_data;

    private:
        void _nullsetup();
        void _createdata();
        void _deletedata();
        void _init(const T& value);

        bool _checkSize(std::size_t M, std::size_t N) const;
        bool _checkIndexes(std::size_t m, std::size_t n) const;

    public:
        void clear();
        void resize(std::size_t M, std::size_t N);
        void resize(std::size_t M, std::size_t N, const T& value);

        bool isEmpty() const;
        std::size_t getM() const;
        std::size_t getN() const;
        void getSize(std::size_t &M, std::size_t &N) const;

        T& at(std::size_t m, std::size_t n);
        const T& at(std::size_t m, std::size_t n) const;

        Matrix<T>& inverse();
        Matrix<T>& transpose();
        Matrix<T> getcalcInverse() const;
        Matrix<T> getcalcTranspose() const;

        T getcalcDet() const;
        T getcalcTrace() const;

        bool compare(const Matrix<T>& matrix) const;
        bool compareSize(const Matrix<T>& matrix) const;

    public:
        static Matrix<T> inverse(const Matrix<T>& matrix);
        static Matrix<T> transpose(const Matrix<T>& matrix);
        static Matrix<T> multiply(const Matrix<T>& matrix1, const Matrix<T>& matrix2);
        static bool compare(const Matrix<T>& matrix1, const Matrix<T>& matrix2);

    public:
        Matrix<T>& operator = (Matrix<T>&& matrix);
        Matrix<T>& operator = (const Matrix<T>& matrix);

        operator T () const;
        operator bool () const;

        T* operator [] (std::size_t m);
        const T* operator [] (std::size_t m) const;

        T& operator () (std::size_t m, std::size_t n);
        const T& operator () (std::size_t m, std::size_t n) const;

        bool operator == (const Matrix<T>& matrix) const;
        bool operator != (const Matrix<T>& matrix) const;

        Matrix<T> operator - () const;

        Matrix<T> operator + (const T& value) const;
        Matrix<T> operator + (const Matrix<T>& matrix) const;

        Matrix<T> operator - (const T& value) const;
        Matrix<T> operator - (const Matrix<T>& matrix) const;

        Matrix<T> operator * (const T& value) const;
        Matrix<T> operator * (const Matrix<T>& matrix) const;

        Matrix<T> operator / (const T& value) const;
        Matrix<T> operator / (const Matrix<T>& matrix) const;

        Matrix<T>& operator += (const T& value);
        Matrix<T>& operator += (const Matrix<T>& matrix);

        Matrix<T>& operator -= (const T& value);
        Matrix<T>& operator -= (const Matrix<T>& matrix);

        Matrix<T>& operator *= (const T& value);
        Matrix<T>& operator *= (const Matrix<T>& matrix);

        Matrix<T>& operator /= (const T& value);
        Matrix<T>& operator /= (const Matrix<T>& matrix);

        template<typename U>
        friend Matrix<U> operator + (const U& value, const Matrix<U>& matrix);
        template<typename U>
        friend Matrix<U> operator * (const U& value, const Matrix<U>& matrix);
    };
};
////////////////////////////////////
namespace TRiOLD
{
    template<typename T>
    Matrix<T>::~Matrix()
    {
        _deletedata();
    }

    template<typename T>
    Matrix<T>::Matrix()
        : m_M(0), m_N(0), m_data(nullptr)
    {
    }

    template<typename T>
    Matrix<T>::Matrix(Matrix<T>&& matrix)
        : m_M(matrix.m_M), m_N(matrix.m_N), m_data(matrix.m_data)
    {
        matrix._nullsetup();
    }

    template<typename T>
    Matrix<T>::Matrix(const Matrix<T>& matrix)
        : m_M(matrix.m_M), m_N(matrix.m_N), m_data(nullptr)
    {
        _createdata();
        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                m_data[m][n] = matrix.m_data[m][n];
    }

    template<typename T>
    Matrix<T>::Matrix(std::size_t N)
        : Matrix(N, N)
    {
    }

    template<typename T>
    Matrix<T>::Matrix(std::size_t M, std::size_t N)
        : Matrix(M, N, 0)
    {
    }

    template<typename T>
    Matrix<T>::Matrix(std::size_t M, std::size_t N, const T& value)
        : m_M(M), m_N(N), m_data(nullptr)
    {
        if(!_checkSize(M,N))
            throw std::runtime_error("Wrong Matrix size");

        _createdata();
        _init(value);
    }


    template<typename T>
    Matrix<T>::Matrix(const T& m00, const T& m01,
                      const T& m10, const T& m11)
        : m_M(2), m_N(2), m_data(nullptr)
    {
        _createdata();
        m_data[0][0] = m00;
        m_data[0][1] = m01;
        m_data[1][0] = m10;
        m_data[1][1] = m11;
    }

    template<typename T>
    Matrix<T>::Matrix(const T& m00, const T& m01, const T& m02,
                      const T& m10, const T& m11, const T& m12,
                      const T& m20, const T& m21, const T& m22)
        : m_M(3), m_N(3), m_data(nullptr)
    {
        _createdata();
        m_data[0][0] = m00;
        m_data[0][1] = m01;
        m_data[0][2] = m02;
        m_data[1][0] = m10;
        m_data[1][1] = m11;
        m_data[1][2] = m12;
        m_data[2][0] = m20;
        m_data[2][1] = m21;
        m_data[2][2] = m22;
    }

    template<typename T>
    void TRiOLD::Matrix<T>::_nullsetup()
    {
        m_M = 0;
        m_N = 0;
        m_data = nullptr;
    }

    template<typename T>
    void Matrix<T>::_createdata()
    {
        m_data = new T* [m_M];
        for(std::size_t m = 0; m < m_M; ++m)
            m_data[m] = new T[m_N];
    }

    template<typename T>
    void Matrix<T>::_deletedata()
    {
        for(std::size_t m = 0; m < m_M; ++m)
            delete [] m_data[m];
        delete [] m_data;

    }

    template<typename T>
    void Matrix<T>::_init(const T& value)
    {
        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                m_data[m][n] = value;
    }

    template<typename T>
    bool Matrix<T>::_checkSize(std::size_t M, std::size_t N) const
    {
        if(M == 0 && N == 0)
            return true;
        if(M*N == 0 || M*N > 4294967295)
            return false;
        return true;
    }

    template<typename T>
    bool Matrix<T>::_checkIndexes(std::size_t m, std::size_t n) const
    {
        return !(m >= m_M || n >= m_N);
    }

    template<typename T>
    void TRiOLD::Matrix<T>::clear()
    {
        _deletedata();
        _nullsetup();
    }

    template<typename T>
    void Matrix<T>::resize(std::size_t M, std::size_t N)
    {
        resize(M, N, 0);
    }

    template<typename T>
    void Matrix<T>::resize(std::size_t M, std::size_t N, const T& value)
    {
        if(M == m_M && N == m_N)
            return;

        if(M == 0 && N == 0)
        {
            _deletedata();
            _nullsetup();
            return;
        }

        std::size_t Mmin = M < m_M ? M : m_M;
        std::size_t Nmin = N < m_N ? N : m_N;
        Matrix<T> res(M, N, value);

        for(std::size_t m = 0; m < Mmin; ++m)
            for(std::size_t n = 0; n < Nmin; ++n)
                res[m][n] = m_data[m][n];
        *this = std::move(res);
    }

    template<typename T>
    bool Matrix<T>::isEmpty() const
    {
        return !(m_data);
    }

    template<typename T>
    std::size_t Matrix<T>::getM() const
    {
        return m_M;
    }

    template<typename T>
    std::size_t Matrix<T>::getN() const
    {
        return m_N;
    }

    template<typename T>
    void Matrix<T>::getSize(std::size_t& M, std::size_t& N) const
    {
        M = m_M;
        N = m_N;
    }

    template<typename T>
    T& Matrix<T>::at(std::size_t m, std::size_t n)
    {
        if(!_checkIndexes(m, n))
            throw std::runtime_error("Out of Matrix range");
        return m_data[m][n];
    }

    template<typename T>
    const T& Matrix<T>::at(std::size_t m, std::size_t n) const
    {
        if(!_checkIndexes(m, n))
            throw std::runtime_error("Out of Matrix range");
        return m_data[m][n];
    }

    template<typename T>
    Matrix<T>& Matrix<T>::inverse()
    {
        Matrix<T> res = inverse(*this);
        *this = std::move(res);
        return *this;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::transpose()
    {
        Matrix<T> res = transpose(*this);
        *this = std::move(res);
        return *this;
    }

    template<typename T>
    Matrix<T> Matrix<T>::getcalcInverse() const
    {
        return inverse(*this);
    }

    template<typename T>
    Matrix<T> Matrix<T>::getcalcTranspose() const
    {
        return transpose(*this);
    }

    template<typename T>
    T Matrix<T>::getcalcDet() const
    {

    }

    template<typename T>
    T Matrix<T>::getcalcTrace() const
    {

    }

    template<typename T>
    bool Matrix<T>::compare(const Matrix<T>& matrix) const
    {
        return compare(*this, matrix);
    }

    template<typename T>
    bool Matrix<T>::compareSize(const Matrix<T>& matrix) const
    {
        return (m_M == matrix.m_M && m_N == matrix.m_N);
    }

    template<typename T>
    Matrix<T> Matrix<T>::inverse(const Matrix<T>& matrix)
    {

    }

    template<typename T>
    Matrix<T> Matrix<T>::transpose(const Matrix<T>& matrix)
    {
        if(matrix.isEmpty())
            return Matrix<T>();

        std::size_t M = matrix.getM();
        std::size_t N = matrix.getN();
        Matrix<T> res(N, M);
        for(std::size_t m = 0; m < M; ++m)
            for(std::size_t n = 0; n < N; ++n)
                res[n][m] = matrix[m][n];
        return std::move(res);
    }

    template<typename T>
    Matrix<T> Matrix<T>::multiply(const Matrix<T>& matrix1, const Matrix<T>& matrix2)
    {
        if(matrix1.isEmpty() || matrix2.isEmpty())
            throw std::runtime_error("Matrix1 or Matrix2 is empty");

        if(matrix1.getN() != matrix2.getM())
            throw std::runtime_error("Matrix1 and Matrix2 are not agreed");

        std::size_t K = matrix1.getN();
        std::size_t M = matrix1.getM();
        std::size_t N = matrix2.getN();
        Matrix<T> res(M,N);
        for(std::size_t m = 0; m < M; ++m)
            for(std::size_t n = 0; n < N; ++n)
                for(std::size_t k = 0; k < K; ++k)
                    res[m][n] += matrix1[m][k] * matrix2[k][n];
        return std::move(res);
    }

    template<typename T>
    bool Matrix<T>::compare(const Matrix<T>& matrix1, const Matrix<T>& matrix2)
    {
        if(!matrix1.compareSize(matrix2))
            return false;
        std::size_t M = matrix1.getM();
        std::size_t N = matrix1.getN();
        for(std::size_t m = 0; m < M; ++m)
            for(std::size_t n = 0; n < N; ++n)
                if(matrix1[m][n] != matrix2[m][n])
                    return false;
        return true;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator = (Matrix<T>&& matrix)
    {
        if(&matrix == this)
            return *this;

        _deletedata();
        m_M = matrix.m_M;
        m_N = matrix.m_N;
        m_data = matrix.m_data;
        matrix._nullsetup();
        return *this;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator = (const Matrix<T>& matrix)
    {
        if(&matrix == this)
            return *this;

        _deletedata();
        m_M = matrix.m_M;
        m_N = matrix.m_N;
        _createdata();
        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                m_data[m][n] = matrix.m_data[m][n];
        return *this;
    }

    template<typename T>
    Matrix<T>::operator T() const
    {
        return m_data ? **m_data : 0;
    }

    template<typename T>
    Matrix<T>::operator bool() const
    {
        return m_data ? true : false;
    }

    template<typename T>
    T* Matrix<T>::operator [] (std::size_t m)
    {
        return m_data[m];
    }

    template<typename T>
    const T* Matrix<T>::operator [] (std::size_t m) const
    {
        return m_data[m];
    }

    template<typename T>
    T& Matrix<T>::operator () (std::size_t m, std::size_t n)
    {
        return at(m, n);
    }

    template<typename T>
    const T& Matrix<T>::operator () (std::size_t m, std::size_t n) const
    {
        return at(m, n);
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator - () const
    {
        Matrix<T> newMatrix(*this);
        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                newMatrix[m][n] = -newMatrix[m][n];
        return std::move(newMatrix);
    }

    template<typename T>
    bool Matrix<T>::operator == (const Matrix<T>& matrix) const
    {
        return compare(matrix);
    }

    template<typename T>
    bool Matrix<T>::operator != (const Matrix<T>& matrix) const
    {
        return !compare(matrix);
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator + (const T& value) const
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        Matrix<T> res(m_M, m_N);
        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                res[m][n] = m_data[m][n] + value;
        return std::move(res);
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator + (const Matrix<T>& matrix) const
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        if(!compareSize(matrix))
            throw std::runtime_error("Matrices have different sizes");

        Matrix<T> res(m_M, m_N);
        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                res[m][n] = m_data[m][n] + matrix[m][n];
        return std::move(res);
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator - (const T& value) const
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        Matrix<T> res(m_M, m_N);
        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                res[m][n] = m_data[m][n] - value;
        return std::move(res);
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator - (const Matrix<T>& matrix) const
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        if(!compareSize(matrix))
            throw std::runtime_error("Matrices have different sizes");

        Matrix<T> res(m_M, m_N);
        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                res[m][n] = m_data[m][n] - matrix[m][n];
        return std::move(res);
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator * (const T& value) const
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        Matrix<T> res(m_M, m_N);
        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                res[m][n] = m_data[m][n] * value;
        return std::move(res);
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator * (const Matrix<T>& matrix) const
    {
        return multiply(*this, matrix);
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator / (const T& value) const
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        if(!value)
            throw std::runtime_error("Value is similar to zero");

        Matrix<T> res(m_M, m_N);
        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                res[m][n] = m_data[m][n] / value;
        return std::move(res);
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator / (const Matrix<T>& matrix) const
    {
        return multiply(*this, inverse(matrix));
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator += (const T& value)
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                m_data[m][n] = m_data[m][n] + value;
        return *this;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator += (const Matrix<T>& matrix)
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        if(!compareSize(matrix))
            throw std::runtime_error("Matrices have different sizes");

        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                m_data[m][n] = m_data[m][n] + matrix[m][n];
        return *this;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator -= (const T& value)
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                m_data[m][n] = m_data[m][n] - value;
        return *this;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator -= (const Matrix<T>& matrix)
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        if(!compareSize(matrix))
            throw std::runtime_error("Matrices have different sizes");

        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                m_data[m][n] = m_data[m][n] - matrix[m][n];
        return *this;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator *= (const T& value)
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                m_data[m][n] = m_data[m][n] * value;
        return *this;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator *= (const Matrix<T>& matrix)
    {
        Matrix<T> res = *this * matrix;
        *this = std::move(res);
        return *this;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator /= (const T& value)
    {
        if(isEmpty())
            throw std::runtime_error("Matrix is empty");

        if(!value)
            throw std::runtime_error("Value is similar to zero");

        for(std::size_t m = 0; m < m_M; ++m)
            for(std::size_t n = 0; n < m_N; ++n)
                m_data[m][n] = m_data[m][n] * value;
        return *this;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator /= (const Matrix<T>& matrix)
    {
        Matrix<T> res = *this / matrix;
        *this = std::move(res);
        return *this;
    }

    template<typename U>
    Matrix<U> operator + (const U& value, const Matrix<U>& matrix)
    {
        return matrix + value;
    }

    template<typename U>
    Matrix<U> operator * (const U& value, const Matrix<U>& matrix)
    {
        return matrix * value;
    }
}
////////////////////////////////////
#endif // TMATRIX_H
