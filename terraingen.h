#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include <vector>
#include <math.h>
#include <stdlib.h>
#include "terrainConfig.h"

#ifndef QVECTOR_H
#include <QVector>
#endif

#ifndef QPOINT_H
#include <QPoint>
#endif

class Terraingen
{
public:
    Terraingen(int dimension, int polyDepth, int range, int roughness);
    Terraingen(const int dimension, const int polyDepth, const QVector<terrainConfig> &terrainConfigs);
    QVector<QVector<float> > generateTerrain();
    QVector<QVector<float> > generateTotalTerrain();

private:
    int size;
    int dimensions;
    int depth;
    int range;
    int roughness;
    int terrainWidth;
    int configWidth;

    terrainConfig currentConfig;
    QVector<terrainConfig> terrainConfigs;
    QVector<QVector<float> > terrain;
    QVector< QVector<terrainConfig> > sectorMap;
    QPoint currentSector;
    void setSectorMap();
    void setCurrentSector(int x, int y);
    float randomInitialNumber();
    float randomHeightValue(float sideAverages, float smoothness);
    void diamondSquare(int side1_x,
                       int side1_y,
                       int side2_x,
                       int side2_y,
                       int side3_x,
                       int side3_y,
                       int side4_x,
                       int side4_y);
    void erode(double erosionFactor);

};

#endif // TERRAINGEN_H
