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

        QLineF ray = QLineF(QPointF(qi.x(), qi.y() - 1000), QPointF(qi.x(), qi.y() + 200));

        bool ea_initialized = false;
        bool eb_initialized = false;
        QLineF eA;
        QLineF eB;
        QPointF eA_intersect_point;
        QPointF eB_intersect_point;

        for (auto i = 0; i < polygon.size(); i++)
        {
            auto p1_index = i;
            auto p2_index = (i + 1) % N;
            auto p1 = polygon[p1_index];
            auto p2 = polygon[p2_index];

            auto edge_left_x = std::min(p1.x(), p2.x());
            auto edge_right_x = std::max(p1.x(), p2.x());
            auto edge_down_y = std::min(p1.y(), p2.y());
            auto edge_up_y = std::max(p1.y(), p2.y());

            auto eA_up_y = std::max(eA.p1().y(), eA.p2().y());
            auto eB_down_y = std::min(eB.p1().y(), eB.p2().y());

            auto has_endpoint_to_the_right = edge_right_x > qi.x();

            auto has_endpoint_to_the_up = edge_up_y > qi.y();
            auto has_endpoint_to_the_down = edge_down_y < qi.y();

            auto candidate_edge = QLineF(p1, p2);

            QPointF intersection_point;
            auto intersection_indicator = ray.intersect(candidate_edge, &intersection_point);

            auto has_intersection = intersection_indicator != QLineF::IntersectType::NoIntersection && edge_left_x <= intersection_point.x() && intersection_point.x() <= edge_right_x;

            float intersect_y = intersection_point.y();

            auto should_consider_for_ea = has_intersection && intersect_y >= qi.y() && has_endpoint_to_the_right && has_endpoint_to_the_up;
            auto should_consider_for_eb = has_intersection && intersect_y <= qi.y() && has_endpoint_to_the_right && has_endpoint_to_the_down;

            //  Handle initial cases (bcz im lazy as fukkkkk)
            if (should_consider_for_ea)
            {

                if (intersection_point.y() < eA_intersect_point.y() || (intersection_point.y() == eA_intersect_point.y() && edge_up_y < eA_up_y || !ea_initialized))
                {
                    ea_initialized = true;
                    eA_intersect_point = QPointF(intersection_point);
                    eA = candidate_edge;
                }
            }

            if (should_consider_for_eb)
            {
                if (intersection_point.y() > eB_intersect_point.y() || (intersection_point.y() == eB_intersect_point.y() && edge_down_y > eB_down_y || !eb_initialized))
                {
                    eb_initialized = true;
                    eB_intersect_point = QPointF(intersection_point);
                    eB = candidate_edge;
                }
            }
        }

        if (ea_initialized && eb_initialized)
        {
            auto ea_right_point = eA.p1().x() > eA.p2().x() ? eA.p1() : eA.p2();
            auto eb_right_point = eB.p1().x() > eB.p2().x() ? eB.p1() : eB.p2();

            auto initial_r = ea_right_point.x() < eb_right_point.x() ? ea_right_point : eb_right_point;

            auto initial_trapezoid = resolveTrapezoidPoints(qi, eA, eB, initial_r);

            bool replacement_initialized = false;
            QPointF replacement;

            for (auto inside_point: polygon) {
                if (!initial_trapezoid.containsPoint(inside_point, Qt::OddEvenFill)) {
                    continue;
                }

                if (!replacement_initialized || inside_point.x() < replacement.x()) {
                    replacement_initialized = true;
                    replacement = QPointF(inside_point);
                }
            }
            auto new_r = replacement_initialized ? replacement : initial_r;
            auto final_trapezoid = resolveTrapezoidPoints(qi, eA, eB, new_r);

            trapezoids.push_back(final_trapezoid);

        }

        eA_display = eA;
        eB_display = eB;
        qi_ray_display = ray;
        qi_display = qi;
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