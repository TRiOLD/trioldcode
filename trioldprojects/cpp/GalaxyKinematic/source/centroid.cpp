////////////////////////////////////
#include "centroid.h"

#include <iostream>
#include <cmath>

////////////////////////////////////
Centroid::Centroid()
    :Star(), m_starsAmount(0), m_V0(), m_Mm(), m_Mp()
{
}

Centroid::Centroid(const Cartesian& GCC, const Cartesian& GCV, std::size_t starsAmount,
                   const Cartesian& V0, const Matrix<double>& M)
    :Star(GCC, GCV), m_starsAmount(starsAmount), m_V0(V0)
{
    _expandTensor(m_Mm, m_Mp, M);
}

Centroid::Centroid(const Cartesian& GCC, const Cartesian& GCV, std::size_t starsAmount,
                   const Cartesian& V0, const Matrix<double>& Mm, const Matrix<double>& Mp)
    :Star(GCC, GCV), m_starsAmount(starsAmount), m_V0(V0), m_Mm(Mm), m_Mp(Mp)
{
}

Centroid::Centroid(const Cartesian& GCC, const Cartesian& GCV, std::size_t starsAmount,
                   double u, double v, double w, double w1, double w2, double w3,
                   double mp12, double mp23, double mp13, double mp11, double mp22, double mp33)
    :Star(GCC, GCV), m_starsAmount(starsAmount)
{
    m_V0 = Cartesian(u,v,w);
    m_Mm = Matrix<double>(
        0.0, -w3, w2,
        w3, 0.0, -w1,
       -w2, w1, 0.0);
    m_Mp = Matrix<double>(
        mp11, mp12, mp13,
        mp12, mp22, mp23,
        mp13, mp23, mp33);
}

std::size_t Centroid::getStarsAmount() const
{
    return m_starsAmount;
}

Star::Cartesian Centroid::getV0() const
{
    return m_V0;
}

Matrix<double> Centroid::getMm() const
{
    return m_Mm;
}

Matrix<double> Centroid::getMp() const
{
    return m_Mp;
}

Matrix<double> Centroid::getcalcM() const
{
    return m_Mm + m_Mp;
}

void Centroid::getKP(
    double& u, double& v, double& w, double& w1, double& w2, double& w3,
        double& mp12, double& mp23, double& mp13, double& mp11, double& mp22, double& mp33) const
{
    u = m_V0.x; v = m_V0.y; w = m_V0.z;
    w1 = m_Mm[2][1]; w2 = m_Mm[0][2]; w3 = m_Mm[1][0];
    mp12 = m_Mp[0][1]; mp23 = m_Mp[1][2]; mp13 = m_Mp[0][2];
    mp11 = m_Mp[0][0]; mp22 = m_Mp[1][1]; mp33 = m_Mp[2][2];
}

Star::Cartesian Centroid::getcalcV0_localRot(double R_Sun) const
{
    return _rotateVector(m_V0, _calcRotAngleXY(R_Sun, m_GCC));
}

Matrix<double> Centroid::getcalcMm_localRot(double R_Sun) const
{
    return _rotateMatrix(m_Mm, _calcRotAngleXY(R_Sun, m_GCC));
}

Matrix<double> Centroid::getcalcMp_localRot(double R_Sun) const
{
    return _rotateMatrix(m_Mp, _calcRotAngleXY(R_Sun, m_GCC));
}

Matrix<double> Centroid::getcalcM_localRot(double R_Sun) const
{
    return _rotateMatrix(m_Mm+m_Mp, _calcRotAngleXY(R_Sun, m_GCC));
}

void Centroid::_expandTensor(Matrix<double>& Mm, Matrix<double>& Mp, const Matrix<double>& M)
{
    Mp = Matrix<double>(
        M[0][0], 0.5*(M[0][1]+M[1][0]), 0.5*(M[0][2]+M[2][0]),
        0.5*(M[0][1]+M[1][0]), M[1][1], 0.5*(M[1][2]+M[2][1]),
        0.5*(M[0][2]+M[2][0]), 0.5*(M[1][2]+M[2][1]), M[2][2]);
    Mm = M - Mp;
}

Matrix<double> Centroid::_rotateMatrix(const Matrix<double>& matrix, double angleXY)
{
    Matrix<double> R(
        std::cos(angleXY), -std::sin(angleXY), 0.0,
        std::sin(angleXY),  std::cos(angleXY), 0.0,
        0.0, 0.0, 1.0);
    return R * matrix * R.getcalcTranspose();
}

////////////////////////////////////
