#include <QtCore/QDebug>
#include <math.h>

#include "watchpaintengine.h"

using namespace sowatch;

#define TRACE(x)
//#define TRACE(x) x

WatchPaintEngine::WatchPaintEngine()
	: QPaintEngine(QPaintEngine::AllFeatures),
	  _painter()
{

}

WatchPaintEngine::~WatchPaintEngine()
{

}

bool WatchPaintEngine::begin(QPaintDevice *pdev)
{
	_damaged = QRegion();
	_area = QRect(0, 0, pdev->width(), pdev->height());
	_hasPen = false;
	_penWidth = 0.0;
	_hasBrush = false;
	_clipEnabled = false;
	_clipRegion = _area;
	_transform = QTransform();

	return _painter.begin(pdev);
}

bool WatchPaintEngine::end()
{
	return _painter.end();
}

void WatchPaintEngine::damageMappedRect(const QRect &r)
{
	if (_clipEnabled) {
		_damaged += _clipRegion.intersected(r);
	} else {
		_damaged += r;
	}
}

void WatchPaintEngine::damageRect(const QRect &r)
{
	damageMappedRect(_transform.mapRect(r));
}

void WatchPaintEngine::damageRect(const QRectF &r)
{
	damageMappedRect(_transform.mapRect(r).toAlignedRect());
}

void WatchPaintEngine::damagePenStroke(const QLineF &line)
{
	if (!_hasPen) return;

	const qreal a = line.angle();
	const qreal sn = sinf(a);
	const qreal cs = cosf(a);
	const qreal w = _penWidth == 0.0 ? 1.0 : _penWidth;
	const qreal x1 = line.x1();
	const qreal x2 = line.x2();
	const qreal y1 = line.y1();
	const qreal y2 = line.y2();

	QPointF p1(x1-(w*sn/2.0f), y1+(w*cs/2.0f));
	QPointF p2(x2+(w*sn/2.0f), y2-(w*cs/2.0f));
	QRectF r = QRectF(p1, p2).normalized();
	damageRect(r);
}

void WatchPaintEngine::updateClipRegion(const QRegion& region, Qt::ClipOperation op)
{
	QRegion mapped = _transform.map(region);
	switch (op) {
		case Qt::NoClip:
			_clipEnabled = false;
			_clipRegion = _area;
			break;
		case Qt::ReplaceClip:
			_clipEnabled = true;
			_clipRegion = mapped;
			break;
		case Qt::IntersectClip:
			_clipEnabled = true;
			_clipRegion &= mapped;
			break;
		case Qt::UniteClip:
			_clipEnabled = true;
			_clipRegion |= mapped;
			break;
	}
}

void WatchPaintEngine::drawEllipse(const QRectF &r)
{
	TRACE(qDebug() << __func__ << r);
	damageRect(r);
	_painter.drawEllipse(r);
}

void WatchPaintEngine::drawEllipse(const QRect &r)
{
	TRACE(qDebug() << __func__ << r);
	damageRect(r);
	_painter.drawEllipse(r);
}

void WatchPaintEngine::drawImage(const QRectF &r, const QImage &pm, const QRectF &sr, Qt::ImageConversionFlags flags)
{
	TRACE(qDebug() << __func__ << r);
	damageRect(r);
	_painter.drawImage(r, pm, sr, flags);
}

void WatchPaintEngine::drawLines(const QLineF *lines, int lineCount)
{
	TRACE(qDebug() << __func__ << lines << lineCount);
	int i;
	for (i = 0; i < lineCount; i++) {
		const QLineF& line = lines[i];
		damagePenStroke(line);
	}
	_painter.drawLines(lines, lineCount);
}

void WatchPaintEngine::drawLines(const QLine *lines, int lineCount)
{
	TRACE(qDebug() << __func__ << lines << lineCount);
	int i;
	for (i = 0; i < lineCount; i++) {
		const QLine& line = lines[i];
		damagePenStroke(line);
	}
	_painter.drawLines(lines, lineCount);
}

void WatchPaintEngine::drawPath(const QPainterPath &path)
{
	TRACE(qDebug() << __func__ << path);
	damageRect(path.boundingRect());
	_painter.drawPath(path);
}

void WatchPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
	damageRect(r);
	_painter.drawPixmap(r, pm, sr);
}

void WatchPaintEngine::drawPoints(const QPointF *points, int pointCount)
{
	int i;
	for (i = 0; i < pointCount; i++) {
		const QPointF& p = points[i];
		damageRect(QRectF(p.x() - _penWidth/2.0f, p.y() - _penWidth/2.0f,
						  _penWidth, _penWidth));
	}
	_painter.drawPoints(points, pointCount);
}

void WatchPaintEngine::drawPoints(const QPoint *points, int pointCount)
{
	int i;
	for (i = 0; i < pointCount; i++) {
		const QPoint& p = points[i];
		damageRect(QRect(p.x() - _penWidth/2, p.y() - _penWidth/2,
						 _penWidth, _penWidth));
	}
	_painter.drawPoints(points, pointCount);
}

void WatchPaintEngine::drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
	QPolygonF p(pointCount);
	int i;
	for (i = 0; i < pointCount; i++) {
		p[i] = points[i];
	}
	damageRect(p.boundingRect());
	_painter.drawPolygon(points, pointCount,
						 mode == WindingMode ? Qt::WindingFill : Qt::OddEvenFill);
}

