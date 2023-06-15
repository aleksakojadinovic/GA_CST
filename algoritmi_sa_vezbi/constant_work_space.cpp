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
    for (int i = 0; i < 20; i++)
    {
        QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
        colors.push_back(color);
    }
}

int ConstantWorkSpace::getLeftMostPointIndex(int fromIndex)
{
    // TODO: remove this
    return 0;
}

QPolygonF ConstantWorkSpace::resolveTrapezoidPoints(QPointF qi, QLineF eA, QLineF eB, QPointF r)
{
    // Find intersects
    QPointF intersect1;
    QPointF intersect2;

    QLineF ray(QPointF(qi.x(), qi.y() + 2000), QPointF(qi.x(), qi.y() - 2000));
    ray.intersect(eA, &intersect1);
    ray.intersect(eB, &intersect2);
    auto t_vector = QVector<QPointF>();
    t_vector.push_back(QPointF(intersect1));
    t_vector.push_back(QPointF(intersect2));

    ray = QLineF(QPointF(r.x(), r.y() + 2000), QPointF(r.x(), r.y() - 2000));
    ray.intersect(eA, &intersect1);
    ray.intersect(eB, &intersect2);
    t_vector.push_back(QPointF(intersect2));
    t_vector.push_back(QPointF(intersect1));

    return QPolygonF(t_vector);
}

bool ConstantWorkSpace::isInteriorBelowLine(QLineF line)
{
    // This will be true if
}

bool ConstantWorkSpace::getBindingEdges(const std::vector<QPointF> &polygon, const QPointF &point, QLineF *upperEdge, QLineF *lowerEdge)
{
    bool upperFound = false;
    bool lowerFound = false;
    qreal upperMinDist = std::numeric_limits<qreal>::max();
    qreal lowerMinDist = std::numeric_limits<qreal>::max();
    QPointF intersection;

    for (size_t i = 0; i < polygon.size(); ++i)
    {
        QPointF p1 = polygon[i];
        QPointF p2 = polygon[(i + 1) % polygon.size()];

        // Create a line
        QLineF line(p1, p2);


        std::cerr << "Line: " << p1.x() << "," << p1.y() << " --> " << p2.x() << ", " << p2.y() << std::endl;

        // auto intersectionResult = QLineF(point, QPointF(point.x(), -10000)).intersect(line, &intersection);
        if (QLineF(point, QPointF(point.x(), -10000)).intersect(line, &intersection) == QLineF::BoundedIntersection)
        {
            std::cerr << "ive intersected something on lower" << std::endl;
            qreal dist = point.y() - p1.y();
            if (intersection.y() <= point.y() && (!lowerFound || dist < lowerMinDist))
            {
                *lowerEdge = line;
                lowerMinDist = dist;
                lowerFound = true;
            }
        }

        if (QLineF(point, QPointF(point.x(), 10000)).intersect(line, &intersection) == QLineF::BoundedIntersection)
        {
            std::cerr << "ive intersected something on upper" << std::endl;
            qreal dist = p1.y() - point.y();
            if (intersection.y() > point.y() && (!upperFound || dist < upperMinDist))
            {
                *upperEdge = line;
                upperMinDist = dist;
                upperFound = true;
            }
        }
    }

    return upperFound && lowerFound;
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

    for (auto p_index = 0; p_index < polygon.size(); p_index++)
    {
        QPointF qi;
        int qi_index;

        if (p_index == 0)
        {
            // qi is the leftmost point in the set (which is the first because that's how we sorted them)
            qi = polygon[0];
            qi_index = 0;
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
                if (p.x() < qi.x() || qi_index == 0)
                {
                    qi = p;
                    qi_index = j;
                }
            }
        }

        QLineF eA;
        QLineF eB;

        auto edgesFound = getBindingEdges(polygon, qi, &eA, &eB);

        if (edgesFound)
        {
            ea_initialized_display = true;
            eb_initialized_display = true;
            eA_display = eA;
            eB_display = eB;
        }else{
            ea_initialized_display = false;
            ea_initialized_display = false;
            std::cerr << "Not found :(" << std::endl;
        }

        // qi_ray_display = ray;

        qi_display = qi;
        qi_ray_display = QLineF(QPointF(qi.x(), -1000), QPointF(qi.x(), 100000));
        AlgoritamBaza_updateCanvasAndBlock();

        qi_minus_1 = qi;
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
        auto textOption = QTextOption();
        textOption.setTextDirection(Qt::LayoutDirection::LeftToRight);

        painter->drawText(rect, QString::number(i), textOption);

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
    // pen.setWidth(5);
    // pen.setColor(Qt::yellow);
    // painter->setPen(pen);
    // painter->drawLine(qi_display, qi_edges_display.first);

    // pen.setColor(Qt::cyan);
    // painter->setPen(pen);
    // painter->drawLine(qi_display, qi_edges_display.second);

    // Draw ray from qi
    pen.setColor(Qt::black);
    pen.setWidth(5);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->drawLine(qi_ray_display);

    // Draw eA

    // std::cerr << "drawing ea from " << ptos(eA.p1()) << " to " << ptos(eA.p2()) << std::endl;
    pen.setColor(Qt::yellow);
    pen.setWidth(5);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawLine(eA_display);

    // Draw eB
    if (eb_initialized_display)
    {
        pen.setColor(Qt::cyan);
        pen.setWidth(5);
        pen.setStyle(Qt::SolidLine);
        painter->setPen(pen);
        painter->drawLine(eB_display);
    }

    // Draw replacement point
    if (replacement_display_visible)
    {
        std::cerr << "no replacement here" << std::endl;
        pen.setColor(Qt::gray);
        pen.setWidth(15);
        painter->setPen(pen);
        painter->drawPoint(replacement_display);
    }

    // Draw trapezoids
    for (auto i = 0; i < trapezoids.size(); i++)
    {
        auto t = trapezoids[i];

        auto color = colors[i & 20];
        QBrush b(QColor(color.red(), color.green(), color.blue(), 122));
        pen.setColor(Qt::black);
        pen.setWidth(5);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->setBrush(b);
        painter->drawPolygon(t);
    }
}

void ConstantWorkSpace::preparePoints()
{
    auto min_point = polygon[0];

    std::cerr << polygon.size() << std::endl;

    for (auto i = 1ul; i < polygon.size(); i++)
    {
        if (polygon[i].x() < min_point.x() || (polygon[i].x() == min_point.x() && polygon[i].y() > min_point.y()))
        {
            min_point = polygon[i];
        }
    }

    std::sort(polygon.begin(), polygon.end(), [&](const auto &lhs, const auto &rhs)
              {
        float P = pomocneFunkcije::povrsinaTrouglaF(min_point, lhs, rhs);
        return  (P > 0) ||  (P == 0 && pomocneFunkcije::distanceKvadratF(min_point, lhs)
                             < pomocneFunkcije::distanceKvadratF(min_point, rhs)); });
}

bool ConstantWorkSpace::findEdgeAbove(int pointIndex, int *result)
{
    QPointF q = polygon[pointIndex];

    QLineF ray = QLineF(QPointF(q.x(), q.y() - 1000), QPointF(q.x(), q.y() + 200));

    std::cerr << "Finding edge above for point p" << pointIndex << " = " << ptos(q) << std::endl;
}
std::string ConstantWorkSpace::ptos(QPointF p)
{
    return std::to_string(p.x()) + ", " + std::to_string(p.y());
}