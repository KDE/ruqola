/*
 * SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "config-ruqola.h"
#include "managerdatapaths.h"
#include "ruqola.h"
#include "ruqolacommandlineoptions.h"
#include "ruqolaglobalconfig.h"
#include <KCrash>
#include <KLocalizedString>
#include <KWindowSystem>
#include <QApplication>
#include <QCommandLineParser>

#include "ruqolamainwindow.h"

#if HAVE_KUSERFEEDBACK
#include "userfeedback/ruqolauserfeedbackprovider.h"
#endif

#include <iostream>

#include <KAboutData>
#include <QDirIterator>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWindow>

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
#include <private/qtx11extras_p.h>
#endif

#include <kdsingleapplication.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("ruqola")));

    KCrash::initialize();

    KDSingleApplication sapp;

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif

    KLocalizedString::setApplicationDomain("ruqola");

    KAboutData aboutData(QStringLiteral("ruqola"),
                         i18n("Ruqola"),
                         QStringLiteral(RUQOLA_VERSION),
                         i18n("Rocket Chat Client"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2020-2023 Ruqola authors"));

    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    aboutData.addAuthor(i18n("Riccardo Iaconelli"), i18n("Original Author"), QStringLiteral("riccardo@kde.org"));

    aboutData.setOrganizationDomain(QByteArrayLiteral("kde.org"));
    aboutData.setProductName(QByteArrayLiteral("ruqola"));
    aboutData.addCredit(i18n("David Faure"), i18n("Bug fixing, delegates etc."), QStringLiteral("faure@kde.org"));
    aboutData.addCredit(i18n("Kevin Funk"), i18n("Bug fixing"), QStringLiteral("kfunk@kde.org"));
    aboutData.addCredit(i18n("Olivier JG"), i18n("Bug fixing"), QStringLiteral("olivier.de.gaalon@kdab.com"));
    aboutData.addCredit(i18n("Milian Wolff"), i18n("Bug fixing"), QStringLiteral("mail@milianw.de"));
    aboutData.addCredit(i18n("Till Adam"), i18n("Windows compile bug fixing"), QStringLiteral("adam@kde.org"));
    aboutData.addCredit(i18n("Shantanu Tushar"), i18n("Bug fixing"), QStringLiteral("shantanu@kde.org"));
    aboutData.addCredit(i18n("Alessandro Ambrosano"), i18n("Bug fixing"), QStringLiteral("alessandro.ambrosano@gmail.com"));
    aboutData.addCredit(i18n("Hannah von Reth"), i18n("Bug fixing for Windows/Mac"), QStringLiteral("vonreth@kde.org"));
    aboutData.addCredit(i18n("Allen Winter"), i18n("Packaging"), QStringLiteral("allen.winter@kdab.com"));
    aboutData.addCredit(i18n("Waqar Ahmed"), i18n("Bug fixing (compile, network crash...)"), QStringLiteral("waqar.17a@gmail.com"));
    aboutData.addCredit(i18n("Nicolas Fella"), i18n("Optimization, fix windows/mac support"), QStringLiteral("nicolas.fella@gmx.de"));

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    ruqolaOptions(&parser);

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
#if HAVE_KUSERFEEDBACK
    if (parser.isSet(QStringLiteral("feedback"))) {
        auto userFeedback = new RuqolaUserFeedbackProvider;
        QTextStream(stdout) << userFeedback->describeDataSources() << '\n';
        delete userFeedback;
        return 0;
    }
#endif

    if (parser.isSet(QStringLiteral("list-accounts"))) {
        const QString configPath = ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
        QDirIterator it(configPath,
                        QStringList() << QStringLiteral("ruqola.conf"),
                        QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                        QDirIterator::Subdirectories);
        std::cout << qPrintable(i18n("The following accounts are available:")) << std::endl;
        while (it.hasNext()) {
            QString result = it.next();
            result.remove(configPath + QLatin1Char('/'));
            result.remove(QStringLiteral("/ruqola.conf"));
            std::cout << "   " << result.toLocal8Bit().data() << std::endl;
        }
        return 0;
    }

    (void)Ruqola::self();

    if (RuqolaGlobalConfig::self()->useCustomFont()) {
        qApp->setFont(RuqolaGlobalConfig::self()->generalFont());
    } else {
#ifdef Q_OS_WIN
        qApp->setFont(QFont(QStringLiteral("Segoe UI Emoji")));
#endif
    }

    if (!sapp.isPrimaryInstance()) {
        QJsonDocument doc;

        QJsonObject obj;
        obj[QLatin1String("working_dir")] = QDir::currentPath();
        obj[QLatin1String("args")] = QJsonArray::fromStringList(app.arguments());

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
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

    auto mw = new RuqolaMainWindow();
    QApplication::connect(&sapp, &KDSingleApplication::messageReceived, &app, [mw](const QByteArray &messageData) {
        QJsonDocument doc = QJsonDocument::fromJson(messageData);
        QJsonObject message = doc.object();

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
        if (KWindowSystem::isPlatformWayland()) {
            qputenv("XDG_ACTIVATION_TOKEN", message[QLatin1String("xdg_activation_token")].toString().toUtf8());
        } else if (KWindowSystem::isPlatformX11()) {
            QX11Info::setNextStartupId(message[QLatin1String("startup_id")].toString().toUtf8());
        }
#endif

        QStringList arguments;

        const auto argumentsJson = message[QLatin1String("arguments")].toArray();
        for (const QJsonValue &val : argumentsJson) {
            arguments << val.toString();
        }

        mw->slotActivateRequested(arguments, message[QLatin1String("working_dir")].toString());
    });

    mw->parseCommandLine(&parser);

    mw->show();
    const int val = app.exec();
    return val;
}
