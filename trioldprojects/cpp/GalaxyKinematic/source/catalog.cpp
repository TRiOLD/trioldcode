////////////////////////////////////
#include "catalog.h"

#include <cmath>

#include <TException.h>
#include <TFiletable.h>
using namespace TRiOLD;

////////////////////////////////////
double _d2r(double angle)
{
    return angle / 180.0 * M_PI;
}

std::list<Star> Catalog::readCatalog(const std::string& filepath,
                                     const ConfigTable& config)
{
    Filetable file(filepath);
    if(!file.isOpen())
        Exception("Catalog::readCatalog() !file.isOpen()");

    Filetable::Row row;
    if(config.isWithHeader)
        file >> row;

    std::list<Star> res;
    while(file >> row)
    {
        Star::Equatorial EqC(
            (config.coordInRad) ? row.at(config.cn_ra).toDouble() :
                                  _d2r(row.at(config.cn_ra).toDouble()),
            (config.coordInRad) ? row.at(config.cn_de).toDouble() :
                                  _d2r(row.at(config.cn_de).toDouble()),
            1.0 / row.at(config.cn_plx).toDouble());
        Star::Equatorial EqV(
            row.at(config.cn_muRa).toDouble(),
            row.at(config.cn_muDe).toDouble(),
            row.at(config.cn_Vr).toDouble());
        res.push_back({EqC, EqV});
    }
    file.close();
    return res;
}

std::list<Star> Catalog::readCatalog_agreedStruct(const std::string& filepath)
{
    Filetable file(filepath);
    if(!file.isOpen())
        Exception("Catalog::readCatalog() !file.isOpen()");

    Filetable::Row row;
    file >> row;

    std::list<Star> res;
    while(file >> row)
    {
        Star::Cartesian GCC(
            row.at(0).toDouble(), row.at(1).toDouble(), row.at(2).toDouble());
        Star::Cartesian GCV(
            row.at(3).toDouble(), row.at(4).toDouble(), row.at(5).toDouble());
        res.push_back({GCC, GCV});
    }
    file.close();
    return res;
}

void Catalog::writeCatalog_agreedStruct(const std::string& filepath,
                                        const std::list<Star>& stars)
{
    Filetable file(filepath, Filetable::OUT, Filetable::CSV);
    if(!file.isOpen())
        Exception("Catalog::readCatalog() !file.isOpen()");

    file << Filetable::Row({
        {"X[kpc]"}, {"Y[kpc]"}, {"Z[kpc]"},
        {"VX[km/s]"}, {"VY[km/s]"}, {"VZ[km/s]"}});

    for(const auto &it : stars)
    {
        Star::Cartesian GCC = it.getGCC();
        Star::Cartesian GCV = it.getGCV();
        file << Filetable::Row(
            {{GCC.x, 12}, {GCC.y, 12}, {GCC.z, 12},
             {GCV.x, 12}, {GCV.y, 12}, {GCV.z, 12}});
    }
    file.close();
}

////////////////////////////////////

void Catalog::writeCentroids(const std::string& filepath,
                             const std::vector<Centroid>& centroids,
                             const ConfigConstants& config)
{
    Filetable file(filepath, Filetable::OUT, Filetable::CSV);
    if(!file.isOpen())
        Exception("Catalog::readCatalog() !file.isOpen()");

    file << Filetable::Row({
        {"starsAmount"},
        {"X[kpc]"}, {"Y[kpc]"}, {"Z[kpc]"},
        {"VX[km/s]"}, {"VY[km/s]"}, {"VZ[km/s]"},
        {"R[kpc]"}, {"theta[rad]"}, {"Z[kpc]"},
        {"VR[km/s]"}, {"Vtheta[km/s]"}, {"VZ[km/s]"},
        {"u[km/s]"}, {"v[km/s]"}, {"w[km/s]"},
        {"w1[km/s/kpc]"}, {"w2[km/s/kpc]"}, {"w3[km/s/kpc]"},
        {"mp12[km/s/kpc]"}, {"mp23[km/s/kpc]"}, {"mp13[km/s/kpc]"},
        {"mp11[km/s/kpc]"}, {"mp22[km/s/kpc]"}, {"mp33[km/s/kpc]"}});

    for(const auto &it : centroids)
    {
        Centroid::Cartesian GCC = it.getGCC();
        Centroid::Cartesian GCV = it.getGCV();
        Centroid::Cylindrical GCCC, GCCV;
        it.getcalcGCCCandV(GCCC, GCCV,
            config.R_Sun, {config.VX_Sun, config.VY_Sun, config.VZ_Sun});
        double u, v, w, w1, w2, w3, mp12, mp23, mp13, mp11, mp22, mp33;
        it.getKP(u, v, w, w1, w2, w3, mp12, mp23, mp13, mp11, mp22, mp33);

        const int p = 12; // precision
        file << Filetable::Row({
            {(int)it.getStarsAmount()},
            {GCC.x, p}, {GCC.y, p}, {GCC.z, p},
            {GCV.x, p}, {GCV.y, p}, {GCV.z, p},
            {GCCC.R, p}, {GCCC.theta, p}, {GCCC.Z, p},
            {GCCV.R, p}, {GCCV.theta, p}, {GCCV.Z, p},
            {u, p}, {v, p}, {w, p},
            {w1, p}, {w2, p}, {w3, p},
            {mp12, p}, {mp23, p}, {mp13, p},
            {mp11, p}, {mp22, p}, {mp33, p}});
    }
    file.close();
}
