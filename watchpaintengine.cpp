#include <QtCore/QDebug>
#include <math.h>

#include "watch.h"
#include "watchpaintengine.h"

using namespace sowatch;

WatchPaintEngine::WatchPaintEngine(Watch* watch, QImage* image)
	: QPaintEngine(QPaintEngine::AllFeatures),
	  _watch(watch), _painter(),
	  _hasPen(false), _hasBrush(false), _clipEnabled(false)
{
	Q_UNUSED(image);
}

bool WatchPaintEngine::begin(QPaintDevice *pdev)
{
	_damaged = QRegion();
	_watch = static_cast<Watch*>(pdev);

	return _painter.begin(&_watch->_image);
}

bool WatchPaintEngine::end()
{
	bool ret = _painter.end();
	if (ret) {
		_watch->update(_damaged.rects().toList());
	}
	return ret;
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
	damageMappedRect(_transform.mapRect(r).toRect());
}

void WatchPaintEngine::damagePenStroke(const QLineF &line)
{
	if (!_hasPen) return;

	const qreal a = line.angle();
	const qreal sn = sinf(a);
	const qreal cs = cosf(a);
	const qreal w = _penWidth = 0.0 ? 1.0 : _penWidth;
	const qreal x1 = line.x1();
	const qreal x2 = line.x2();
	const qreal y1 = line.x1();
	const qreal y2 = line.y2();

	damageRect(QRectF(x1-(w*sn/2.0f), y1+(w*cs/2.0f), x2+(w*sn/2.0f), y2-(w*cs/2.0f)).normalized());
}

void WatchPaintEngine::updateClipRegion(const QRegion& region, Qt::ClipOperation op)
{
	switch(op) {
		case Qt::NoClip:
			_clipEnabled = false;
			_clipRegion = QRegion(0, 0, _watch->width(), _watch->height());
			break;
		case Qt::ReplaceClip:
			_clipEnabled = true;
			_clipRegion = region;
			break;
		case Qt::IntersectClip:
			_clipEnabled = true;
			_clipRegion &= region;
			break;
		case Qt::UniteClip:
			_clipEnabled = true;
			_clipRegion |= region;
			break;
	}
}

void WatchPaintEngine::drawEllipse(const QRectF &r)
{
	damageRect(r);
	_painter.drawEllipse(r);
}

void WatchPaintEngine::drawEllipse(const QRect &r)
{
	damageRect(r);
	_painter.drawEllipse(r);
}

void WatchPaintEngine::drawImage(const QRectF &r, const QImage &pm, const QRectF &sr, Qt::ImageConversionFlags flags)
{
	damageRect(r);
	_painter.drawImage(r, pm, sr, flags);
}

void WatchPaintEngine::drawLines(const QLineF *lines, int lineCount)
{
	int i;
	for (i = 0; i < lineCount; i++) {
		const QLineF& line = lines[i];
		damagePenStroke(line);
	}
	_painter.drawLines(lines, lineCount);
}

void WatchPaintEngine::drawLines(const QLine *lines, int lineCount)
{
	int i;
	for (i = 0; i < lineCount; i++) {
		const QLine& line = lines[i];
		damagePenStroke(line);
	}
	_painter.drawLines(lines, lineCount);
}

void WatchPaintEngine::drawPath(const QPainterPath &path)
{
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
	const qreal penWidth = _painter.pen().widthF();
	int i;
	for (i = 0; i < pointCount; i++) {
		const QPointF& p = points[i];
		damageRect(QRect(p.x() - penWidth/2, p.y() - penWidth/2,
						 p.x() + penWidth/2, p.y() + penWidth/2));
	}
	_painter.drawPoints(points, pointCount);
}

void WatchPaintEngine::drawPoints(const QPoint *points, int pointCount)
{
	const qreal penWidth = _painter.pen().widthF();
	int i;
	for (i = 0; i < pointCount; i++) {
		const QPoint& p = points[i];
		damageRect(QRect(p.x() - penWidth/2, p.y() - penWidth/2,
						 p.x() + penWidth/2, p.y() + penWidth/2));
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
	const qreal height = textItem.ascent() + textItem.descent();
	damageRect(QRect(p.x(), p.y(), p.x() + textItem.width(), p.y() + height));
	_painter.drawTextItem(p, textItem);
}

void WatchPaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s)
{
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
		_clipEnabled = state.isClipEnabled();
		_painter.setClipping(_clipEnabled);
	}
	if (flags & QPaintEngine::DirtyClipPath)
	{
		QRegion region = state.clipPath().boundingRect().toRect();
		updateClipRegion(region, state.clipOperation());
		_painter.setClipPath(state.clipPath(), state.clipOperation());
	}
	if (flags & QPaintEngine::DirtyClipRegion)
	{
		updateClipRegion(state.clipRegion(), state.clipOperation());
		_painter.setClipRegion(state.clipRegion(), state.clipOperation());
	}
	if (flags & QPaintEngine::DirtyCompositionMode)
	{
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
	if (flags & QPaintEngine::DirtyTransform)
	{
		_transform = state.transform();
		_painter.setTransform(_transform);
	}
}
