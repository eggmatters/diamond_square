#include "world.h"

World::World()
{
}

World::World(int size, int polyDepth) : size(size), polyDepth(polyDepth) {

    sectorTypes.append(mountains);
    sectorTypes.append(morrasse);
    sectorTypes.append(hills);
    sectorTypes.append(scrub);
    sectorTypes.append(plains);
    sectorTypes.append(plains);
    sectorTypes.append(scrub);
    sectorTypes.append(hills);
    sectorTypes.append(morrasse);
    setWorldMap();
}

void World::setWorldMap() {

    Terraingen *terrain = new Terraingen(size, 1, sectorTypes);
    QVector<QVector<float> > worldHeightMap = terrain->generateTotalTerrain();
    worldMapWidth = 0;
    QPoint origin = QPoint(0,0);
    int mapSize = sectorTypes.size();
    int width = mapSize / 2;
    //find integer square root of map array size:
    while (mapSize > 2 ) {
        if ( width * width == sectorTypes.size()) { break; }
        else {
            mapSize--;
            width = mapSize / 2;
        }
    }
    worldMap = QVector<QVector<Sector*> >(width + 1, QVector<Sector*>(width + 1));
    worldMapWidth = width;
    int i = 0;
    //Is the ordering correct?
    for (int x = 0; x < worldMapWidth; x++) {
        for (int y = 0; y < worldMapWidth; y++) {
            origin.setX(x * size); origin.setY(y * size);
            Sector *terrainSector = new Sector(i, size, polyDepth, origin, worldHeightMap);
            this->worldMap[x][y] = terrainSector;
            i++;
        }
    }
}

QVector3D World::initCamera() {
    this->currentSector = worldMap[1][1];
    currentSectorIndex = QPoint(1,1);
    currentSector->toBeRendered = true;
    currentSector->isRendered = true;
    vertices = currentSector->vertices;
    textures = currentSector->textures;
    QVector3D camera;
    QPoint origin = currentSector->getOrigin();

    int center = currentSector->terrainSize() / (2 * polyDepth);

    camera.setX(-((qreal)origin.x() + (qreal)center));
    camera.setZ(-((qreal)origin.y() + (qreal)center));
    camera.setY(getYPosition(camera));
    camera.setY(0);
    return camera;
}

float World::getYPosition(QVector3D camera) {    
    xPos = abs(trunc(camera.x()));
    zPos = abs(trunc (camera.z()));
    //Check and see if we need to switch sectors:
    checkCurrentSector(camera);
    QPoint origin = currentSector->getOrigin();
    float xMantissa = fabs(camera.x()) - xPos;
    float zMantissa = fabs(camera.z()) - zPos;
    int width = currentSector->terrainSize();
    int xDiv = xMantissa * 1000;
    int zDiv = (fabs(zMantissa) > 0.000001f) ? zMantissa * 1000 : 1000;
    int offset = 0;
    if (zDiv != 0) {
        offset = (fabs(xDiv  / zDiv) > 0.0f) ? 1 : 0;
    }
    int xPosOffset = xPos - origin.x();
    int zPosOffset = zPos - origin.y();

    int planeIndex = (2 * (width) * zPosOffset) + (xPosOffset * 2) + offset;

    //Check and see if we need to render another sector:
    checkSectorPosition();

    return currentSector->planes[planeIndex]->getYPosition(camera);
}


void World::checkSectorPosition() {
    QPoint boundsDirection = QPoint(0,0);
    QPoint origin = currentSector->getOrigin();
    int xPosOffset = xPos - origin.x();
    int zPosOffset = zPos - origin.y();
    //qDebug("xPosOffset = %d zPosOffset = %d",xPosOffset, zPosOffset);
    if (zPosOffset <= BOUNDS_CHECK ) {
        boundsDirection.setY(-1);
    }
    if ((zPosOffset + BOUNDS_CHECK) >= size) {
        boundsDirection.setY(1);
    }
    if (xPosOffset <= BOUNDS_CHECK) {
        boundsDirection.setX(-1);
    }
    if ((xPosOffset + BOUNDS_CHECK) >= size) {
        boundsDirection.setX(1);
    }
    if (boundsDirection.x() == 0 && boundsDirection.y() == 0) { return; }

    appendVertices(boundsDirection);
}

