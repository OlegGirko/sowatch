#include "metawatchsimulatorform.h"
#include "ui_metawatchsimulatorform.h"

using namespace sowatch;

MetaWatchSimulatorForm::MetaWatchSimulatorForm(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MetaWatchSimulatorForm)
{
    ui->setupUi(this);
}

MetaWatchSimulatorForm::~MetaWatchSimulatorForm()
{
    delete ui;
}

void MetaWatchSimulatorForm::refreshScreen(const QPixmap& pixmap)
{
	ui->lblDisplay->setPixmap(pixmap);
	ui->lblDisplay->update();
}

void MetaWatchSimulatorForm::btnAPressed()
{
	emit buttonPressed(0);
}

void MetaWatchSimulatorForm::btnAReleased()
{
	emit buttonReleased(0);
}

void MetaWatchSimulatorForm::btnBPressed()
{
	emit buttonPressed(1);
}

void MetaWatchSimulatorForm::btnBReleased()
{
	emit buttonReleased(1);
}

void MetaWatchSimulatorForm::btnCPressed()
{
	emit buttonPressed(2);
}

void MetaWatchSimulatorForm::btnCReleased()
{
	emit buttonReleased(2);
}

void MetaWatchSimulatorForm::btnDPressed()
{
	emit buttonPressed(3);
}

void MetaWatchSimulatorForm::btnDReleased()
{
	emit buttonReleased(3);
}

void MetaWatchSimulatorForm::btnEPressed()
{
	emit buttonPressed(4);
}

void MetaWatchSimulatorForm::btnEReleased()
{
	emit buttonReleased(4);
}

void MetaWatchSimulatorForm::btnFPressed()
{
	emit buttonPressed(5);
}

void MetaWatchSimulatorForm::btnFReleased()
{
	emit buttonReleased(5);
}
