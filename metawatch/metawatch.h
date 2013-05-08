#ifndef METAWATCH_H
#define METAWATCH_H

#include <QtCore/QQueue>
#include <QtCore/QTimer>
#include <QtConnectivity/QBluetoothAddress>
#include <QtConnectivity/QBluetoothSocket>
#include <QtConnectivity/QBluetoothLocalDevice>
#include <QtSystemInfo/QSystemAlignedTimer>
#include <sowatch.h>
#include <sowatchbt.h>

namespace sowatch
{

using QTM_PREPEND_NAMESPACE(QBluetoothSocket);
using QTM_PREPEND_NAMESPACE(QBluetoothAddress);
using QTM_PREPEND_NAMESPACE(QSystemAlignedTimer);
using QTM_PREPEND_NAMESPACE(QBluetoothLocalDevice);

class MetaWatchPaintEngine;

class MetaWatch : public BluetoothWatch
{
    Q_OBJECT

public:
	explicit MetaWatch(ConfigKey *settings, QObject *parent = 0);
	~MetaWatch();

	static const int DelayBetweenMessages = 5;

	static const int VibrateLength = 500;
	static const int DelayBetweenRings = 2500;
	static const int RingLength = 250;

	enum MessageType {
		NoMessage = 0,
		GetDeviceType = 0x01,
		GetDeviceTypeResponse = 0x02,
		GetInformationString = 0x03,
		GetInformationStringResponse = 0x04,
		DiagnosticLoopback = 0x05,
		WriteOledBuffer = 0x10,
		ChangeOledMode = 0x12,
		WriteOledScrollBuffer = 0x13,
		AdvanceWatchHands = 0x20,
		SetVibrateMode = 0x23,
		SetRealTimeClock = 0x26,
		GetRealTimeClock = 0x27,
		GetRealTimeClockResponse = 0x28,
		PropertyOperation = 0x30,
		PropertyOperationResponse = 0x31,
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
		ReadLightSensorResponse = 0x59,
		ChangeMode = 0xa6
	};

	enum Mode {
		IdleMode = 0,
		ApplicationMode = 1,
		NotificationMode = 2,
		MusicMode = 3 // Please note that the music mode is currently not used
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

	Mode currentMode() const;
	Mode paintTargetMode() const;

	QImage* imageFor(Mode mode);
	QRect rectFor(Mode mode);

	virtual void clear(Mode mode, bool black = false) = 0;
	virtual void update(Mode mode, const QList<QRect>& rects = QList<QRect>()) = 0;
	void grabButton(Mode mode, Button button);
	void ungrabButton(Mode mode, Button button);

protected:
	// Base watch protocol stuff
	struct Message {
		MessageType type;
		quint8 options;
		QByteArray data;
		Message(MessageType ntype = NoMessage, QByteArray ndata = QByteArray(), quint8 noptions = 0) :
			type(ntype), options(noptions), data(ndata)
		{ }
	};

protected:
	ConfigKey *_settings;

	// Some configurable stuff.
	short _notificationTimeout;
	bool _24hMode : 1;
	bool _dayMonthOrder : 1;
	bool _showSeconds : 1;
	bool _separationLines : 1;
	bool _autoBacklight : 1;

	// Notifications: timers
	QTimer* _idleTimer;
	QTimer* _ringTimer;

	// Buttons
	static const char btnToWatch[8];
	QStringList _buttonNames;

	// Current watch state
	/** The watch's currently displayed date & time. */
	QDateTime _watchTime;
	/** The current watch battery level (from 0 to 100). */
	short _watchBattery;
	/** Whether the watch is currently charging. */
	bool _watchCharging;
	/** The watch's current display mode. */
	Mode _currentMode;
	/** The mode where paint operations done using QPaintDevice go into */
	Mode _paintMode;

	// Required by QPaintDevice
	mutable MetaWatchPaintEngine* _paintEngine;

	/** The framebuffers for each of the watch modes */
	QImage _image[3];

	/** The "packets to be sent" asynchronous queue **/
	QQueue<Message> _toSend;
	QTimer* _sendTimer;
	Message _partialReceived;

	// Watch connect/disconnect handling
	void setupBluetoothWatch();
	void desetupBluetoothWatch();

	// Message passing
	/** Used to calculate CRC fields in message header */
	static const quint8 bitRevTable[16];
	static const quint16 crcTable[256];
	static quint16 calcCrc(const QByteArray& data, int size);
	static quint16 calcCrc(const Message& msg);

	/** Sends a message to the watch. Does not block. */
	virtual void send(const Message& msg);
	/** Sends a message to the watch if a message of the same type is not
	 *  already queued. Does not block.
	 */
	void sendIfNotQueued(const Message& msg);

	/* Some functions that wrap sending some watch messages. */
	void updateWatchProperties();
	void setVibrateMode(bool enable, uint on, uint off, uint cycles);
	void updateLcdLine(Mode mode, const QImage& image, int line);
	void updateLcdLines(Mode mode, const QImage& image, int lineA, int lineB);
	void updateLcdLines(Mode mode, const QImage& image, const QVector<bool>& lines);
	void configureLcdIdleSystemArea(bool entireScreen);
	void updateLcdDisplay(Mode mode, int startRow = 0, int numRows = 0);
	void loadLcdTemplate(Mode mode, int templ);
	void changeMode(Mode mode);
	void enableButton(Mode mode, Button button, ButtonPress press);
	void disableButton(Mode mode, Button button, ButtonPress press);

	void handleMessage(const Message& msg);
	void handleDeviceTypeMessage(const Message& msg);
	void handleRealTimeClockMessage(const Message& msg);
	void handlePropertyOperationMessage(const Message& msg);
	void handleStatusChangeMessage(const Message& msg);
	void handleButtonEventMessage(const Message& msg);
	void handleBatteryVoltageMessage(const Message& msg);

private slots:
	void settingChanged(const QString& key);
	void dataReceived();
	void timedSend();
	void timedRing();

private:
	void realSend(const Message& msg);
	void realReceive(bool block);
};

}

#endif // METAWATCH_H
