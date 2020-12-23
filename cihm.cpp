#include "cihm.h"
#include "ui_cihm.h"

CIhm::CIhm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CIhm)
{
    ui->setupUi(this);
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &CIhm::on_timer);

    _sgp30.begin();
    ui->statusBar->showMessage("Init du capteur effectuée.");
    QString ch(QString::number(_sgp30.serialnumber[0],16)+QString::number(_sgp30.serialnumber[1],16)+QString::number(_sgp30.serialnumber[2],16));
    ui->statusBar->showMessage(ch);
}

CIhm::~CIhm()
{
    delete ui;
}

void CIhm::on_pbLireMes_clicked()
{
    int itv = ui->spbInterval->value();
    timer->setInterval(itv*1000);  // en secondes
    timer->start();
    ui->pbLireMes->setEnabled(false);
    ui->pbStop->setEnabled(true);
}

void CIhm::on_pbStop_clicked()
{
    timer->stop();
    ui->pbLireMes->setEnabled(true);
    ui->pbStop->setEnabled(false);
}

void CIhm::on_timer()
{
    static int cpt=0;
    QString ch;

    if (! _sgp30.IAQmeasure()) {
       ui->teTVOC->append("failed");
       return;
    }
    ch = QString::number(_sgp30.TVOC)+" ppb";
    ui->teTVOC->append(ch);
    ch = QString::number(_sgp30.eCO2)+" ppm";
    ui->teECO2->append(ch);

    // Signaux utilisés pour la calibration
    if (! _sgp30.IAQmeasureRaw()) {
        ui->teRawH2->append("Raw failed");
        return;
    }
    ch = QString::number(_sgp30.rawH2);
    ui->teRawH2->append(ch);
    ch = QString::number(_sgp30.rawEthanol);
    ui->teRawEthanol->append(ch);

    //sleep(1);

    cpt++;
    if (cpt == 30) {
        cpt = 0;
        uint16_t TVOC_base, eCO2_base;
        if (! _sgp30.getIAQBaseline(&eCO2_base, &TVOC_base)) {
          ui->teBaseECO2->append("Failed baseline");
          return;
        } // if get
        ui->teBaseECO2->append(QString::number(eCO2_base)+" ppm");
        ui->teBaseTvoc->append(QString::number(TVOC_base)+" ppb");
    } // if cpt
}
