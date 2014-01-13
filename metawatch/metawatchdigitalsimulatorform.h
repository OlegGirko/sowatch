#ifndef METAWATCHSIMULATORFORM_H
#define METAWATCHSIMULATORFORM_H

#include <QMainWindow>

namespace Ui {
	class MetaWatchDigitalSimulatorForm;
}

namespace sowatch {

class MetaWatchDigitalSimulatorForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit MetaWatchDigitalSimulatorForm(QWidget *parent = 0);
    ~MetaWatchDigitalSimulatorForm();

	void refreshScreen(const QPixmap& screen);

signals:
	void buttonPressed(int button);
	void buttonReleased(int button);

private slots:
	void btnAPressed();
	void btnAReleased();
	void btnBPressed();
	void btnBReleased();
	void btnCPressed();
	void btnCReleased();
	void btnDPressed();
	void btnDReleased();
	void btnEPressed();
	void btnEReleased();
	void btnFPressed();
	void btnFReleased();

	void on_actionCaptureScreen_triggered();

	void on_actionQuit_triggered();

private:
    Ui::MetaWatchDigitalSimulatorForm *ui;
};

}

#endif // METAWATCHSIMULATORFORM_H
