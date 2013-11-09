#ifndef LIVEVIEWPAINTENGINE_H
#define LIVEVIEWPAINTENGINE_H

#include <sowatch.h>
#include "liveview.h"

namespace sowatch
{

class LiveViewPaintEngine : public WatchPaintEngine
{
public:
	LiveViewPaintEngine();

	bool begin(QPaintDevice *pdev);
	bool end();

	void drawRects(const QRectF *rects, int rectCount);
	void drawRects(const QRect *rects, int rectCount);

	void updateState(const QPaintEngineState &state);

protected:
	bool fillsEntireImage(const QRect& rect);

	LiveView* _watch;
	bool _isBrushBlack;
};

}

#endif // LIVEVIEWPAINTENGINE_H
