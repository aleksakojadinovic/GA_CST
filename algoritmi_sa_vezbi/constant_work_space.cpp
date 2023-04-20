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

int ConstantWorkSpace::getLeftMostPointIndex(int fromIndex)
{
    // TODO: remove this
    return 0;
}

void ConstantWorkSpace::pokreniAlgoritam()
{
    updateAnimation();

    QPointF qiminus1;

    for (auto i = 0; i < polygon.size(); i++)
    {
        QPointF qi;

        if (i == 0)
        {
            // qi is the leftmost point in the set
            qi = *std::min(std::begin(polygon), std::end(polygon), [](auto p1, auto p2)
                           { return (*p1).x() < (*p2).x(); });
        }
        else
        {
            // qi is the leftmost point that is to the right of qiminus1
            qi = polygon[0];
            for (auto p : polygon)
            {
                if (p.x() <= qiminus1.x())
                {
                    continue;
                }
                if (p.x() < qi.x())
                {
                    qi = p;
                }
            }
        }

        qidisplay = QPointF(qi.x(), qi.y());

        qiminus1 = qi;
        AlgoritamBaza_updateCanvasAndBlock();
    }

    emit finishAnimation;
}

void ConstantWorkSpace::crtajAlgoritam(QPainter *painter) const
{
    const auto N = polygon.size();
    auto pen = painter->pen();
    


    // Draw the polygon
    for (int i = 0; i < N; i++)
    {
        const auto p1 = polygon[i];
        const auto p2 = polygon[(i + 1) % N];
        
        pen.setColor(Qt::black);
        pen.setWidth(5);
        painter->setPen(pen);
    
        painter->drawLine(p1, p2);

        pen.setColor(Qt::magenta);
        pen.setWidth(10);
        painter->setPen(pen);
        
        painter->drawPoint(p1);
        painter->drawPoint(p2);
        
    }

    // Draw current qi
    std::cerr << "qidisplay" << qidisplay.x() << "," << qidisplay.y() << std::endl;
    pen.setColor(Qt::red);
    pen.setWidth(15);

    painter->setPen(pen);
    painter->drawPoint(qidisplay);
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
        }
        else
        {
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
