#include "hnwidget.h"
#include "ui_hnwidget.h"
#include <QStylePainter>

HNWidget::HNWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HNWidget)
{
    ui->setupUi(this);
}

HNWidget::~HNWidget()
{
    delete ui;
}

void HNWidget::setPixmap(QString pic)
{
    m_pic = pic;
}

void HNWidget::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    if(m_pic.isEmpty())
        return;

#if 0
    //会产生label的效果，左右按照rect长，但是不缩放形状
    QImage image(m_pic);
    p.drawItemPixmap(rect(), Qt::AlignCenter,
                     QPixmap::fromImage(image.copy(rect()).scaledToHeight(rect().height())));
#else
    p.drawItemPixmap(rect(), Qt::AlignCenter, QIcon(m_pic).pixmap(rect().size(), QIcon::Normal, QIcon::On));
#endif
}
