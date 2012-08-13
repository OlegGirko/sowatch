#ifndef METAWATCH_H
#define METAWATCH_H

#include <QtCore/QQueue>
#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include <QtConnectivity/QBluetoothAddress>
#include <QtConnectivity/QBluetoothSocket>
#include <QtSystemInfo/QSystemAlignedTimer>
#include <sowatch.h>

using QTM_PREPEND_NAMESPACE(QBluetoothSocket);
using QTM_PREPEND_NAMESPACE(QBluetoothAddress);
using QTM_PREPEND_NAMESPACE(QSystemAlignedTimer);

namespace sowatch
{

class MetaWatchPaintEngine;

class MetaWatch : public Watch
{
    Q_OBJECT

public:
	explicit MetaWatch(ConfigKey *settings, QObject *parent = 0);
	~MetaWatch();

	static const int DelayBetweenMessages = 10;
	static const int DelayBetweenRings = 2500;
	static const int RingLength = 500;

	enum MessageType {
		NoMessage = 0,
		GetDeviceType = 0x01,
		GetDeviceTypeResponse = 0x02,
		GetInformationString = 0x03,
		GetInformationStringResponse = 0x04,
		WriteOledBuffer = 0x10,
		ChangeOledMode = 0x12,
		WriteOledScrollBuffer = 0x13,
		AdvanceWatchHands = 0x20,
		SetVibrateMode = 0x23,
		SetRealTimeClock = 0x26,
		GetRealTimeClock = 0x27,
		GetRealTimeClockResponse = 0x28,
		NvalOperation = 0x30,
		NvalOperationResponse = 0x31,
		StatusChangeEvent = 0x33,
		ButtonEvent = 0x34,
		GeneralPurposePhone = 0x35,
		GeneralPurposeWatch = 0x36,
		WriteLcdBuffer = 0x40,
		ConfigureLcdIdleBufferSize = 0x42,
		UpdateLcdDisplay = 0x43,
		LoadLcdTemplate = 0x44,
		EnableButton = 0x46,
		DisableButton = 0x47,
		BatteryConfiguration = 0x53,
		LowBatteryWarning = 0x54,
		LowBatteryBluetoothOff = 0x55,
		ReadBatteryVoltage = 0x56,
		ReadBatteryVoltageResponse = 0x57,
		ReadLightSensor = 0x58,
		ReadLightSensorResponse = 0x59
	};

	enum NvalValue {
		ReservedNval = 0,
		LinkKey = 0x1,
		IdleBufferConfiguration = 0x2,
		TimeFormat = 0x2009,
		DateFormat = 0x200a
	};

	enum Mode {
		IdleMode = 0,
		ApplicationMode = 1,
		NotificationMode = 2
	};

	enum Button {
		BtnA = 0,
		BtnB,
		BtnC,
		BtnD,
		BtnE,
		BtnF
	};

	enum ButtonPress {
		PressOnly = 0,
		PressAndRelease = 1,
		HoldAndRelease = 2,
		LongHoldAndRelease = 3
	};

	QPaintEngine* paintEngine() const;
	int metric(PaintDeviceMetric metric) const = 0;

	QString model() const;
	QStringList buttons() const;
	bool isConnected() const;
	bool busy() const;

	void setDateTime(const QDateTime& dateTime);
	void queryDateTime();
	QDateTime dateTime() const;

	void queryBatteryLevel();
	int batteryLevel() const;

	void queryCharging();
	bool charging() const;

	void updateNotificationCount(Notification::Type type, int count);
	void updateWeather(WeatherNotification *weather);

	void displayIdleScreen();
	void displayNotification(Notification *notification);
	void displayApplication();

	void vibrate(int msecs);

	Mode currentMode() const;
	Mode paintTargetMode() const;

	QImage* imageFor(Mode mode);
	QRect rectFor(Mode mode);

	virtual void clear(Mode mode, bool black = false) = 0;
	virtual void update(Mode mode, const QList<QRect>& rects = QList<QRect>()) = 0;
	void grabButton(Mode mode, Button button);
	void ungrabButton(Mode mode, Button button);

protected:
	ConfigKey *_settings;

