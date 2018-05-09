#ifndef WORLD_H
#define WORLD_H

#ifndef QVECTOR_H
#include <QVector>
#endif

#ifndef QVECTOR3D_H
#include <QVector3D>
#endif

#ifndef QVECTOR32D_H
#include <QVector2D>
#endif

#ifndef QPOINT_H
#include <QPoint>
#endif

#include "terraingen.h"
#include "terrainConfig.h"
#include "sector.h"

#define BOUNDS_CHECK 20
#define NORTHEAST    1
#define NORTH        2
#define NORTHWEST    3
#define WEST         4
#define SOUTHWEST    5
#define SOUTH        6
#define SOUTHEAST    7
#define EAST         8

class World
{
public:
    World();
    World(int size, int polyDepth);
    QVector3D initCamera();
    float getYPosition(QVector3D camera);
    Sector* currentSector;
    QVector<QVector3D> vertices;
    QVector<QVector2D> textures;

private:    
    int size;
    int polyDepth;
    QVector<QVector<Sector*> > worldMap;
    QVector<terrainConfig>  sectorTypes;
    QPoint currentSectorIndex;
    int xPos;
    int zPos;
    int worldMapWidth;
    void setWorldMap();
    void checkSectorPosition();
    void appendVertices(QPoint direction);
    void switchCurrentSector(QPoint direction);
    void updateVertices();
    bool checkCurrentSector(QVector3D camera);


};


#endif // WORLD_H
