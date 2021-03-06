#ifndef STAR_HPP
#define STAR_HPP

#include "SpaceObject.hpp"

class Star : public SpaceObject
{
    public:
        Star();
        Star(const std::string & name, double mass,
             double rad, const Coordinates & position);
        std::string toString() const;
};

#endif
