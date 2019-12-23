#include "SolarSystem.hpp"
#include <fstream>
#include <cmath>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <QTextStream>
#include <QString>
#include <QFile>

SolarSystem::SolarSystem()
{}

SolarSystem::SolarSystem(const Star & star_, const vector & planets_)
    : star(star_)
{
    for (c_iterator i = planets_.begin(); i != planets_.end(); ++i) {
        planets.push_back(*i);
    }
}

void SolarSystem::setStar(const Star & star)
{
    this->star = star;
}

void SolarSystem::setPlanets(const vector & planets_)
{
    for (c_iterator i = planets_.begin(); i != planets_.end(); ++i) {
        planets.push_back(*i);
    }
}

Star SolarSystem::getStar() const
{
    return star;
}

std::vector<Planet> SolarSystem::getPlanets() const
{
    return planets;
}

void SolarSystem::planetsMovement(double time)
{
    for (iterator i = planets.begin(); i != planets.end(); ++i) {
        i->setPositionX(star.getPosition().getX() +
                i->getOrbit() * cos(i->getStartTime() + time * i->getSpeed()));
        i->setPositionY(star.getPosition().getY() +
                i->getOrbit() * sin(i->getStartTime() + time * i->getSpeed()));
    }

    if (checkCollision()) {
        throw std::runtime_error("Collision detected");
    };
}

std::string SolarSystem::toString() const
{
    std::string str = star.toString() + "\n";

    for (c_iterator i = planets.begin(); i != planets.end(); ++i) {
        str += "\n" + i->toString() + "\n";
    }

    return str;
}

void SolarSystem::output(const char * filename, bool append) const
{
    if (filename == nullptr) {
        throw std::invalid_argument("Empty filename!");
    }

    std::ofstream fout;

    if (append) {
        fout.open(filename, std::ios::out | std::ios::app);
    } else {
        fout.open(filename);
    }

    if (!fout) {
        throw std::runtime_error("[OUTPUT] File wasn't opened: " + std::string(filename));
    }

    fout << toString() << std::endl;
}

void SolarSystem::input(const char * filename)
{
    planets.clear();

    if (filename == nullptr) {
        throw std::invalid_argument("Empty filename!");
    }

    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        throw std::runtime_error("[INPUT] File wasn't opened: " + std::string(filename));
    };

    QTextStream fin(&file);

    // Firt record - is star
    QString name, color;
    double mass, rad, x, y, speed;

    fin >> name >> mass >> rad >> x >> y;
    if (fin.status() != QTextStream::Ok) {
        throw std::ios_base::failure("[INPUT] Failed to read star data");
    }
    fin.readLine(); // skip all leftover data in current line

    star = Star(name.toStdString(), mass, rad, Coordinates(x, y));

    // Then - planets
    while (!fin.atEnd()) {
        QString name = "";
        fin >> name >> color >> mass >> rad >> speed >> x >> y;

        if (fin.status() != QTextStream::Ok) {
            std::cerr << "Failed to read planet data. Stopping to read." << std::endl;
            break;
        }

        fin.readLine(); // skip all leftover data in current line

        if (name == "") {
            continue;
        }

        planets.push_back(Planet(name.toStdString(), color.toStdString(), mass, rad, speed, Coordinates(x, y)));
    }

    file.close();
    calc_start();

    std::sort(planets.begin(), planets.end(), [](Planet & lhs, Planet & rhs)
            {
                return (lhs.getOrbit() < rhs.getOrbit());
            });

    std::cout << "[INPUT] Loaded configuration:\n" << toString() << std::endl;
}

void SolarSystem::save(const char * filename) const
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)) {
        throw std::runtime_error("[SAVE STATE] File wasn't opened: " + std::string(filename));
    };

    QTextStream fout(&file);

    fout << star.getName().c_str() << " " << star.getMass() << " "
        << star.getRad() << " " << star.getPosition().getX()
        << " " << star.getPosition().getY() << "\n";

    for (c_iterator i = planets.begin(); i != planets.end(); ++i) {
        fout << i->getName().c_str() << " " << i->getColor().c_str()
            << " " << i->getMass() << " " << i->getRad()
            << " " << i->getSpeed() << " " << i->getPosition().getX()
            << " " << i->getPosition().getY() << "\n";
    }

    file.close();
}

size_t SolarSystem::getSize() const
{
    return planets.size();
}

void SolarSystem::calc_start()
{
    for (iterator i = planets.begin(); i != planets.end(); ++i) {
        i->setOrbit(std::sqrt(std::pow(i->getPosition().getX()
                        - star.getPosition().getX(), 2)
                    + std::pow(i->getPosition().getY()
                        - star.getPosition().getY(), 2)));

        i->setStartTime(std::atan(
                    (i->getPosition().getY() - star.getPosition().getY())
                    /
                    (i->getPosition().getX() - star.getPosition().getX())
                    ));

        if (i->getPosition().getX() - star.getPosition().getX() < 0) {
            i->setStartTime(3.14 + i->getStartTime());
        }
    }
}

bool SolarSystem::checkCollision() const
{
    double dist = std::sqrt(
            std::pow(star.getPosition().getX() - planets[0].getPosition().getX(), 2) +
            std::pow(star.getPosition().getY() - planets[0].getPosition().getY(), 2));

    if (dist < star.getRad() + planets[0].getRad()) {
        return true;
    }

    for (c_iterator i = planets.begin(); i != planets.end() - 1; ++i) {
        dist = std::sqrt(
                std::pow(i->getPosition().getX() - (i+1)->getPosition().getX(), 2) +
                std::pow(i->getPosition().getY() - (i+1)->getPosition().getY(), 2));

        if (dist < i->getRad() + (i + 1)->getRad()) {
            return true;
        }
    }
    return false;
}

void SolarSystem::clear()
{
    planets.clear();
}
