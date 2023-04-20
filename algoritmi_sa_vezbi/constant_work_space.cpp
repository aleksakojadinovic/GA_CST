#include "constant_work_space.h"
#include <iostream>
#include <limits>
#include "pomocnefunkcije.h"

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
    preparePoints();
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

void ConstantWorkSpace::preparePoints()
{
    auto max_point = polygon[0];

    std::cerr << polygon.size() << std::endl;

    for (auto i = 1ul; i < polygon.size(); i++)
    {
        if (polygon[i].x() > max_point.x() || (polygon[i].x() == max_point.x() && polygon[i].y() < max_point.y()))
        {
            max_point = polygon[i];
        }else{
            std::cerr << polygon[i].x() << "is not better than" << max_point.x() << std::endl;
        }
    }

    std::cerr << "max point" << max_point.x() << "," << max_point.y() << std::endl;

    std::sort(polygon.begin(), polygon.end(), [&](const auto &lhs, const auto &rhs)
              {
        float P = pomocneFunkcije::povrsinaTrouglaF(max_point, lhs, rhs);
        return  (P < 0) ||  (P == 0 && pomocneFunkcije::distanceKvadratF(max_point, lhs)
                             < pomocneFunkcije::distanceKvadratF(max_point, rhs)); });
}
