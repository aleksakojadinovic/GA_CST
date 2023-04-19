#include "constant_work_space.h"
#include <iostream>

ConstantWorkSpace::ConstantWorkSpace(QWidget *qw,
                                     int deltaT,
                                     const bool &naiive,
                                     std::string fileName = "",
                                     int numObjects = NUM_RANDOM)
    : AlgoritamBaza(qw, deltaT, naiive)
{
    if (fileName == "")
    {
    }
    else
    {
        readFromFile(fileName);
    }
}

void ConstantWorkSpace::pokreniAlgoritam()
{
    updateAnimation();
    emit finishAnimation;
}

void ConstantWorkSpace::crtajAlgoritam(QPainter *painter) const
{
    const auto N = polygon.size();
    std::cerr << "asdasdsa" << std::endl;
    for (int i = 0; i < N; i++)
    {
        const auto p1 = polygon[i];
        const auto p2 = polygon[(i + 1) % N];

        std::cerr << p1.x() << std::endl;

        painter->drawPoint(p1);
        painter->drawPoint(p2);
        painter->drawLine(p1, p2);
    }
}
