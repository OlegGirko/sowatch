#include <QtCore/QDebug>
#include <QtGui/QPainter>

#include "metawatchsimulator.h"

#define SIMULATE_DAMAGES 1
#define SIMULATE_FRAMERATE 1

using namespace sowatch;

MetaWatchSimulator::MetaWatchSimulator(QObject *parent) :
	WatchSimulator(parent),
	_image(96, 96, QImage::Format_Mono),
	_screen(96, 96),
	_form(new MetaWatchSimulatorForm),
	_nextFrame(QTime::currentTime())
{
	_form->showNormal();
	connect(_form, SIGNAL(destroyed()), SIGNAL(disconnected()));
	connect(_form, SIGNAL(buttonPressed(int)), SIGNAL(buttonPressed(int)));
	connect(_form, SIGNAL(buttonReleased(int)), SIGNAL(buttonReleased(int)));
}

MetaWatchSimulator::~MetaWatchSimulator()
{
	delete _form;
}

QString MetaWatchSimulator::model() const
{
	return "metawatch-digital";
}

bool MetaWatchSimulator::isConnected() const
{
	return true;
}

bool MetaWatchSimulator::busy() const
{
#if SIMULATE_FRAMERATE
	return _nextFrame > QTime::currentTime();
#else
	return false;
#endif
}

void MetaWatchSimulator::update(const QList<QRect> &rects)
{
#if SIMULATE_DAMAGES
	const QRect imageRect = _image.rect();
	QPainter p;
	QVector<bool> rows(96, false);
	unsigned total = 0, totalRows;

	p.begin(&_screen);
	foreach (const QRect& rect, rects) {
		QRect r = rect.intersect(imageRect);
		for (int i = r.top(); i <= r.bottom(); i++) {
			rows[i] = true;
		}
		total += r.width() * r.height();

		p.drawImage(r, _image, r);
	}
	p.end();

	totalRows = rows.count(true);

	_form->refreshScreen(_screen);

	qDebug() << "updated " << total << " pixels " << totalRows << " lines";
	_nextFrame = QTime::currentTime().addMSecs(((totalRows / 2) + 1) * 30);
#else
	_form->refreshScreen(QPixmap::fromImage(_image));
	_nextFrame = QTime::currentTime().addMSecs(30);
#endif
}

void MetaWatchSimulator::vibrate(bool on)
{
	qDebug() << "vibrate" << on;
}
