#include "EffectGraduallyBlinkFrame.h"

namespace Effect {

GraduallyBlinkFrame::GraduallyBlinkFrame(const QColor &outerStart, const QColor &outerEnd, int totalFrameCount, bool isLoop) :
    AbstractEffect(totalFrameCount, isLoop),
    width(1),
    outerStartColor(outerStart),
    outerEndColor(outerEnd),
    innerStartColor(outerStart),
    innerEndColor(outerEnd)
{
}

GraduallyBlinkFrame::GraduallyBlinkFrame(int width, const QColor &outerStart, const QColor &innerStart, const QColor &outerEnd, const QColor &innerEnd,
                                         int totalFrameCount, bool isLoop) :
    AbstractEffect(totalFrameCount, isLoop),
    width(width),
    outerStartColor(outerStart),
    outerEndColor(outerEnd),
    innerStartColor(innerStart),
    innerEndColor(innerEnd)
{
    Q_ASSERT(width > 0);
}

void GraduallyBlinkFrame::draw(QPainter &painter, const QRectF &rect)
{
    width == 1 ? drawSingleLineFrame(painter, rect)
               : drawGradationFrame(painter, rect);
}

void GraduallyBlinkFrame::drawSingleLineFrame(QPainter &painter, const QRectF &rect) const
{
    painter.save();

    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing, false);

    QColor color = currentColor(outerStartColor, outerEndColor);

    painter.setPen(QPen(color, 1.0));
    painter.drawRect(rect.marginsRemoved(QMarginsF(0, 0, 1, 1)));

    painter.restore();
}

void GraduallyBlinkFrame::drawGradationFrame(QPainter &painter, const QRectF &rect) const
{
    painter.save();

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::transparent);

    QLinearGradient gradient(rect.topLeft(), rect.topLeft() + QPointF(width, 0));

    QColor outer = currentColor(outerStartColor, outerEndColor);
    QColor inner = currentColor(innerStartColor, innerEndColor);

    gradient.setColorAt(0, outer);
    gradient.setColorAt(1, inner);

    auto edgesPolygon = createEdgesPolygon(rect);             // left - top - right - bottom
    auto gradientsStartStop = createGradientsStartStop(rect); // left - top - right - bottom

    for (int i = 0; i < 4; ++i) {
        const PointPair &startStop = gradientsStartStop.at(i);

        gradient.setStart(startStop.first);
        gradient.setFinalStop(startStop.second);

        painter.setBrush(gradient);
        painter.drawPolygon(edgesPolygon.at(i));
    }

    painter.restore();
}

QColor GraduallyBlinkFrame::currentColor(const QColor &start, const QColor &end) const
{
    if (totalFrames() == 0)
        return start;

    double pct = (2 * nowFrame()) / totalFrames();

    if (pct > 1.0)
        pct = 2 - pct;

    QColor color;

    color.setRedF  ((end.redF()   - start.redF())   * pct + start.redF());
    color.setGreenF((end.greenF() - start.greenF()) * pct + start.greenF());
    color.setBlueF ((end.blueF()  - start.blueF())  * pct + start.blueF());
    color.setAlphaF((end.alphaF() - start.alphaF()) * pct + start.alphaF());

    return color;
}

QVector<QVector<QPointF>> GraduallyBlinkFrame::createEdgesPolygon(const QRectF &rect) const
{
    // left - top - right - bottom
    return {
        {
            rect.topLeft(),
            rect.topLeft()    + QPointF(width,  width),
            rect.bottomLeft() + QPointF(width, -width),
            rect.bottomLeft(),
        },
        {
            rect.topLeft(),
            rect.topLeft()  + QPointF( width, width),
            rect.topRight() + QPointF(-width, width),
            rect.topRight(),
        },
        {
            rect.topRight(),
            rect.topRight()    + QPointF(-width,  width),
            rect.bottomRight() + QPointF(-width, -width),
            rect.bottomRight(),
        },
        {
            rect.bottomRight(),
            rect.bottomRight() + QPointF(-width, -width),
            rect.bottomLeft()  + QPointF( width, -width),
            rect.bottomLeft(),
        },
    };
}

QVector<GraduallyBlinkFrame::PointPair> GraduallyBlinkFrame::createGradientsStartStop(const QRectF &rect) const
{
    // left - top - right - bottom
    return {
        PointPair(rect.topLeft(),     rect.topLeft()     + QPointF(width, 0)),
        PointPair(rect.topLeft(),     rect.topLeft()     + QPointF(0, width)),
        PointPair(rect.bottomRight(), rect.bottomRight() + QPointF(-width, 0)),
        PointPair(rect.bottomRight(), rect.bottomRight() + QPointF(0, -width)),
    };
}

} // Effect
