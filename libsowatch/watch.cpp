#include "watch.h"
#include "watchpaintengine.h"

using namespace sowatch;

Watch::Watch(QObject* parent) :
	QObject(parent)
{

}

Watch::~Watch()
{

}

void Watch::vibrate(int msecs)
{
	/* The default implementation does nothing. */
	Q_UNUSED(msecs);
}
