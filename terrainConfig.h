#ifndef TERRAINCONFIG_H
#define TERRAINCONFIG_H

class terrainConfig {
public:
    terrainConfig() {}
    terrainConfig(int range, int roughness) : range(range), roughness(roughness) {}
    int range;
    int roughness;
    terrainConfig operator = (const terrainConfig & rhs)
    {
        this->roughness = rhs.roughness;
        this->range = rhs.range;
        return *this;
    }

};

static const terrainConfig mountains = terrainConfig(20, 20);
static const terrainConfig morrasse  = terrainConfig(15,15);
static const terrainConfig hills     = terrainConfig(10,10);
static const terrainConfig scrub     = terrainConfig(5,10);
static const terrainConfig plains    = terrainConfig(5,15);
static const terrainConfig flat      = terrainConfig(1,1);

#endif // TERRAINCONFIG_H
