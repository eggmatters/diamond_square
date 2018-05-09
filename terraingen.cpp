#include <time.h>
#include <stdio.h>
#include <QtDebug>
#include <QString>

#include "terraingen.h"

Terraingen::Terraingen(int dimension, int polyDepth, int range, int roughness) {
    dimensions = dimension * polyDepth;
    srand((unsigned)time(0));
    this->range = range;
    this->roughness = roughness;
}

Terraingen::Terraingen(const int dimension, const int polyDepth, const QVector<terrainConfig> &terrainConfigs) {
    size = dimension;
    dimensions = dimension * polyDepth;
    srand((unsigned)time(0));
    foreach(terrainConfig tc, terrainConfigs) {
        this->terrainConfigs.append(tc);
    }

}



QVector< QVector<float> > Terraingen::generateTerrain() {
    //set heights for first 4 corners plus middle.
    int n = dimensions;
    terrain = QVector< QVector<float> >(dimensions + 1, QVector<float>(dimensions + 1, 0.0f));
    terrain[0][0] = randomInitialNumber();
    terrain[0][n] = randomInitialNumber();
    terrain[n][n] = randomInitialNumber();
    terrain[n][0] = randomInitialNumber();

    diamondSquare(0, 0,
                  0, n,
                  n, n,
                  n, 0);
    this->erode(0.2f);
    return terrain;

}

QVector< QVector<float> > Terraingen::generateTotalTerrain() {    
    setSectorMap();
    int n = dimensions * terrainWidth;
    terrain = QVector< QVector<float> >(n + 1, QVector<float>(n + 1, 0.0f));
    setCurrentSector(0,0);
    terrain[0][0] = randomInitialNumber();
    setCurrentSector(0,n);
    terrain[0][n] = randomInitialNumber();
    setCurrentSector(n,n);
    terrain[n][n] = randomInitialNumber();
    setCurrentSector(n,0);
    terrain[n][0] = randomInitialNumber();
    diamondSquare(0, 0,
                  0, n,
                  n, n,
                  n, 0);
    this->erode(0.2f);
    return terrain;

}

float Terraingen::randomHeightValue(float sideAverages, float smoothness) {

    if (sideAverages == 0)
        sideAverages = 1;
    float rnd = (rand() % (int)sideAverages);
    rnd = (rnd / smoothness) + sideAverages;
    if (rnd == 0)
        rnd = 1;

    return rnd;// * 0.5f;
}

float Terraingen::randomInitialNumber() {
    range = sectorMap[currentSector.x()][currentSector.y()].range;
    float rnd = rand() % range;
    return rnd;
}

void Terraingen::diamondSquare(int side1_x, int side1_y,
                               int side2_x, int side2_y,
                               int side3_x, int side3_y,
                               int side4_x, int side4_y)
{    
    //compute average of all 4 corners for the square step
    float p1 = (terrain[side1_x][side1_y] != 0) ? terrain[side1_x][side1_y] : 1.0f;
    float p2 = (terrain[side2_x][side2_y] != 0) ? terrain[side2_x][side2_y] : 1.0f;
    float p3 = (terrain[side3_x][side3_y] != 0) ? terrain[side3_x][side3_y] : 1.0f;
    float p4 = (terrain[side3_x][side3_y] != 0) ? terrain[side3_x][side3_y] : 1.0f;

    float sideAverage = (p1 + p2 + p3 + p4) / 4;
    float smoothness;
    //Calculate midpoints relative to the values passed in:
    int midX = (side3_x - side1_x) / 2 + side1_x;
    int midY = (side3_y - side1_y) / 2 + side1_y;

    //if midpoint distance from a calculated distance is one unit, return:
    if (midX - side1_x != 0 ) {
        setCurrentSector(side1_x, side1_y);
        //calculate smoothness as a factor of how deep into each quadrant we may be:
        //helps prevent spiking of random values:
        sectorMap[currentSector.x()][currentSector.y()].roughness;
        smoothness = roughness / (midX - side1_x );

        if (smoothness == 0)
            smoothness = 1;
        //calculate midpoint
        terrain[midX][midY] = randomHeightValue(sideAverage, smoothness );
        //diamond step:
        if (terrain[side1_x][midY] == 0.0f )
            terrain[side1_x][midY] = randomHeightValue(sideAverage, smoothness );
        if (terrain[midX][side2_y] == 0.0f )
            terrain[midX][side2_y] = randomHeightValue(sideAverage, smoothness );
        if (terrain[side3_x][midY] == 0.0f )
            terrain[side3_x][midY] = randomHeightValue(sideAverage, smoothness );
        if (terrain[midX][side4_y] == 0.0f)
            terrain[midX][side4_y] = randomHeightValue(sideAverage, smoothness );
        //recurse down through remaining quadrants:
        diamondSquare(side1_x, side1_y, side2_x, midY, midX, midY, midX, side4_y);
        diamondSquare(side1_x, midY, side2_x, side2_y, midX, side3_y, midX, midY);
        diamondSquare(midX, midY, midX, side2_y, side3_x, side2_y, side4_x, midY);
        if (midX - side1_x > 1) {
            diamondSquare(midX, side1_y, midX, midY, side3_x, midY, side4_x, side4_y);
        }
    }
    else {
        return;
    }
}

void Terraingen::erode(double erosionFactor) {
    //(up to a) 8-point walk around the perimiter of a point.
    //check height differential for all points and smooth if too high.
    float heightAverage;
    for (int i = 0; i < terrain.size() - 1; i++) {
        for (int j = 0; j < terrain.size() - 1; j++) {
            int iDown  = (i != 0) ? (i - 1) : i;
            int jLeft  = (j != 0) ? (j - 1) : j;
            int iUp    = ((i + 1) < (terrain.size() - 1)) ? i + 1 : (terrain.size() - 1);
            int jRight = ((j + 1) < (terrain.size() - 1)) ? j + 1 : (terrain.size() - 1);
            heightAverage = ( terrain[iDown][jLeft]  +
                              terrain[i][jLeft]      +
                              terrain[iUp][jLeft]    +
                              terrain[iUp][j]        +
                              terrain[iUp][jRight]   +
                              terrain[i][jRight]     +
                              terrain[iDown][jRight] +
                              terrain[iDown][j]    ) / 8;
            if (fabs(heightAverage ) > fabs(erosionFactor)) {
                terrain[i][j] = heightAverage;
            }
        }
    }
}

void Terraingen::setSectorMap() {
    terrainWidth = 0;
    int mapSize = terrainConfigs.size();

    while (mapSize > 2 ) {
        if ( terrainWidth * terrainWidth == terrainConfigs.size()) { break; }
        else {
            mapSize--;
            terrainWidth = mapSize / 2;
        }
    }
    sectorMap = QVector< QVector<terrainConfig> >(terrainWidth, QVector<terrainConfig>(terrainWidth, plains));
    int i = 0;
    for (int x = 0; x < terrainWidth; x++) {
        for (int y = 0; y < terrainWidth; y++) {
            sectorMap[x][y] = terrainConfigs[i];
            //sectorMap[x][y] = plains;
            i++;
        }
    }
}

void Terraingen::setCurrentSector(int x, int y) {
    int px = (x / size == terrainWidth) ? terrainWidth - 1 :  (x / size);
    int py = (y / size == terrainWidth) ? terrainWidth - 1 :  (y / size);
    this->currentSector = QPoint(px, py);
}
