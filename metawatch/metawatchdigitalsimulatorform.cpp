#include "metawatchdigitalsimulatorform.h"
#include "ui_metawatchdigitalsimulatorform.h"

using namespace sowatch;

MetaWatchDigitalSimulatorForm::MetaWatchDigitalSimulatorForm(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MetaWatchDigitalSimulatorForm)
{
    ui->setupUi(this);
}

MetaWatchDigitalSimulatorForm::~MetaWatchDigitalSimulatorForm()
{
    delete ui;
}

void MetaWatchDigitalSimulatorForm::refreshScreen(const QPixmap& pixmap)
{
	ui->lblDisplay->setPixmap(pixmap);
	ui->lblDisplay->update();
}

void MetaWatchDigitalSimulatorForm::btnAPressed()
{
	emit buttonPressed(0);
}

void MetaWatchDigitalSimulatorForm::btnAReleased()
{
	emit buttonReleased(0);
}

void MetaWatchDigitalSimulatorForm::btnBPressed()
{
	emit buttonPressed(1);
}

void MetaWatchDigitalSimulatorForm::btnBReleased()
{
	emit buttonReleased(1);
}

void MetaWatchDigitalSimulatorForm::btnCPressed()
{
	emit buttonPressed(2);
}

void MetaWatchDigitalSimulatorForm::btnCReleased()
{
	emit buttonReleased(2);
}

void MetaWatchDigitalSimulatorForm::btnDPressed()
{
	emit buttonPressed(3);
}

void MetaWatchDigitalSimulatorForm::btnDReleased()
{
	emit buttonReleased(3);
}

void MetaWatchDigitalSimulatorForm::btnEPressed()
{
	emit buttonPressed(4);
}

void MetaWatchDigitalSimulatorForm::btnEReleased()
{
	emit buttonReleased(4);
}

void MetaWatchDigitalSimulatorForm::btnFPressed()
{
	emit buttonPressed(5);
}

void MetaWatchDigitalSimulatorForm::btnFReleased()
{
	emit buttonReleased(5);
}