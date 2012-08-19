#ifndef COMPASSVIEW_H
#define COMPASSVIEW_H

#include <QtGui/QImage>
#include <QtDeclarative/QDeclarativeItem>
#include <QtLocation/QGeoPositionInfoSource>

using QTM_PREPEND_NAMESPACE(QGeoPositionInfoSource);
using QTM_PREPEND_NAMESPACE(QGeoPositionInfo);

class CompassView : public QDeclarativeItem
{
	Q_OBJECT
	Q_PROPERTY(bool updateEnabled READ updateEnabled WRITE setUpdateEnabled NOTIFY updateEnabledChanged)
	Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)

	Q_PROPERTY(qreal currentSpeed READ currentSpeed NOTIFY currentSpeedChanged)
	Q_PROPERTY(qreal currentAltitude READ currentAltitude NOTIFY currentAltitudeChanged)

public:
	explicit CompassView(QDeclarativeItem *parent = 0);
	
	bool updateEnabled() const;
	void setUpdateEnabled(bool enabled);

	int updateInterval() const;
	void setUpdateInterval(int msec);

	qreal currentSpeed() const;
	qreal currentAltitude() const;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
	void updateEnabledChanged();
	void updateIntervalChanged();
	void currentSpeedChanged();
	void currentAltitudeChanged();

protected:
	void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private slots:
	void handlePositionUpdate(const QGeoPositionInfo& info);
	
private:
	bool _enabled;
	QSize _size;
	QImage _image;
	QGeoPositionInfoSource *_posSource;
	qreal _direction;
	qreal _speed;
	qreal _altitude;
};

#endif // COMPASSVIEW_H
