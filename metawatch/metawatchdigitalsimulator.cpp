#include <QtCore/QDebug>
#include <QtGui/QPainter>

#include "metawatchdigitalsimulator.h"

#define SIMULATE_DAMAGES 1
#define SIMULATE_FRAMERATE 1

using namespace sowatch;

MetaWatchDigitalSimulator::MetaWatchDigitalSimulator(ConfigKey *config, QObject *parent) :
	MetaWatchDigital(config, parent),
	_form(new MetaWatchDigitalSimulatorForm),
	_nextFrame(QTime::currentTime())
{
	_pixmap[IdleMode] = QPixmap(screenWidth, screenHeight);
	_pixmap[ApplicationMode] = QPixmap(screenWidth, screenHeight);
	_pixmap[NotificationMode] = QPixmap(screenWidth, screenHeight);
	_form->showNormal();
	connect(_form, SIGNAL(buttonPressed(int)), SIGNAL(buttonPressed(int)));
	connect(_form, SIGNAL(buttonReleased(int)), SIGNAL(buttonReleased(int)));
}

MetaWatchDigitalSimulator::~MetaWatchDigitalSimulator()
{
	delete _form;
}

bool MetaWatchDigitalSimulator::busy() const
{
#if SIMULATE_FRAMERATE
	return _nextFrame > QTime::currentTime();
#else
	return false;
#endif
}

void MetaWatchDigitalSimulator::displayIdleScreen()
{
	MetaWatchDigital::displayIdleScreen();
	_form->refreshScreen(_pixmap[_currentMode]);
}

void MetaWatchDigitalSimulator::displayNotification(Notification *notification)
{
	MetaWatchDigital::displayNotification(notification);
	_form->refreshScreen(_pixmap[_currentMode]);
}

void MetaWatchDigitalSimulator::displayApplication()
{
	MetaWatchDigital::displayApplication();
	// No need to refresh.
}

void MetaWatchDigitalSimulator::clear(Mode mode, bool black)
{
	_pixmap[mode].fill(black ? Qt::black : Qt::white);
	if (mode == _currentMode) {
		_form->refreshScreen(_pixmap[mode]);
	}
}

void MetaWatchDigitalSimulator::update(Mode mode, const QList<QRect> &rects)
{
#if SIMULATE_DAMAGES
	const QRect imageRect = _image[mode].rect();
	QPainter p;
	QVector<bool> rows(96, false);

	p.begin(&_pixmap[mode]);
	foreach (const QRect& rect, rects) {
		QRect r = rect.intersect(imageRect);
		for (int i = r.top(); i <= r.bottom(); i++) {
			rows[i] = true;
		}

		p.drawImage(r, _image[mode], r);
	}
	p.end();

	int totalRows = rows.count(true);

	qDebug() << "updated" << totalRows << "lines";
	_nextFrame = QTime::currentTime().addMSecs(((totalRows / 2) + 1) * DelayBetweenMessages);
#else
	Q_UNUSED(rects);
	_pixmap[mode] = QPixmap::fromImage(_image[mode]);
	_nextFrame = QTime::currentTime().addMSecs(DelayBetweenMessages);
#endif
	if (mode == _currentMode) {
		_form->refreshScreen(_pixmap[mode]);
	}
}

void MetaWatchDigitalSimulator::vibrate(bool on)
{
	qDebug() << "vibrate" << on;
}

void MetaWatchDigitalSimulator::retryConnect()
{
	if (!_connected && _form) {
		qDebug() << "connected";

		_connected = true;
		_currentMode = IdleMode;
		_paintMode = IdleMode;

		handleWatchConnected();

		emit connected();
	}
}

void MetaWatchDigitalSimulator::send(const Message &msg)
{
	// Do not send messages
	Q_UNUSED(msg);
}