void WatchPaintEngine::drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode)
{
	QPolygon p(pointCount);
	int i;
	for (i = 0; i < pointCount; i++) {
		p[i] = points[i];
	}
	damageRect(p.boundingRect());
	_painter.drawPolygon(points, pointCount,
						 mode == WindingMode ? Qt::WindingFill : Qt::OddEvenFill);
}

void WatchPaintEngine::drawRects(const QRectF *rects, int rectCount)
{
	TRACE(qDebug() << __func__ << rects << rectCount);
	int i;
	for (i = 0; i < rectCount; i++) {
		const QRectF& r = rects[i];
		if (_hasBrush) {
			damageRect(r);
		}
		if (_hasPen) {
			damagePenStroke(QLineF(r.left(), r.top(), r.right(), r.top()));
			damagePenStroke(QLineF(r.right(), r.top(), r.right(), r.bottom()));
			damagePenStroke(QLineF(r.left(), r.bottom(), r.right(), r.bottom()));
			damagePenStroke(QLineF(r.left(), r.top(), r.left(), r.bottom()));
		}
	}
	_painter.drawRects(rects, rectCount);
}

void WatchPaintEngine::drawRects(const QRect *rects, int rectCount)
{
	TRACE(qDebug() << __func__ << rects << rectCount);
	int i;
	for (i = 0; i < rectCount; i++) {
		const QRect& r = rects[i];
		if (_hasBrush) {
			damageRect(r);
		}
		if (_hasPen) {
			damagePenStroke(QLine(r.left(), r.top(), r.right(), r.top()));
			damagePenStroke(QLine(r.right(), r.top(), r.right(), r.bottom()));
			damagePenStroke(QLine(r.left(), r.bottom(), r.right(), r.bottom()));
			damagePenStroke(QLine(r.left(), r.top(), r.left(), r.bottom()));
		}
	}

	_painter.drawRects(rects, rectCount);
}

void WatchPaintEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
	TRACE(qDebug() << __func__ << p << textItem.text());
	const qreal ascent = textItem.ascent();
	const qreal descent = textItem.descent();
	const qreal w = textItem.width();
	damageRect(QRect(p.x(), p.y() - ascent, w, ascent + descent));
	_painter.drawTextItem(p, textItem);
}

void WatchPaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s)
{
	TRACE(qDebug() << __func__ << r << pixmap << s);
	damageRect(r);
	_painter.drawTiledPixmap(r, pixmap, s);
}

QPaintEngine::Type WatchPaintEngine::type() const
{
	return QPaintEngine::Raster;
}

void WatchPaintEngine::updateState(const QPaintEngineState &state)
{
	const QPaintEngine::DirtyFlags flags = state.state();

	TRACE(qDebug() << __func__ << flags);

	if (flags & QPaintEngine::DirtyTransform)
	{
		TRACE(qDebug() << " " << "DirtyTransform" << state.transform());
		_transform = state.transform();
		_painter.setTransform(_transform);
	}

	if (flags & QPaintEngine::DirtyBackground)
	{
		_painter.setBackground(state.backgroundBrush());
	}

	if (flags & QPaintEngine::DirtyBackgroundMode)
	{
		_painter.setBackgroundMode(state.backgroundMode());
	}

	if (flags & QPaintEngine::DirtyBrush)
	{
		QBrush brush = state.brush();
		_hasBrush = brush.style() != Qt::NoBrush;
		_painter.setBrush(brush);
	}
	if (flags & QPaintEngine::DirtyBrushOrigin)
	{
		_painter.setBrushOrigin(state.brushOrigin());
	}
	if (flags & QPaintEngine::DirtyClipEnabled)
	{
		TRACE(qDebug() << " " << "DirtyClipEnabled" << state.isClipEnabled());
		_clipEnabled = state.isClipEnabled();
		_painter.setClipping(_clipEnabled);
	}
	if (flags & QPaintEngine::DirtyClipPath)
	{
		TRACE(qDebug() << " " << "DirtyClipPath" << state.clipPath().boundingRect());
		QRegion region = state.clipPath().boundingRect().toAlignedRect();
		updateClipRegion(region, state.clipOperation());
		_painter.setClipPath(state.clipPath(), state.clipOperation());
	}
	if (flags & QPaintEngine::DirtyClipRegion)
	{
		TRACE(qDebug() << " " << "DirtyClipRegion" << state.clipRegion());
		updateClipRegion(state.clipRegion(), state.clipOperation());
		_painter.setClipRegion(state.clipRegion(), state.clipOperation());
	}
	if (flags & QPaintEngine::DirtyCompositionMode)
	{
		TRACE(qDebug() << " " << "DirtyCompositionMode" << state.compositionMode());
		_painter.setCompositionMode(state.compositionMode());
	}
	if (flags & QPaintEngine::DirtyFont)
	{
		_painter.setFont(state.font());
	}
	if (flags & QPaintEngine::DirtyHints)
	{
		_painter.setRenderHints(state.renderHints());
	}
	if (flags & QPaintEngine::DirtyPen)
	{
		QPen pen = state.pen();
		_hasPen = pen.style() != Qt::NoPen;
		_penWidth = pen.widthF();
		_painter.setPen(pen);
	}

	TRACE(qDebug() << __func__ << "end");
}
