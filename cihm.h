#ifndef CIHM_H
#define CIHM_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTimer>
#include <QSpinBox>

#include "cadafruit_sgp30.h"

namespace Ui {
class CIhm;
}

class CIhm : public QMainWindow
{
    Q_OBJECT

public:
    explicit CIhm(QWidget *parent = nullptr);
    ~CIhm();

private slots:
    void on_pbLireMes_clicked();
    void on_timer();
    void on_pbStop_clicked();

private:
    Ui::CIhm *ui;
    QTimer *timer;
    CAdafruit_SGP30 _sgp30;  // composition
};

#endif // CIHM_H
