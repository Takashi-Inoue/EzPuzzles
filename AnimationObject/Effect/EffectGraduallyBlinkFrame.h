#ifndef GRADUALLYBLINKFRAME_H
#define GRADUALLYBLINKFRAME_H

#include "AbstractEffect.h"
#include <QPair>
#include <QPointF>
#include <QVector>

namespace Effect {

class GraduallyBlinkFrame : public AbstractEffect
{
public:
    GraduallyBlinkFrame(const QColor &outerStart, const QColor &outerEnd, int totalFrameCount, bool isLoop);
    GraduallyBlinkFrame(int width, const QColor &outerStart, const QColor &outerEnd, const QColor &innerStart, const QColor &innerEnd, int totalFrameCount, bool isLoop);
    ~GraduallyBlinkFrame() = default;

    // AbstractEffect
    void draw(QPainter &painter, const QRectF &rect) override;

protected:
    typedef QPair<QPointF, QPointF> PointPair;

    void drawSingleLineFrame(QPainter &painter, const QRectF &rect) const;
    void drawGradationFrame(QPainter &painter, const QRectF &rect) const;
    QColor currentColor(const QColor &start, const QColor &end) const;
    QVector<QVector<QPointF>> createEdgesPolygon(const QRectF &rect) const;
    QVector<PointPair> createGradientsStartStop(const QRectF &rect) const;

    int width;
    QColor outerStartColor;
    QColor outerEndColor;
    QColor innerStartColor;
    QColor innerEndColor;
};

} // Effect

#endif // GRADUALLYBLINKFRAME_H