	// Some configurable stuff.
	short _notificationTimeout;
	bool _24hMode : 1;
	bool _dayMonthOrder : 1;

	// Notifications: timers
	QTimer* _idleTimer;
	QTimer* _ringTimer;

	// Buttons
	static const char btnToWatch[8];
	QStringList _buttonNames;

	// Current watch state
	QDateTime _watchTime;
	short _watchBattery;
	short _watchBatteryAverage;
	bool _watchCharging;
	Mode _currentMode;
	Mode _paintMode;

	// Required by QPaintDevice
	mutable MetaWatchPaintEngine* _paintEngine;

	/** The shadow framebuffers for each of the watch modes */
	QImage _image[3];

	// Timers to retry the connection when the watch is not found.
	static const int connectRetryTimesSize = 6;
	static const int connectRetryTimes[connectRetryTimesSize];
	short _connectRetries;
	bool _connected;
	QTimer* _connectTimer;
	QSystemAlignedTimer* _connectAlignedTimer;

	// Connection stuff
	QBluetoothAddress _address;
	QBluetoothSocket* _socket;

	// Base watch protocol stuff
	struct Message {
		MessageType type;
		quint8 options;
		QByteArray data;
		Message(MessageType ntype = NoMessage, QByteArray ndata = QByteArray(), quint8 noptions = 0) :
			type(ntype), options(noptions), data(ndata)
		{ }
	};

	/** The "packets to be sent" asynchronous queue **/
	QQueue<Message> _toSend;
	QTimer* _sendTimer;
	Message _partialReceived;

	/** Pending nvals to be written once the read operation is finished. */
	QMap<NvalValue, int> _nvals;

	static const quint8 bitRevTable[16];
	static const quint16 crcTable[256];
	static quint16 calcCrc(const QByteArray& data, int size);
	static quint16 calcCrc(const Message& msg);

	/** Attempt a connection to the watch. */
	virtual void retryConnect();

	/** Sends a message to the watch. Does not block. */
	virtual void send(const Message& msg);
	/** Sends a message to the watch if a message of the same type is not
	 *  already queued. Does not block.
	 */
	void sendIfNotQueued(const Message& msg);

	static uint nvalSize(NvalValue value);
	void nvalWrite(NvalValue value, int data);

	/* Some functions that wrap sending some watch messages. */
	void setVibrateMode(bool enable, uint on, uint off, uint cycles);
	void updateLcdLine(Mode mode, const QImage& image, int line);
	void updateLcdLines(Mode mode, const QImage& image, int lineA, int lineB);
	void updateLcdLines(Mode mode, const QImage& image, const QVector<bool>& lines);
	void configureLcdIdleSystemArea(bool entireScreen);
	void updateLcdDisplay(Mode mode, bool copy = true);
	void loadLcdTemplate(Mode mode, int templ);
	void enableButton(Mode mode, Button button, ButtonPress press);
	void disableButton(Mode mode, Button button, ButtonPress press);

	void handleMessage(const Message& msg);
	void handleDeviceTypeMessage(const Message& msg);
	void handleRealTimeClockMessage(const Message& msg);
	void handleNvalOperationMessage(const Message& msg);
	void handleStatusChangeMessage(const Message& msg);
	void handleButtonEventMessage(const Message& msg);
	void handleBatteryVoltageMessage(const Message& msg);

	/** To be overriden; should configure a newly connected watch. */
	virtual void handleWatchConnected() = 0;

private slots:
	void settingChanged(const QString& key);
	void socketConnected();
	void socketDisconnected();
	void socketData();
	void socketError(QBluetoothSocket::SocketError error);
	void socketState(QBluetoothSocket::SocketState error);
	void timedReconnect();
	void timedSend();
	void timedRing();

private:
	void realNvalWrite(NvalValue value, int data);
	void realSend(const Message& msg);
	void realReceive(bool block);
};

}

#endif // METAWATCH_H
