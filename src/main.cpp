#include <QtWidgets/QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QProcess>
#include <Version.h>
#include <QQmlContext>
#include "helper/SettingsHelper.h"
#include "helper/LinkHelper.h"
#include "helper/deviceLink.h"
#include "helper/LogHelper.h"

#ifdef FLUENTUI_BUILD_STATIC_LIB
#  if (QT_VERSION > QT_VERSION_CHECK(6, 2, 0))
Q_IMPORT_QML_PLUGIN(FluentUIPlugin)
#  endif
#  include <FluentUI.h>
#endif



int main(int argc, char *argv[])
{
#ifdef WIN32
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
#else
    qputenv("QT_QUICK_CONTROLS_STYLE", "Default");
#endif
#ifdef Q_OS_LINUX
    // fix bug UOSv20 does not print logs
    qputenv("QT_LOGGING_RULES", "");
    // fix bug UOSv20 v-sync does not work
    qputenv("QSG_RENDER_LOOP", "basic");
#endif
    QApplication::setOrganizationName("mizhijitou");
    QApplication::setApplicationName("JiTou-Unlock");
    QApplication::setApplicationDisplayName("JiTou-Unlock");
    QApplication::setApplicationVersion(APPLICATION_VERSION);
    SettingsHelper::getInstance()->init(argv);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#  if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#  endif
#endif
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "JiTou-Unlock-NextVersion-master_" + QLocale(locale).name();
        if (translator.load("./i18n/"+ baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    QQmlApplicationEngine engine;
    DeviceInfoModel DeviceInfoModel;

    engine.rootContext()->setContextProperty("SettingsHelper", SettingsHelper::getInstance());
    engine.rootContext()->setContextProperty("LinkHelper", LinkHelper::getInstance());
    engine.rootContext()->setContextProperty("phoneInfoModel",&DeviceInfoModel);
    engine.rootContext()->setContextProperty("CurrentInfo", CurrentInfo::getInstance());
    engine.rootContext()->setContextProperty("LogHelper", LogHelper::getInstance());
    engine.rootContext()->setContextProperty("currentWorkspace", currentWorkspace::getInstance());
    const QUrl url(QStringLiteral("qrc:/App.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
