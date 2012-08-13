#ifndef METAWATCHSIMULATORFORM_H
#define METAWATCHSIMULATORFORM_H

#include <QWidget>

namespace Ui {
	class MetaWatchDigitalSimulatorForm;
}

namespace sowatch {

class MetaWatchDigitalSimulatorForm : public QWidget
{
    Q_OBJECT

public:
    explicit MetaWatchDigitalSimulatorForm(QWidget *parent = 0);
    ~MetaWatchDigitalSimulatorForm();

	void refreshScreen(const QPixmap& screen);

signals:
	void buttonPressed(int button);
	void buttonReleased(int button);

protected slots:
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

private:
    Ui::MetaWatchDigitalSimulatorForm *ui;
};

}

#endif // METAWATCHSIMULATORFORM_H
