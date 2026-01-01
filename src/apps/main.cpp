/*
 * SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "config-ruqola.h"
#include "localdatabase/localdatabaseutils.h"
#include "managerdatapaths.h"
#include "ruqola.h"
#include "ruqolacommandlineparser.h"
#include "ruqolaglobalconfig.h"
#include <KCrash>
#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

#include "ruqolamainwindow.h"

#if HAVE_KUSERFEEDBACK
#include "userfeedback/ruqolauserfeedbackprovider.h"
#endif

#include <iostream>

#include <KAboutData>
#include <QDirIterator>
#include <QIcon>

#include <KIconTheme>

#include <KStyleManager>

#if WITH_DBUS
#include <KDBusService>
#else
#include <KWindowSystem>
#include <QJsonArray>
#include <QJsonDocument>
#include <kdsingleapplication.h>
#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS) && !defined(Q_OS_HAIKU)
#include <private/qtx11extras_p.h>
#endif
#endif
using namespace Qt::Literals::StringLiterals;
int main(int argc, char *argv[])
{
    KIconTheme::initTheme();
    const QApplication app(argc, argv);
    app.setWindowIcon(QIcon(u":/ruqola/ruqola.svg"_s));

    KStyleManager::initStyle();
#if !WITH_DBUS
    KDSingleApplication sapp;
#endif
    KLocalizedString::setApplicationDomain("ruqola"_ba);

    KAboutData aboutData(u"ruqola"_s,
                         i18n("Ruqola"),
                         QStringLiteral(RUQOLA_VERSION),
                         i18n("Rocket Chat Client"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2020-2026 Ruqola authors"));

    aboutData.addAuthor(i18nc("@info:credit", "Laurent Montel"), i18n("Maintainer"), u"montel@kde.org"_s);
    aboutData.addAuthor(i18nc("@info:credit", "Riccardo Iaconelli"), i18n("Original Author"), u"riccardo@kde.org"_s);

    aboutData.setOrganizationDomain("kde.org"_ba);
    aboutData.setProductName("ruqola"_ba);
    aboutData.addCredit(i18nc("@info:credit", "David Faure"), i18n("Bug fixing, delegates etc."), u"faure@kde.org"_s);
    aboutData.addCredit(i18nc("@info:credit", "Kevin Funk"), i18n("Bug fixing"), u"kfunk@kde.org"_s);
    aboutData.addCredit(i18nc("@info:credit", "Olivier JG"), i18n("Bug fixing"), u"olivier.de.gaalon@kdab.com"_s);
    aboutData.addCredit(i18nc("@info:credit", "Milian Wolff"), i18n("Bug fixing"), u"mail@milianw.de"_s);
    aboutData.addCredit(i18nc("@info:credit", "Till Adam"), i18n("Windows compile bug fixing"), u"adam@kde.org"_s);
    aboutData.addCredit(i18nc("@info:credit", "Shantanu Tushar"), i18n("Bug fixing"), u"shantanu@kde.org"_s);
    aboutData.addCredit(i18nc("@info:credit", "Alessandro Ambrosano"), i18n("Bug fixing"), u"alessandro.ambrosano@gmail.com"_s);
    aboutData.addCredit(i18nc("@info:credit", "Hannah von Reth"), i18n("Bug fixing for Windows/Mac"), u"vonreth@kde.org"_s);
    aboutData.addCredit(i18nc("@info:credit", "Allen Winter"), i18n("Packaging"), u"allen.winter@kdab.com"_s);
    aboutData.addCredit(i18nc("@info:credit", "Waqar Ahmed"), i18n("Bug fixing (compile, network crash...)"), u"waqar.17a@gmail.com"_s);
    aboutData.addCredit(i18nc("@info:credit", "Nicolas Fella"), i18n("Optimization, fix windows/mac support"), u"nicolas.fella@gmx.de"_s);
    aboutData.addCredit(i18nc("@info:credit", "Daniele E. Domenichelli"), i18n("New feature"), u"daniele.domenichelli@kdab.com"_s);
    aboutData.setProgramLogo(QIcon(u":/ruqola/ruqola.svg"_s));

    KAboutData::setApplicationData(aboutData);

    KCrash::initialize();
    QCommandLineParser parser;
    const RuqolaCommandLineParser commandLineParser(&parser);

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
#if HAVE_KUSERFEEDBACK
    if (parser.isSet(commandLineParser.commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::FeedBack))) {
        auto userFeedback = new RuqolaUserFeedbackProvider;
        QTextStream(stdout) << userFeedback->describeDataSources() << '\n';
        delete userFeedback;
        return 0;
    }
#endif

    if (parser.isSet(commandLineParser.commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::ListAccount))) {
        const QString configPath = ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
        QDirIterator it(configPath,
                        QStringList() << u"ruqola.conf"_s,
                        QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                        QDirIterator::Subdirectories);
        std::cout << qPrintable(i18n("The following accounts are available:")) << '\n';
        while (it.hasNext()) {
            QString result = it.next();
            result.remove(configPath + u'/');
            result.remove(u"/ruqola.conf"_s);
            std::cout << "   " << result.toLocal8Bit().data() << '\n';
        }
        return 0;
    }

    if (parser.isSet(commandLineParser.commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::CleanDatabase))) {
        const QStringList lst = {LocalDatabaseUtils::localDatabasePath()};
        qWarning() << " Delete database : " << lst;
        for (const QString &path : std::as_const(lst)) {
            QDir dir(path);
            if (dir.exists()) {
                if (!dir.removeRecursively()) {
                    qWarning() << "Impossible to remove database from " << dir.absolutePath();
                }
            }
        }

        qWarning() << " Delete database was deleted. Quit";
        return 0;
    }
    (void)Ruqola::self();

    if (parser.isSet(commandLineParser.commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::LoginDdpApi))) {
        Ruqola::self()->setUseRestApiLogin(false);
    }
#if ADD_OFFLINE_SUPPORT
    if (parser.isSet(commandLineParser.commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::Offline))) {
        Ruqola::self()->setOfflineMode(true);
    }
#endif

    if (RuqolaGlobalConfig::self()->useCustomFont()) {
        qApp->setFont(RuqolaGlobalConfig::self()->generalFont());
    } else {
#ifdef Q_OS_WIN
        qApp->setFont(QFont(u"Segoe UI Emoji"_s));
#endif
    }
#if WITH_DBUS
    const KDBusService service(KDBusService::Unique);
#else
    if (!sapp.isPrimaryInstance()) {
        QJsonDocument doc;

        QJsonObject obj;
        obj[QLatin1String("working_dir")] = QDir::currentPath();
        obj[QLatin1String("args")] = QJsonArray::fromStringList(app.arguments());
#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS) && !defined(Q_OS_HAIKU)
        if (KWindowSystem::isPlatformWayland()) {
            obj[QLatin1String("xdg_activation_token")] = qEnvironmentVariable("XDG_ACTIVATION_TOKEN");
        } else if (KWindowSystem::isPlatformX11()) {
            obj[QLatin1String("startup_id")] = QString::fromUtf8(QX11Info::nextStartupId());
        }
#endif

        doc.setObject(obj);

        sapp.sendMessage(doc.toJson(QJsonDocument::Compact));
        return 0;
    }
#endif

    auto mw = new RuqolaMainWindow();
#if WITH_DBUS
    QObject::connect(&service, &KDBusService::activateRequested, mw, &RuqolaMainWindow::slotActivateRequested);
#else
    QApplication::connect(&sapp, &KDSingleApplication::messageReceived, &app, [mw](const QByteArray &messageData) {
        QJsonDocument doc = QJsonDocument::fromJson(messageData);
        QJsonObject message = doc.object();

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS) && !defined(Q_OS_HAIKU)
        if (KWindowSystem::isPlatformWayland()) {
            qputenv("XDG_ACTIVATION_TOKEN", message[QLatin1String("xdg_activation_token")].toString().toUtf8());
        } else if (KWindowSystem::isPlatformX11()) {
            QX11Info::setNextStartupId(message[QLatin1String("startup_id")].toString().toUtf8());
        }
#endif
        QStringList arguments;

        const auto argumentsJson = message[QLatin1String("args")].toArray();
        for (const QJsonValue &val : argumentsJson) {
            arguments << val.toString();
        }

        mw->slotActivateRequested(arguments, message[QLatin1String("working_dir")].toString());
    });
#endif
    mw->parseCommandLine(&parser);

    mw->show();
    const int val = app.exec();
    return val;
}
