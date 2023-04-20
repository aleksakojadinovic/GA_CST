#ifndef CONSTANT_WORKSPACE_H
#define CONSTANT_WORKSPACE_H

#include "algoritambaza.h"

#define updateAnimation() AlgoritamBaza_updateCanvasAndBlock()
#define finishAnimation animacijaZavrsila();

#define NUM_RANDOM 20

#include <vector>
#include <QPoint>
#include <fstream>
#include <iostream>

class ConstantWorkSpace : public AlgoritamBaza
{
public:
    /* Konstruktor i destruktor klase */
    ConstantWorkSpace(QWidget *qw, int deltaT, const bool &naiive, std::string fileName, int numObjects);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *) const final;
    void pokreniNaivniAlgoritam() final{};
    void crtajNaivniAlgoritam(QPainter *) const final{};

    void pokreniAlgoritamGrubeSile(){};

    void readFromFile(std::string fileName)
    {
        std::ifstream inputFile(fileName);
        float x, y;
        while (inputFile >> x >> y)
        {
            std::cerr << "adding point" << std::endl;
            polygon.push_back(QPointF(x, y));
        };
        std::cerr << polygon.size() << std::endl;
    }

    void preparePoints();

private:
    std::vector<QPointF> polygon;
};

#endif
