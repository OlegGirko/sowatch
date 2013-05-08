#ifndef LIVEVIEW_H
#define LIVEVIEW_H

#include <sowatch.h>
#include <sowatchbt.h>

namespace sowatch
{

class LiveView : public BluetoothWatch
{
	Q_OBJECT

public:
	explicit LiveView(ConfigKey *settings, QObject *parent = 0);
	~LiveView();

	QPaintEngine* paintEngine() const;
	int metric(PaintDeviceMetric metric) const;

	QString model() const;
	QStringList buttons() const;

	bool busy() const;

	void setDateTime(const QDateTime& dateTime);
	void queryDateTime();
	QDateTime dateTime() const;

	void queryBatteryLevel();
	int batteryLevel() const;

	void queryCharging();
	bool charging() const;

	void displayIdleScreen();
	void displayNotification(Notification *notification);
	void displayApplication();

	void vibrate(int msecs);

protected:
	static const int DelayBetweenMessages = 5;

	enum MessageType {
		NoMessage = 0,
		GetDisplayProperties = 1,
		GetDisplayPropertiesResponse = 2,
		StandBy = 7,
		StandByResponse = 8,
		EnableLed = 40,
		EnableLedResponse = 41,
		Ack = 44,
		GetSoftwareVersion = 68,
		GetSoftwareVersionResponse = 69
	};

	struct Message {
		MessageType type;
		QByteArray data;
		Message(MessageType ntype = NoMessage, QByteArray ndata = QByteArray()) :
			type(ntype), data(ndata)
		{ }
	};

	void setupBluetoothWatch();
	void desetupBluetoothWatch();

	void send(const Message& msg);

	void updateDisplayProperties();
	void updateSoftwareVersion();
	void enableLed();

	void handleMessage(const Message& msg);
	void handleDisplayProperties(const Message& msg);

private slots:
	void handleDataReceived();
	void handleSendTimerTick();

private:
	ConfigKey *_settings;

	/** Message outbox queue. */
	QQueue<Message> _sendingMsgs;
	QTimer* _sendTimer;
	/** Incomplete message that is being received. */
	Message _receivingMsg;
};

}

#endif // LIVEVIEW_H
