#include "hnapp.h"
#include "hngui-qt.h"
#include "hngui.h"
#include "HNDefine.h"

HNApp::HNApp(int &argc, char **argv) : QApplication(argc, argv)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));

    QApplication::setOrganizationName(VER_COMPANYNAME_STR);
    QApplication::setOrganizationDomain(VER_COMPANYDOMAIN_STR);  // 专为Mac OS X 准备的
    QApplication::setApplicationName(VER_PRODUCTNAME_STR);
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, CONFIG_PATH);
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, CONFIG_PATH);

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

    //打开方法数据库
    managerDB = newDatabaseConn();
    setDatabaseName(managerDB, DB_MANAGER);


#if 0
    QFile styleFile("://HNWidgets.qss");
    styleFile.open(QIODevice::ReadOnly);
    QString styleString(styleFile.readAll());;
    setStyleSheet(styleString);
    styleFile.close();
    //设置所有默认颜色
    //setPalette(QPalette(QColor("#F0F0F0")));
#endif

#ifdef __MIPS_LINUX__
    //HNInput::Instance()->Init("min", "control", "hanon", 14, 14);
#endif

    HNPluginWatcher* watcher = HNPluginWatcher::Instance();
    QObject::connect(watcher, SIGNAL(storageChanged(int)), this, SLOT(slotUPanAutoRun(int)));

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    //HNClient
    HNClientInstance(this);
    //HNEthManager
    HNEthManager::Instance(this);
    //HNServer
    //HNPeerPort
    //HNSerialPort
}

HNApp::~HNApp() {}

void HNApp::setLanguage()
{
    QSettings setting;  //  公司或组织名  // 应用程序名
    QString qm;
    qm = setting.value("Language").toInt() ?
                "://HNLang/en_US.qm" : "://HNLang/zh_CN.qm";

    language->load(qm);
    pline() << "currentLanguage" << qm;
    installTranslator(language);
}

void HNApp::slotUPanAutoRun(int status)
{
    if(HNPluginWatcher::E_ADD == status)
    {
        QString mP = HNPluginWatcher::Instance()->upanMountPath();
        QString auth = QString("chmod +x %1/autorun.sh").arg(mP);
        QString app = QString("%1/autorun.sh").arg(mP);
        QFile file(app);
        if(file.exists())
            if(QDialog::Rejected == HNMsgBox::question(0, tr("Some app want to run in u disk!accepted?")))
                return;
        system(auth.toAscii().constData());
        QProcess* p = new QProcess(this);
        p->setWorkingDirectory(mP);
        p->start(app);
    }
}
