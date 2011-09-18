#ifndef WATCHPAINTENGINE_H
#define WATCHPAINTENGINE_H

#include <QtGui/QPaintEngine>

namespace sowatch
{

class Watch;

class WatchPaintEngine : public QPaintEngine
{
public:
	WatchPaintEngine(Watch* watch);

	void drawEllipse(const QRectF &r);
	void drawEllipse(const QRect &r);
	void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr, Qt::ImageConversionFlags flags);
	void drawLines(const QLineF *lines, int lineCount);
	void drawLines(const QLine *lines, int lineCount);
	void drawPath(const QPainterPath &path);
	void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
	void drawPoints(const QPointF *points, int pointCount);
	void drawPoints(const QPoint *points, int pointCount);
	void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
	void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode);
	void drawRects(const QRectF *rects, int rectCount);
	void drawRects(const QRect *rects, int rectCount);
	void drawTextItem(const QPointF &p, const QTextItem &textItem);
	void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s);

	Type type() const;
	void updateState(const QPaintEngineState &state);

protected:
	void damageMappedRect(const QRect& r);
	void damageRect(const QRect& r);
	void damageRect(const QRectF& r);
	void damagePenStroke(const QLineF& line);
	void updateClipRegion(const QRegion& region, Qt::ClipOperation op);

	Watch* _watch;
	QPainter _painter;
	QRegion _damaged;

	bool _hasPen;
	qreal _penWidth;

	bool _hasBrush;

	bool _clipEnabled;
	QRegion _clipRegion;

	QTransform _transform;
};

}

#endif // WATCHPAINTENGINE_H
