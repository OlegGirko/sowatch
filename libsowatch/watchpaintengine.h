#ifndef SOWATCH_WATCHPAINTENGINE_H
#define SOWATCH_WATCHPAINTENGINE_H

#include <QtGui/QPaintEngine>
#include "sowatch_global.h"

namespace sowatch
{

class SOWATCH_EXPORT WatchPaintEngine : public QPaintEngine
{
public:
	~WatchPaintEngine();

	/* You are supposed to override these two functions. */
	bool begin(QPaintDevice *pdev);
	bool end();

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
	WatchPaintEngine();

	void damageMappedRect(const QRect& r);
	void damageRect(const QRect& r);
	void damageRect(const QRectF& r);
	void damagePenStroke(const QLineF& line);
	void updateClipRegion(const QRegion& region, Qt::ClipOperation op);

	QPainter _painter;
	QRegion _damaged;
	QRect _area;

	bool _hasPen;
	qreal _penWidth;

	bool _hasBrush;

	bool _clipEnabled;
	QRegion _clipRegion;

	QTransform _transform;
};

}

#endif // SOWATCH_WATCHPAINTENGINE_H
