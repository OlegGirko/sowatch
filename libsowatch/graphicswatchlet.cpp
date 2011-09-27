#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtGui/QPainter>

#include "watch.h"
#include "graphicswatchlet.h"

using namespace sowatch;

GraphicsWatchlet::GraphicsWatchlet(WatchServer* server, const QString& id) :
	Watchlet(server, id), _scene(0), _frameTimer(), _damaged()
{
	_frameTimer.setSingleShot(true);
	connect(&_frameTimer, SIGNAL(timeout()), SLOT(frameTimeout()));
}

GraphicsWatchlet::~GraphicsWatchlet()
{

}

QGraphicsScene* GraphicsWatchlet::scene()
{
	return _scene;
}

void GraphicsWatchlet::setScene(QGraphicsScene *scene)
{
	if (_scene) {
		disconnect(_scene, 0, this, 0);
	}
	_scene = scene;
	if (_scene) {
		connect(_scene, SIGNAL(changed(QList<QRectF>)),
				this, SLOT(sceneChanged(QList<QRectF>)));
	}
}

void GraphicsWatchlet::sceneChanged(const QList<QRectF> &region)
{
	foreach(const QRectF& r, region) {
		_damaged += r.toRect();
	}
	if (!_damaged.isEmpty()) {
		_frameTimer.start(frameDelay);
	}
}

void GraphicsWatchlet::frameTimeout()
{
	if (!_active) return; // Watchlet was ejected, do not draw.
	if (watch()->busy()) {
		_frameTimer.start(busyFrameDelay);
		return;
	}

	const QVector<QRect> rects = _damaged.rects();
	QPainter p(watch());

	foreach(const QRect& r, rects) {
		_scene->render(&p, r, r, Qt::IgnoreAspectRatio);
	}
	_damaged = QRegion();
}

void GraphicsWatchlet::activate()
{
	Watchlet::activate();
	// We have to assume that the watch has completely forgot about everything.
	QRect area(0, 0, watch()->width(), watch()->height());
	_damaged += area;
	_scene->update(area);
}

void GraphicsWatchlet::deactivate()
{
	_frameTimer.stop();
	Watchlet::deactivate();
}
