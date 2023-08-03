////////////////////////////////////
#ifndef CATALOG_H
#define CATALOG_H

////////////////////////////////////
#include "star.h"
#include "centroid.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>

////////////////////////////////////
namespace Catalog
{
    ////////////////////////////////
    struct ConfigTable
    {
        bool isWithHeader = true;
        bool coordInRad = false;

        unsigned int cn_ra = 0;
        unsigned int cn_de = 1;
        unsigned int cn_plx = 2;
        unsigned int cn_muRa = 3;
        unsigned int cn_muDe = 4;
        unsigned int cn_Vr = 5;
    };

    struct ConfigConstants
    {
        double R_Sun =  8.28;           // kpc

        double VX_Sun = 11.1;           // km/sec
        double VY_Sun = 243.13;         // km/sec
        double VZ_Sun = 8.31;           // km/sec
    };
    ////////////////////////////////

    std::list<Star> readCatalog(const std::string& filepath, const ConfigTable& config);
    std::list<Star> readCatalog_agreedStruct(const std::string& filepath);
    void writeCatalog_agreedStruct(const std::string& filepath, const std::list<Star>& stars);

    void writeCentroids(const std::string& filepath, const std::vector<Centroid>& centroids,
                        const ConfigConstants& config);


};

////////////////////////////////////
#endif // CATALOG_H
