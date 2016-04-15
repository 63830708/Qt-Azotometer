#ifndef QREPORTVIEWDIALOG_H
#define QREPORTVIEWDIALOG_H

#include "hndialog.h"
#include "hngui-qt.h"
#include "hnsqltablemodel.h"

namespace Ui {
class QReportViewDialog;
}

class HNPrintPreviewDialog : public HNDialog
{
    Q_OBJECT

public:
    explicit HNPrintPreviewDialog(QWidget *parent = 0);
    ~HNPrintPreviewDialog();

    //SerialNo;
    QString newReport(QString library, QString method);
    void endReport(QString serialNo, QString Exception,
                      QString stopReason, QByteArray& plot);
    void initAll(QString name);
    void printToPdf();
    void print();

private:
    Ui::QReportViewDialog *ui;
    QSqlDatabase m_db;
};

#endif // QREPORTVIEWDIALOG_H
