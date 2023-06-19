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
    colors.push_back(Qt::red);
    colors.push_back(Qt::green);
    colors.push_back(Qt::blue);
    // for (int i = 0; i < 20; i++)
    // {
    //     QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
    //     colors.push_back(color);
    // }
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

void ConstantWorkSpace::find_biding_edge(int qi_index, bool *is_found, QLineF *edge, bool is_upper, int *edge_start_index)
{

    float best_intersection_y = is_upper ? -100000 : 100000;
    bool best_is_incident;
    QPointF point = polygon[qi_index];

    *is_found = false;

    QLineF ray(point, QPointF(point.x(), is_upper ? 10000 : -10000));

    for (int i = 0; i < polygon.size(); i++)
    {
        auto edge_p1 = polygon[i];
        auto edge_p2 = polygon[(i + 1) % polygon.size()];

        auto candidate_edge = QLineF(edge_p1, edge_p2);

        // Check for intersection

        QPointF intersection;

        auto intersection_result = ray.intersect(candidate_edge, &intersection);

        if (intersection_result != QLineF::IntersectType::BoundedIntersection)
        {
            continue;
        }

        // handle incident edge
        auto edge_max_right = std::max(edge_p1.x(), edge_p2.x());
        if (edge_max_right <= point.x())
        {
            continue;
        }

        // Not considering vertical edges
        if (candidate_edge.dx() == 0)
        {
            continue;
        }

        auto is_incident = qi_index == i || qi_index == (i + 1) % polygon.size();

        if (is_incident)
        {
            // handle incident edge

            // 1) edge needs to have an endpoint to the right of qi


            // 2) Does the interior of the polygon lie below/above the line
            // The interior is below iff the next point ccw order is below the edge

            // THEORY: Interior lies above if and only if cross product of edge and its perpendicular vector is > 0

            

            bool can_be_upper_edge = false;
            bool can_be_lower_edge = false;

            auto goes_up = edge_p2.y() > edge_p1.y();
            auto goes_down = edge_p2.y() < edge_p1.y();

            


            auto is_horizontal = edge_p1.y() == edge_p2.y();

            if (is_horizontal) {
                auto goes_left = edge_p2.x() > edge_p1.x();
                auto goes_right = edge_p2.x() < edge_p1.x();

                if (is_upper && !goes_left) {
                    continue;
                }

                if (!is_upper & !goes_right) {
                    continue;
                }
            }else{

                auto slope = candidate_edge.dy() / candidate_edge.dx();
                can_be_upper_edge = (slope < 0 && goes_up) || (slope > 0 && goes_down);
                can_be_lower_edge = (slope < 0 && goes_down) || (slope > 0 && goes_up);

                if (is_upper && !can_be_upper_edge) {
                    continue;
                }

                if (!is_upper && !can_be_lower_edge) {
                    continue;
                }

            }

            

            // Doesn't work
            // if (is_upper && (candidate_edge.dy() / candidate_edge.dx()) < 0)
            // {
            //     continue;
            // }

            // if (!is_upper && (candidate_edge.dy() / candidate_edge.dx()) > 0)
            // {
            //     continue;
            // }
            // auto qi_next_index = (qi_index + 1) % polygon.size();

            // auto o = orientation(edge_p1, edge_p2, polygon[qi_next_index]);

            // if (is_upper && orientation < 0) {
            //     continue;
            // }
            // if (!is_upper && orientation > 0) {
            //     continue;
            // }
        }

        // Now we know this point should be considered

        if (!(*is_found) || (best_is_incident && !is_incident) || ((is_upper && intersection.y() < best_intersection_y) || (!is_upper && intersection.y() > best_intersection_y)))
        {
            *is_found = true;
            *edge = candidate_edge;
            best_intersection_y = intersection.y();
            *edge_start_index = i;
            best_is_incident = is_incident;
        }
    }
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

        QLineF ray = QLineF(QPointF(qi.x(), qi.y() - 10000), QPointF(qi.x(), qi.y() + 10000));

        bool ea_initialized = false;
        bool eb_initialized = false;
        QLineF eA;
        QLineF eB;
        QPointF eA_intersect_point;
        QPointF eB_intersect_point;

        int eA_start_index;
        int eB_start_index;

        find_biding_edge(qi_index, &ea_initialized, &eA, true, &eA_start_index);
        find_biding_edge(qi_index, &eb_initialized, &eB, false, &eB_start_index);

        if (ea_initialized && eb_initialized)
        {
            auto ea_right_point = eA.p1().x() > eA.p2().x() ? eA.p1() : eA.p2();
            auto eb_right_point = eB.p1().x() > eB.p2().x() ? eB.p1() : eB.p2();

            auto initial_r = ea_right_point.x() < eb_right_point.x() ? ea_right_point : eb_right_point;
            auto initial_r_index = ea_right_point.x() < eb_right_point.x() ? eA_start_index : eB_start_index;

            auto initial_trapezoid = resolveTrapezoidPoints(qi, eA, eB, initial_r);

            bool replacement_initialized = false;
            QPointF replacement;
            int replacement_index = initial_r_index;

            for (int k = 0; k < polygon.size(); k++)
            {
                auto inside_point = polygon[k];
                if (k == qi_index)
                {
                    continue;
                }

                if (!initial_trapezoid.containsPoint(inside_point, Qt::OddEvenFill))
                {
                    continue;
                }

                if (!replacement_initialized || inside_point.x() < replacement.x())
                {
                    replacement_initialized = true;
                    replacement_index = k;
                    replacement = QPointF(inside_point);
                }
            }
            auto new_r = replacement_initialized ? replacement : initial_r;
            auto final_trapezoid = resolveTrapezoidPoints(qi, eA, eB, new_r);

            std::cerr << "for qi_index=" << qi_index << " we have trapezoid with ea= " << eA_start_index << " eb = " << eB_start_index << " and r = " << replacement_index << std::endl;

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

    

    // std::cerr << "drawing ea from " << ptos(eA.p1()) << " to " << ptos(eA.p2()) << std::endl;
    if (ea_initialized_display && eb_initialized_display)
    {
        // Draw eA
        pen.setColor(Qt::yellow);
        pen.setWidth(5);
        pen.setStyle(Qt::SolidLine);
        painter->setPen(pen);
        painter->drawLine(eA_display);


        // Draw eB
        pen.setColor(Qt::cyan);
        pen.setWidth(5);
        pen.setStyle(Qt::SolidLine);
        painter->setPen(pen);
        painter->drawLine(eB_display);
    }


    // Draw trapezoids
    for (auto i = 0; i < trapezoids.size(); i++)
    {
        auto t = trapezoids[i];

        auto color = colors[i & colors.size()];
        QBrush b(QColor(color.red(), color.green(), color.blue(), 122));
        pen.setColor(Qt::black);
        pen.setWidth(5);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->setBrush(b);
        painter->drawPolygon(t);
    }



    // Draw replacement point
    if (replacement_display_visible)
    {
        // std::cerr << "no replacement here" << std::endl;
        pen.setColor(Qt::gray);
        pen.setWidth(15);
        painter->setPen(pen);
        painter->drawPoint(replacement_display);
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