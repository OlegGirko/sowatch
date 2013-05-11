#include "liveviewpaintengine.h"

using namespace sowatch;

LiveViewPaintEngine::LiveViewPaintEngine() :
    WatchPaintEngine()
{

}

bool LiveViewPaintEngine::begin(QPaintDevice *pdev)
{
	_watch = static_cast<LiveView*>(pdev);

	return WatchPaintEngine::begin(_watch->image());
}

bool LiveViewPaintEngine::end()
{
	bool ret = WatchPaintEngine::end();
	if (ret) {
		QRect rect = _damaged.boundingRect();
		if (!rect.isEmpty()) {
			QImage sub_image = _watch->image()->copy(rect);
			_watch->renderImage(rect.x(), rect.y(), sub_image);
		}
	}
	return ret;
}

void LiveViewPaintEngine::drawRects(const QRectF *rects, int rectCount)
{
	int i;
	for (i = 0; i < rectCount; i++) {
		const QRectF& r = rects[i];
		if (_hasBrush && fillsEntireImage(r.toRect()) && _isBrushBlack) {
			_watch->clear();
			_damaged = QRegion();
			continue;
		}
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

void LiveViewPaintEngine::drawRects(const QRect *rects, int rectCount)
{
	int i;
	for (i = 0; i < rectCount; i++) {
		const QRect& r = rects[i];
		if (_hasBrush && fillsEntireImage(r) && _isBrushBlack) {
			_watch->clear();
			_damaged = QRegion();
			continue;
		}
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

void LiveViewPaintEngine::updateState(const QPaintEngineState &state)
{
	WatchPaintEngine::updateState(state);
	if (state.state() & QPaintEngine::DirtyBrush) {
		QBrush brush = state.brush();
		_isBrushBlack = false;
		if (brush.style() == Qt::SolidPattern) {
			const QColor color = brush.color();
			if (color == Qt::black) {
				_isBrushBlack = true;
			}
		}
	}
}

bool LiveViewPaintEngine::fillsEntireImage(const QRect& rect)
{
	return rect == _area &&
			(!_clipEnabled ||
			 (_clipRegion.numRects() == 1 && _clipRegion.rects().at(0) == _area));
}
