#include "metawatch.h"
#include "metawatchpaintengine.h"

using namespace sowatch;

MetaWatchPaintEngine::MetaWatchPaintEngine(MetaWatch* watch, QImage* image) :
	WatchPaintEngine(watch, image),	_watch(watch),
	_imageRect(image->rect())
{
}

void MetaWatchPaintEngine::drawRects(const QRectF *rects, int rectCount)
{
	int i;
	for (i = 0; i < rectCount; i++) {
		const QRectF& r = rects[i];
		if (_hasBrush && r.toRect() == _imageRect && (_isBrushBlack | _isBrushWhite)) {
			_watch->clear(_isBrushWhite);
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
		if (_hasBrush && r == _imageRect && (_isBrushBlack | _isBrushWhite)) {
			_watch->clear(_isBrushWhite);
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
