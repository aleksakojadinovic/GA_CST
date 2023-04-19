#ifndef CONSTANT_WORKSPACE_H
#define CONSTANT_WORKSPACE_H

#include "algoritambaza.h"

#define updateAnimation() AlgoritamBaza_updateCanvasAndBlock()
#define finishAnimation animacijaZavrsila();

#define NUM_RANDOM 20

#include <vector>
#include <QPoint>

class ConstantWorkSpace : public AlgoritamBaza
{
public:
    /* Konstruktor i destruktor klase */
    ConstantWorkSpace(QWidget *qw, int deltaT, const bool &naiive, std::string fileName, int numObjects);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *) const final;
    void pokreniNaivniAlgoritam() final {};
    void crtajNaivniAlgoritam(QPainter *) const final{};

    void pokreniAlgoritamGrubeSile() {};

private:
    std::vector<QPoint> polygon;
};

#endif
