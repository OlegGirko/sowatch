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

	// Connect form signals
	connect(_form, SIGNAL(buttonPressed(int)), SIGNAL(buttonPressed(int)));
	connect(_form, SIGNAL(buttonReleased(int)), SIGNAL(buttonReleased(int)));
	connect(_form, SIGNAL(destroyed()), SLOT(handleFormDestroyed()));

	// Show the form
	_form->showNormal();

	// Schedule a connection even if BT is off or anything like that.
	scheduleConnect();
}

MetaWatchDigitalSimulator::~MetaWatchDigitalSimulator()
{
	_connected = false;
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

	if (mode == IdleMode) {
		QRect systemArea(0, 0, screenWidth, systemAreaHeight);
		p.fillRect(systemArea, Qt::BDiagPattern);
		p.drawText(systemArea, Qt::AlignCenter, "System area");
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

void MetaWatchDigitalSimulator::connectToWatch()
{
	// Skip BluetoothWatch connection stuff

	if (!_connected && _form) {
		qDebug() << "simulator connected";

		_connected = true;
		_currentMode = IdleMode;
		_paintMode = IdleMode;

		MetaWatchDigital::setupBluetoothWatch();

		emit connected();
	}
}

void MetaWatchDigitalSimulator::send(const Message &msg)
{
	// Do not send messages
	Q_UNUSED(msg);
}

void MetaWatchDigitalSimulator::handleFormDestroyed()
{
	if (_connected) {
		_connected = false;
		qDebug() << "simulator disconnected";
		emit disconnected();
		_form = 0;
	}
}
