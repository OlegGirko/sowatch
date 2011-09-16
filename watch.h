#ifndef WATCH_H
#define WATCH_H

#include <QtCore/QObject>
#include <QtGui/QPaintDevice>
#include <QtGui/QImage>

namespace sowatch
{

class Watch : public QObject, public QPaintDevice
{
	Q_OBJECT
	Q_PROPERTY(QString model READ model)
	Q_PROPERTY(bool connected READ isConnected)
public:
	explicit Watch(const QImage& image, QObject* parent = 0);
	~Watch();

	QPaintEngine* paintEngine() const;
	int metric(PaintDeviceMetric metric) const;

	Q_INVOKABLE virtual QString model() const = 0;
	Q_INVOKABLE virtual bool isConnected() const = 0;

	/** Indicates if watch is too busy atm and we should limit frame rate. */
	Q_INVOKABLE virtual bool busy() const = 0;

signals:
	void connected();
	void disconnected();
	void buttonPressed(int button);
	void buttonReleased(int button);

public slots:
	virtual void update(const QList<QRect>& rects) = 0;
	virtual void update(const QRect& rect);
	virtual void vibrate(bool on) = 0;

protected:
	QImage _image;
	mutable QPaintEngine* _paintEngine;

friend class WatchPaintEngine;
};

}

#endif // WATCH_H
