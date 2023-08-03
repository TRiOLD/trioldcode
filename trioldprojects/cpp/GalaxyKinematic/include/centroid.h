////////////////////////////////////
#ifndef CENTROID_H
#define CENTROID_H

////////////////////////////////////
#include "star.h"

#include <TMatrix.h>
using namespace TRiOLD;

////////////////////////////////////
class Centroid : public Star
{
public:
    Centroid();
    Centroid(const Cartesian& GCC, const Cartesian& GCV, std::size_t starsAmount,
             const Cartesian& V0, const Matrix<double>& M);
    Centroid(const Cartesian& GCC, const Cartesian& GCV, std::size_t starsAmount,
             const Cartesian& V0, const Matrix<double>& Mm, const Matrix<double>& Mp);
    Centroid(const Cartesian& GCC, const Cartesian& GCV, std::size_t starsAmount,
             double u, double v, double w, double w1, double w2, double w3,
             double mp12, double mp23, double mp13, double mp11, double mp22, double mp33);

protected:
    std::size_t m_starsAmount;
    Cartesian m_V0;
    Matrix<double> m_Mm;
    Matrix<double> m_Mp;

public:
    std::size_t getStarsAmount() const;

    Cartesian getV0() const;
    Matrix<double> getMm() const;
    Matrix<double> getMp() const;
    Matrix<double> getcalcM() const;

    void getKP(
        double& u, double& v, double& w, double& w1, double& w2, double& w3,
        double& mp12, double& mp23, double& mp13, double& mp11, double& mp22, double& mp33) const;

    Cartesian getcalcV0_localRot(double R_Sun) const;
    Matrix<double> getcalcMm_localRot(double R_Sun) const;
    Matrix<double> getcalcMp_localRot(double R_Sun) const;
    Matrix<double> getcalcM_localRot(double R_Sun) const;

private:
    static void _expandTensor(Matrix<double>& Mm, Matrix<double>& Mp, const Matrix<double>& M);
    static Matrix<double> _rotateMatrix(const Matrix<double>& matrix, double angleXY);

};

////////////////////////////////////
#endif // CENTROID_H
