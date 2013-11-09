#ifndef METAWATCHPAINTENGINE_H
#define METAWATCHPAINTENGINE_H

#include <sowatch.h>
#include "metawatch.h"

namespace sowatch
{

/** This WatchPaintEngine accelerates fillRects by using the MetaWatch's template command. */
class MetaWatchPaintEngine : public WatchPaintEngine
{
public:
	MetaWatchPaintEngine();

	bool begin(QPaintDevice *pdev);
	bool end();

	void drawRects(const QRectF *rects, int rectCount);
	void drawRects(const QRect *rects, int rectCount);

	void updateState(const QPaintEngineState &state);

protected:
	bool fillsEntireImage(const QRect& rect);

	MetaWatch* _watch;
	MetaWatch::Mode _mode;
	bool _isBrushBlack;
	bool _isBrushWhite;
};

}

#endif // METAWATCHPAINTENGINE_H
