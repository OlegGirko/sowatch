#include "metawatch.h"
#include "metawatchpaintengine.h"

using namespace sowatch;

const QRect MetaWatchPaintEngine::totalAreaRect(0, 0, MetaWatch::screenWidth, MetaWatch::screenHeight);

MetaWatchPaintEngine::MetaWatchPaintEngine(MetaWatch* watch) :
	WatchPaintEngine(),
	_watch(watch)
{
}

bool MetaWatchPaintEngine::begin(QPaintDevice *pdev)
{
	_watch = static_cast<MetaWatch*>(pdev);
	_mode = _watch->paintTargetMode();

	return WatchPaintEngine::begin(_watch->imageFor(_mode));
}

bool MetaWatchPaintEngine::end()
{
	bool ret = WatchPaintEngine::end();
	if (ret) {
		_watch->update(_mode, _damaged.rects().toList());
	}
	return ret;
}

void MetaWatchPaintEngine::drawRects(const QRectF *rects, int rectCount)
{
	int i;
	for (i = 0; i < rectCount; i++) {
		const QRectF& r = rects[i];
		if (_hasBrush && fillsEntireImage(r.toRect()) && (_isBrushBlack | _isBrushWhite)) {
			_watch->clear(_mode, _isBrushBlack);
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

void MetaWatchPaintEngine::drawRects(const QRect *rects, int rectCount)
{
	int i;
	for (i = 0; i < rectCount; i++) {
		const QRect& r = rects[i];
		if (_hasBrush && fillsEntireImage(r) && (_isBrushBlack | _isBrushWhite)) {
			_watch->clear(_mode, _isBrushBlack);
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

void MetaWatchPaintEngine::updateState(const QPaintEngineState &state)
{
	WatchPaintEngine::updateState(state);
	if (state.state() & QPaintEngine::DirtyBrush) {
		QBrush brush = state.brush();
		_isBrushBlack = false;
		_isBrushWhite = false;
		if (brush.style() == Qt::SolidPattern) {
			const QColor color = brush.color();
			if (color == Qt::black) {
				_isBrushBlack = true;
			} else if (color == Qt::white) {
				_isBrushWhite = true;
			}
		}
	}
}

bool MetaWatchPaintEngine::fillsEntireImage(const QRect& rect)
{
	return rect == totalAreaRect &&
			(!_clipEnabled ||
			 (_clipRegion.numRects() == 1 && _clipRegion.rects().at(0) == totalAreaRect));
}
