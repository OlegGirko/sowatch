#ifndef METAWATCHPAINTENGINE_H
#define METAWATCHPAINTENGINE_H

#include <QtCore/QRect>
#include "watchpaintengine.h"

namespace sowatch
{

class MetaWatch;

/** This WatchPaintEngine accelerates fillRects by using the MetaWatch's template command. */
class MetaWatchPaintEngine : public WatchPaintEngine
{
public:
	explicit MetaWatchPaintEngine(MetaWatch* watch, QImage* image);

	void drawRects(const QRectF *rects, int rectCount);
	void drawRects(const QRect *rects, int rectCount);

	void updateState(const QPaintEngineState &state);

protected:
	bool fillsEntireImage(const QRect& rect);

	MetaWatch* _watch;
	QRect _imageRect;
	bool _isBrushBlack;
	bool _isBrushWhite;
};

}

#endif // METAWATCHPAINTENGINE_H
