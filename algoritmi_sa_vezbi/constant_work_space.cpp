#include "constant_work_space.h"

ConstantWorkSpace::ConstantWorkSpace(QWidget *qw,
                                     int deltaT,
                                     const bool &naiive,
                                     std::string fileName="",
                                     int numObjects=NUM_RANDOM)
    : AlgoritamBaza(qw, deltaT, naiive)
{
    if (fileName == "")
    {
        polygon = generisiNasumicneTacke(numObjects);
    }

    // generisiNasumicnePravougaonike(brojPravougaonika);
    else
    {
        ucitajPodatkeIzDatoteke(fileName);
    };
}

void ConstantWorkSpace::pokreniAlgoritam()
{
    updateAnimation();
    emit finishAnimation;
}

void ConstantWorkSpace::crtajAlgoritam(QPainter *painter) const
{
    const auto N = polygon.size();
    for (int i = 0; i < N; i++)
    {
        const auto p1 = polygon[i];
        const auto p2 = polygon[(i + 1) % N];

        painter->drawPoint(p1);
        painter->drawPoint(p2);
        painter->drawLine(p1, p2);
    }
}
