#include "watch.h"
#include "watchpaintengine.h"

using namespace sowatch;

Watch::Watch(const QImage& image, QObject* parent) :
	QObject(parent), _image(image), _paintEngine(0)
{

}

Watch::~Watch()
{
	if (_paintEngine) {
		delete _paintEngine;
	}
}

QPaintEngine* Watch::paintEngine() const
{
	if (!_paintEngine) {
		_paintEngine = new WatchPaintEngine(const_cast<Watch*>(this),
											const_cast<QImage*>(&_image));
	}

	return _paintEngine;
}

int Watch::metric(PaintDeviceMetric metric) const
{
	switch (metric) {
	case PdmWidth:
		return _image.width();
	case PdmHeight:
		return _image.height();
	case PdmWidthMM:
		return _image.widthMM();
	case PdmHeightMM:
		return _image.heightMM();
	case PdmNumColors:
		return _image.numColors();
	case PdmDepth:
		return _image.depth();
	case PdmDpiX:
		return _image.logicalDpiX();
	case PdmDpiY:
		return _image.logicalDpiY();
	case PdmPhysicalDpiX:
		return _image.physicalDpiX();
	case PdmPhysicalDpiY:
		return _image.physicalDpiY();
	}

	return -1;
}

void Watch::update(const QRect &rect)
{
	QList<QRect> rects;
	rects << rect;
	update(rects);
}
