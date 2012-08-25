#include <math.h>

#include "qmapwatchletplugin.h"
#include "compassview.h"

using namespace sowatch;
QTM_USE_NAMESPACE

CompassView::CompassView(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    _enabled(false),
    _image(SOWATCH_QML_DIR "/qmapwatchlet/compass.png"),
    _posSource(QGeoPositionInfoSource::createDefaultSource(this)),
    _direction(-1.0), _speed(NAN), _altitude(NAN)
{
	if (_posSource) {
		connect(_posSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
				SLOT(handlePositionUpdate(QGeoPositionInfo)));
		_posSource->lastKnownPosition();
	} else {
		qWarning() << "No position source for moving map!";
	}

	setFlag(QGraphicsItem::ItemHasNoContents, false);
}

bool CompassView::updateEnabled() const
{
	return _enabled;
}

void CompassView::setUpdateEnabled(bool enabled)
{
	if (_posSource && _enabled != enabled) {
		if (enabled) {
			qDebug() << "Start position updates";
			_posSource->startUpdates();
		} else {
			qDebug() << "Stop position updates";
			_posSource->stopUpdates();
		}
		_enabled = enabled;

		emit updateEnabledChanged();
	}

}

int CompassView::updateInterval() const
{
	if (_posSource) {
		return _posSource->updateInterval();
	} else {
		return 0;
	}
}

void CompassView::setUpdateInterval(int msec)
{
	if (_posSource) {
		_posSource->setUpdateInterval(msec);
		emit updateIntervalChanged();
	}
}

qreal CompassView::currentSpeed() const
{
	return _speed;
}

qreal CompassView::currentAltitude() const
{
	return _altitude;
}

void CompassView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
	// Now render the arrow indicator
	const int centerX = _size.width() / 2, centerY = _size.height() / 2;

	if (_direction >= 0.0) {
		painter->save();
		painter->translate(centerX, centerY);
		painter->rotate(_direction);
		painter->drawImage(-_image.width() / 2, -_image.height() / 2, _image);
		painter->restore();
	}
}

void CompassView::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
	Q_UNUSED(oldGeometry);
	_size = newGeometry.size().toSize();
}

void CompassView::handlePositionUpdate(const QGeoPositionInfo &info)
{
	qDebug() << "Pos update";

	qreal newDirection = -1.0;
	if (info.hasAttribute(QGeoPositionInfo::Direction)) {
		newDirection = info.attribute(QGeoPositionInfo::Direction);
	}
	qDebug() << "New dir" << newDirection;
	if (newDirection != _direction) {
		_direction = newDirection;
		update();
	}

	qreal newSpeed = NAN;
	if (info.hasAttribute(QGeoPositionInfo::GroundSpeed)) {
		newSpeed = info.attribute(QGeoPositionInfo::GroundSpeed);
	}
	qDebug() << "New speed" << newSpeed;
	if (newSpeed != _speed) {
		_speed = newSpeed;
		emit currentSpeedChanged();
	}

	qreal newAlt = NAN;
	if (info.hasAttribute(QGeoPositionInfo::VerticalAccuracy)) {
		newAlt = info.coordinate().altitude();
	}
	qDebug() << "New altitude" << newAlt;
	if (newAlt != _altitude) {
		_altitude = newAlt;
		emit currentAltitudeChanged();
	}
}
