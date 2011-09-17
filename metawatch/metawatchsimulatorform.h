#ifndef METAWATCHSIMULATORFORM_H
#define METAWATCHSIMULATORFORM_H

#include <QWidget>

namespace Ui {
	class MetaWatchSimulatorForm;
}

namespace sowatch {

class MetaWatchSimulatorForm : public QWidget
{
    Q_OBJECT

public:
    explicit MetaWatchSimulatorForm(QWidget *parent = 0);
    ~MetaWatchSimulatorForm();

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
    Ui::MetaWatchSimulatorForm *ui;
};

}

#endif // METAWATCHSIMULATORFORM_H
