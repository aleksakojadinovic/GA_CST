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
    // for (auto i = 0; i < polygon.size(); i++)
    // {
    //     auto p = polygon[i];
    //     std::cerr << i << " --> " << p.x() << ", " << p.y() << std::endl;
    // }
    updateAnimation();

    QPointF qi_minus_1;

    auto N = polygon.size();

    for (auto i = 0; i < polygon.size(); i++)
    {
        QPointF qi;
        int qi_index;

        if (i == 0)
        {
            // qi is the leftmost point in the set
            qi = polygon[0];
            qi_index = 0;
            for (auto j = 0; j < polygon.size(); j++)
            {
                auto p = polygon[j];
                if (p.x() < qi.x())
                {
                    qi = p;
                    qi_index = j;
                }
            }
        }
        else
        {
            // qi is the leftmost point that is to the right of qiminus1
            qi = polygon[0];
            qi_index = 0;
            for (auto j = 0; j < polygon.size(); j++)
            {
                auto p = polygon[j];
                if (p.x() <= qi_minus_1.x())
                {
                    continue;
                }
                if (p.x() < qi.x())
                {
                    qi = p;
                    qi_index = j;
                }
            }
        }

        qi_display = QPointF(qi.x(), qi.y());
        // AlgoritamBaza_updateCanvasAndBlock();

        // At each vertex qi
        // , we check if there
        // is a trapezoid to the right of qi
        // incident to qi
        // . This happens precisely if at least one of
        // the two edges incident to qi has an endpoint to the right of qi
        // .

        // First we need to find the edges of the point qi. But since this is a simple polygon those will be one before and one after qi
        auto before_index = (qi_index - 1) % N;
        auto after_index = (qi_index + 1) % N;
        auto qi_before = polygon[before_index];
        auto qi_after = polygon[after_index];

        // std::cerr << "qi is " << qi.x() << " , " << qi.y() << ", at index" << qi_index << " and its edges go to " << qi_before.x() << ", " << qi_before.y() << " and " << qi_after.x() << ", " << qi_after.y() << " which are at indices " << before_index << " and " << after_index << std::endl;

        qi_edges_display = std::pair<QPointF, QPointF>(qi_before, qi_after);
        AlgoritamBaza_updateCanvasAndBlock();


        // . This happens precisely if at least one of
        // the two edges incident to qi has an endpoint to the right of qi
        // Meaning either qi_before and qi_after is to the right of qi

        auto hasTrapezoidToTheRight = qi_before.x() > qi.x() || qi_after.x() > qi.x();

        if (!hasTrapezoidToTheRight){
            // Early return
            qi_minus_1 = qi;
            continue;
        }


        
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

        auto p = polygon[i];

        pen.setColor(Qt::black);
        pen.setWidth(5);
        painter->setPen(pen);
        QRectF rect(QPointF(p.x() + 5, p.y() + 5), QPointF(p.x() - 20, p.y() - 20));
        rect = rect.normalized();
        painter->drawText(rect, QString::number(i));

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
    pen.setColor(Qt::red);
    pen.setWidth(15);
    painter->setPen(pen);
    painter->drawPoint(qi_display);

    // Draw helper edges

    pen.setWidth(5);

    pen.setColor(Qt::yellow);
    painter->setPen(pen);
    painter->drawLine(qi_display, qi_edges_display.first);

    pen.setColor(Qt::cyan);
    painter->setPen(pen);
    painter->drawLine(qi_display, qi_edges_display.second);
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
    }

    std::cerr << "max point" << max_point.x() << "," << max_point.y() << std::endl;

    std::sort(polygon.begin(), polygon.end(), [&](const auto &lhs, const auto &rhs)
              {
        float P = pomocneFunkcije::povrsinaTrouglaF(max_point, lhs, rhs);
        return  (P < 0) ||  (P == 0 && pomocneFunkcije::distanceKvadratF(max_point, lhs)
                             < pomocneFunkcije::distanceKvadratF(max_point, rhs)); });
}
