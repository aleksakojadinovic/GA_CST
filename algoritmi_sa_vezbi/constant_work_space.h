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

#include <optional>

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

    int getLeftMostPointIndex(int fromIndex);

    void readFromFile(std::string fileName)
    {
        std::ifstream inputFile(fileName);
        float x, y;
        while (inputFile >> x >> y)
        {
            polygon.push_back(QPointF(x, y));
        };
    }

    void preparePoints();

    bool findEdgeAbove(int pointIndex, int *result);

    QPolygonF resolveTrapezoidPoints(QPointF, QLineF, QLineF, QPointF);
    bool isInteriorBelowLine(QLineF);

    std::string ptos(QPointF p);
    bool getBindingEdges(const std::vector<QPointF> &polygon, const QPointF &point, QLineF *upperEdge, QLineF *lowerEdge);

    void find_biding_edge(int qi_index, bool * is_found, QLineF * edge, bool is_upper, int * edge_start_index);

private:
    std::vector<QPointF> polygon;
    std::vector<QPolygonF> trapezoids;

    std::vector<QColor> colors;

    QPointF replacement_display;
    bool replacement_display_visible;

    bool ea_initialized_display;
    bool eb_initialized_display;

    QPointF qi_display;
    std::pair<QPointF, QPointF> qi_edges_display;
    QLineF qi_ray_display;
    QLineF eA_display;
    QLineF eB_display;
};

#endif
