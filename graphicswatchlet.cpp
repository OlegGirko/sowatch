#include <QtCore/QDebug>
#include <QtGui/QPainter>

#include "watch.h"
#include "graphicswatchlet.h"

using namespace sowatch;

GraphicsWatchlet::GraphicsWatchlet(WatchServer* server, const QString& id) :
	Watchlet(server, id), _scene(0), _damaged()
{
}

QGraphicsScene* GraphicsWatchlet::scene()
{
	return _scene;
}

void GraphicsWatchlet::setScene(QGraphicsScene *scene)
{
	if (_scene) {
		disconnect(this, SLOT(sceneChanged(QList<QRectF>)));
	}
	_scene = scene;
	if (_scene) {
		connect(_scene, SIGNAL(changed(QList<QRectF>)),
				this, SLOT(sceneChanged(QList<QRectF>)));
	}
}

void GraphicsWatchlet::sceneChanged(const QList<QRectF> &region)
{
	foreach(const QRectF& r, region)
	{
		_damaged += r.toRect();
	}

	if (!_damaged.isEmpty() && _active && !watch()->busy()) {
		const QVector<QRect> rects = _damaged.rects();
		QPainter p(watch());
		foreach(const QRect& r, rects)
		{
			_scene->render(&p, r, r, Qt::IgnoreAspectRatio);
		}
		_damaged = QRegion();
	}
}
