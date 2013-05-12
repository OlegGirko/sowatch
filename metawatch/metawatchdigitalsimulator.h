#ifndef METAWATCHSIMULATOR_H
#define METAWATCHSIMULATOR_H

#include <QtCore/QTime>
#include <QtGui/QPixmap>
#include "metawatchdigital.h"
#include "metawatchdigitalsimulatorform.h"

namespace sowatch {

class MetaWatchDigitalSimulator : public MetaWatchDigital
{
    Q_OBJECT
public:
    explicit MetaWatchDigitalSimulator(ConfigKey *settings, QObject *parent = 0);
	~MetaWatchDigitalSimulator();

	bool busy() const;

	void displayIdleScreen();
	void displayNotification(Notification *notification);
	void displayApplication();

	void clear(Mode mode, bool black);
	void update(Mode mode, const QList<QRect> &rects);

	void vibrate(bool on);

	void connectToWatch();
	void send(const Message& msg);

private slots:
	void handleFormDestroyed();
	void handleButtonPressed(int button);

private:
	MetaWatchDigitalSimulatorForm* _form;
	QPixmap _pixmap[3];
	QTime _nextFrame;
};

}

#endif // METAWATCHSIMULATOR_H
