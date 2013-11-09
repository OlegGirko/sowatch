#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtGui/QPainter>

#include "watch.h"
#include "graphicswatchlet.h"

using namespace sowatch;

GraphicsWatchlet::GraphicsWatchlet(Watch* watch, const QString& id)
    : Watchlet(watch, id),
      _scene(0), _frameTimer(),
      _fullUpdateMode(false), _damaged()
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

bool GraphicsWatchlet::fullUpdateMode() const
{
	return _fullUpdateMode;
}

void GraphicsWatchlet::setFullUpdateMode(bool fullUpdateMode)
{
	_fullUpdateMode = fullUpdateMode;
}

QRectF GraphicsWatchlet::sceneRect() const
{
	if (_scene) {
		return _scene->sceneRect();
	} else {
		return QRectF();
	}
}

QRect GraphicsWatchlet::viewportRect() const
{
	if (_active) {
		const Watch *watch = this->watch();
		return QRect(0, 0, watch->width(), watch->height());
	} else {
		return QRect();
	}
}

void GraphicsWatchlet::sceneChanged(const QList<QRectF> &rects)
{
	// Only consider scene updates if the watchlet is active
	if (_active) {
		QRect viewport = viewportRect();
		if (_fullUpdateMode) {
			_damaged += viewport;
		} else {
			foreach(const QRectF& frect, rects) {
				QRect rect = frect.toAlignedRect() & viewport;
				_damaged += rect;
			}
		}

		// Start frame timer if we got new data
		if (!_damaged.isEmpty() && !_frameTimer.isActive()) {
			_frameTimer.start(frameDelay);
		}
	}
}

void GraphicsWatchlet::frameTimeout()
{
	// Do not draw if watchlet is not active
	if (!_active) return;

	if (watch()->busy()) {
		// Watch is busy, delay this frame.
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
	// We have to assume that the watch has completely forgot about everything
	// So assume the entire viewport is damaged
	QRect viewport = viewportRect();
	_damaged += viewport;
	// This will emit sceneChanged and start the frame timer.
	_scene->update(viewport);
}

void GraphicsWatchlet::deactivate()
{
	// Stop updates
	_frameTimer.stop();
	_damaged = QRegion();

	Watchlet::deactivate();
}
