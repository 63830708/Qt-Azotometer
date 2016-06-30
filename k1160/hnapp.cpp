#include "hnapp.h"
#include "hngui-qt.h"
#include "hngui.h"
#include "HNDefine.h"
#include "HNInput.h"
#include "hnmsgbox.h"
#include "qinputtool.h"

HNApp::HNApp(int &argc, char **argv) : QApplication(argc, argv)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QApplication::setOrganizationName(VER_COMPANYNAME_STR);
    QApplication::setOrganizationDomain(VER_COMPANYDOMAIN_STR);  // 专为Mac OS X 准备的
    QApplication::setApplicationName(VER_PRODUCTNAME_STR);
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, CONFIG_PATH);
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, CONFIG_PATH);

    system("rm -f /tmp/LCK..ttyS*");

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    //打印失真与否与此处无关
    QApplication::setGraphicsSystem("raster");
#endif

#ifdef __MIPS_LINUX__
    //QApplication::setOverrideCursor(Qt::ArrowCursor);
    QWSServer::setCursorVisible(false);
#endif

#ifdef __MIPS_LINUX__
    QFontDatabase db;

#if 0
    int heitiFontID = db.addApplicationFont("/usr/lib/fonts/heiti.ttf");
    QString heiti = db.applicationFontFamilies ( heitiFontID ).at(0);
    pline() << heiti;
#else
    int wenquanyiFontID = db.addApplicationFont("/usr/lib/fonts/wenquanyi.ttf");
    QString wenquanyi = db.applicationFontFamilies ( wenquanyiFontID ).at(0);
    pline() << wenquanyi;
#endif

    QFont font(wenquanyi, 11);
    QApplication::setFont(font);
#endif

    pline() << qApp->applicationDirPath();

    language = new QTranslator(this);
    setLanguage();

#if 0
    //打开方法数据库
    managerDB = newDatabaseConn();
    setDatabaseName(managerDB, DB_MANAGER);
#endif

#if 1
    //Could not parse application stylesheet 告警 可以忽略
    //关于选中项的颜色，暂且按照默认，后来更改整体UI颜色和效果
    //可以实现橙色一行选中
    //肯定也能实现表头透明和Hanon效果。
    QFile styleFile("./skin/default.qss");
    styleFile.open(QIODevice::ReadOnly);
    QString styleString(styleFile.readAll());;
    styleFile.close();
    setStyleSheet(styleString);
    //设置所有默认颜色
    //setPalette(QPalette(QColor("#F0F0F0")));
#endif

#ifdef __MIPS_LINUX__
    //HNInput::Instance()->Init("min", "control", "hanon", 14, 14);
    QInputTool::Instance()->Init("control", "brown", 10, 10);
    QInputTool::Instance()->hide();
#endif

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    QObject::connect(HNPluginWatcher::Instance(), SIGNAL(storageChanged(int)),
                     this, SLOT(slotUPanAutoRun(int)));
    //HNClient
    HNClientInstance(this);
    //HNEthManager
    HNEthManager::Instance(this);
    //HNServer
    //HNPeerPort
    HNPeerPortInstance(this);
    //HNSerialPort
    HNSerialPortInstance(this);
}

HNApp::~HNApp() {}

void HNApp::setTheme()
{
    QSettings setting;
    QString themeName = setting.value(QString("/%1/Theme").arg(gUserName)).toString();
    if(themeName.isEmpty())
        themeName = "blue";

    //pline() << QDateTime::currentDateTime();
    QFile styleFile(QString(":/theme/%1/theme.qss").arg(themeName));
    styleFile.open(QIODevice::ReadOnly);
    QString styleString(styleFile.readAll());;
    setStyleSheet(styleString);
    styleFile.close();
    //pline() << QDateTime::currentDateTime();
}

void HNApp::setLanguage()
{
    QSettings setting;  //  公司或组织名  // 应用程序名
    QString qm;
    qm = setting.value("Language").toInt() ?
                "./lang/en_US.qm" : "./lang/zh_CN.qm";

    language->load(qm);
    pline() << "currentLanguage" << qm;
    installTranslator(language);
}

void HNApp::slotUPanAutoRun(int status)
{
    if(HNPluginWatcher::E_ADD == status)
    {
        QString mP = HNPluginWatcher::Instance()->upanMountPath();
        QString app = QString("%1/autorun.sh").arg(mP);
        QFile file(app);
        if(file.exists())
            if(QDialog::Rejected == HNMsgBox::question(0, tr("Some app want to run in u disk!accepted?")))
                return;
        QProcess* p = new QProcess(this);
        p->setWorkingDirectory(mP);
        p->start(app);
    }
}