void World::appendVertices(QPoint direction) {
    int nextX = currentSectorIndex.x() + direction.x();
    int nextY = currentSectorIndex.y() + direction.y();
    if (nextX < 0) {
        nextX = worldMapWidth - 1;
    }
    if (nextY < 0) {
        nextY = worldMapWidth -1;
    }
    if (nextX + 1 >= worldMapWidth + 1) {
        nextX = 0;
    }
    if (nextY + 1 >= worldMapWidth + 1) {
        nextY = 0;
    }
    worldMap[nextX][nextY]->setSectorToRender();
    //Render additional sectors from the corners:
    //NORTHEAST:
    if (direction.x() == -1 && direction.y() == 1) {
        QPoint eastDirection = QPoint(-1, 0);
        QPoint northDirection = QPoint(0,1);
        appendVertices(eastDirection);
        appendVertices(northDirection);
    }
    //NORTHWEST:
    if (direction.x() == 1 && direction.y() == 1) {
        QPoint northDirection = QPoint(0,1);
        QPoint westDirection = QPoint(1,0);
        appendVertices(northDirection);
        appendVertices(westDirection);
    }
    //SOUTHWEST:
    if (direction.x() == 1 && direction.y() == -1) {
        QPoint westDirection = QPoint(1,0);
        QPoint southDirection = QPoint(0, -1);
        appendVertices(westDirection);
        appendVertices(southDirection);
    }
    //SOUTHEAST
    if (direction.x() == -1 && direction.y() == -1) {
        QPoint southDirection = QPoint(0, -1);
        QPoint eastDirection = QPoint(-1, 0);
        appendVertices(southDirection);
        appendVertices(eastDirection);
    }
    updateVertices();


}

void World::updateVertices() {
    for (int x = 0; x < worldMapWidth; x++) {
        for (int y = 0; y < worldMapWidth; y++) {
            if (!worldMap[x][y]->isRendered && worldMap[x][y]->toBeRendered) {
                worldMap[x][y]->setSectorRendered();
                for (int j = 0; j < worldMap[x][y]->vertices.size(); j++) {
                    vertices.append(worldMap[x][y]->vertices[j]);
                    textures.append(worldMap[x][y]->textures[j]);
                }
            }
        }
    }
}

bool World::checkCurrentSector(QVector3D camera) {
    QPoint boundsDirection = QPoint(0,0);
    QPoint origin = currentSector->getOrigin();
    float xPosOffset = fabs(camera.x()) - origin.x();
    float zPosOffset = fabs(camera.z()) - origin.y();
    if (xPosOffset < 0) { xPosOffset = 0.0f; }
    if (zPosOffset < 0) { zPosOffset = 0.0f; }
    //qDebug("xPosOffset = %d zPosOffset = %d",xPosOffset, zPosOffset);
    if (zPosOffset <= 0 ) {
        boundsDirection.setY(-1);
    }
    if (zPosOffset >= size) {
        boundsDirection.setY(1);
    }
    if (xPosOffset <= 0) {
        boundsDirection.setX(-1);
    }
    if (xPosOffset >= size) {
        boundsDirection.setX(1);
    }
    if (boundsDirection.x() == 0 && boundsDirection.y() == 0) { return false; }
    switchCurrentSector(boundsDirection);
    return true;
}

void World::switchCurrentSector(QPoint direction) {
    qDebug("Switching sectors!!");
    int nextX = currentSectorIndex.x() + direction.x();
    int nextY = currentSectorIndex.y() + direction.y();

    if (nextX < 0) {
        nextX = worldMapWidth - 1;
    }
    if (nextY < 0) {
        nextY = worldMapWidth -1;
    }
    if (nextX + 1 >= worldMapWidth + 1) {
        nextX = 0;
    }
    if (nextY + 1 >= worldMapWidth + 1) {
        nextY = 0;
    }
    this->currentSector = worldMap[nextX][nextY];
    currentSectorIndex = QPoint(nextX,nextY);
    qDebug("set currentSector[%d][%d]", nextX, nextY);
    if (!currentSector->isRendered) {
        currentSector->setSectorToRender();
        updateVertices();
    }
}



