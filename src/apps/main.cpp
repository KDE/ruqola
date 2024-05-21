/*
 * SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
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

#define HAVE_KICONTHEME __has_include(<KIconTheme>)
#if HAVE_KICONTHEME
#include <KIconTheme>
#endif

#define HAVE_STYLE_MANAGER __has_include(<KStyleManager>)
#if HAVE_STYLE_MANAGER
#include <KStyleManager>
#endif

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
#include <KDBusService>
#endif

int main(int argc, char *argv[])
{
#if HAVE_KICONTHEME && (KICONTHEMES_VERSION >= QT_VERSION_CHECK(6, 3, 0))
    KIconTheme::initTheme();
#endif
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("ruqola")));

    KCrash::initialize();
#if HAVE_STYLE_MANAGER
    KStyleManager::initStyle();
#else // !HAVE_STYLE_MANAGER
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif
#endif
    KLocalizedString::setApplicationDomain(QByteArrayLiteral("ruqola"));

    KAboutData aboutData(QStringLiteral("ruqola"),
                         i18n("Ruqola"),
                         QStringLiteral(RUQOLA_VERSION),
                         i18n("Rocket Chat Client"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2020-2024 Ruqola authors"));

    aboutData.addAuthor(i18nc("@info:credit", "Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    aboutData.addAuthor(i18nc("@info:credit", "Riccardo Iaconelli"), i18n("Original Author"), QStringLiteral("riccardo@kde.org"));

    aboutData.setOrganizationDomain(QByteArrayLiteral("kde.org"));
    aboutData.setProductName(QByteArrayLiteral("ruqola"));
    aboutData.addCredit(i18nc("@info:credit", "David Faure"), i18n("Bug fixing, delegates etc."), QStringLiteral("faure@kde.org"));
    aboutData.addCredit(i18nc("@info:credit", "Kevin Funk"), i18n("Bug fixing"), QStringLiteral("kfunk@kde.org"));
    aboutData.addCredit(i18nc("@info:credit", "Olivier JG"), i18n("Bug fixing"), QStringLiteral("olivier.de.gaalon@kdab.com"));
    aboutData.addCredit(i18nc("@info:credit", "Milian Wolff"), i18n("Bug fixing"), QStringLiteral("mail@milianw.de"));
    aboutData.addCredit(i18nc("@info:credit", "Till Adam"), i18n("Windows compile bug fixing"), QStringLiteral("adam@kde.org"));
    aboutData.addCredit(i18nc("@info:credit", "Shantanu Tushar"), i18n("Bug fixing"), QStringLiteral("shantanu@kde.org"));
    aboutData.addCredit(i18nc("@info:credit", "Alessandro Ambrosano"), i18n("Bug fixing"), QStringLiteral("alessandro.ambrosano@gmail.com"));
    aboutData.addCredit(i18nc("@info:credit", "Hannah von Reth"), i18n("Bug fixing for Windows/Mac"), QStringLiteral("vonreth@kde.org"));
    aboutData.addCredit(i18nc("@info:credit", "Allen Winter"), i18n("Packaging"), QStringLiteral("allen.winter@kdab.com"));
    aboutData.addCredit(i18nc("@info:credit", "Waqar Ahmed"), i18n("Bug fixing (compile, network crash...)"), QStringLiteral("waqar.17a@gmail.com"));
    aboutData.addCredit(i18nc("@info:credit", "Nicolas Fella"), i18n("Optimization, fix windows/mac support"), QStringLiteral("nicolas.fella@gmx.de"));

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    RuqolaCommandLineParser commandLineParser(&parser);

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
#if HAVE_KUSERFEEDBACK
    if (parser.isSet(commandLineParser.commandLineFromEnum(RuqolaCommandLineParser::FeedBack))) {
        auto userFeedback = new RuqolaUserFeedbackProvider;
        QTextStream(stdout) << userFeedback->describeDataSources() << '\n';
        delete userFeedback;
        return 0;
    }
#endif

    if (parser.isSet(commandLineParser.commandLineFromEnum(RuqolaCommandLineParser::ListAccount))) {
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

    if (parser.isSet(commandLineParser.commandLineFromEnum(RuqolaCommandLineParser::LoginDdpApi))) {
        Ruqola::self()->setUseRestApiLogin(false);
    }

    if (RuqolaGlobalConfig::self()->useCustomFont()) {
        qApp->setFont(RuqolaGlobalConfig::self()->generalFont());
    } else {
#ifdef Q_OS_WIN
        qApp->setFont(QFont(QStringLiteral("Segoe UI Emoji")));
#endif
    }

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
    // TODO Port to something like KDSingleApplication
    KDBusService service(KDBusService::Unique);
#endif
    auto mw = new RuqolaMainWindow();
#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
    QObject::connect(&service, &KDBusService::activateRequested, mw, &RuqolaMainWindow::slotActivateRequested);
#endif
    mw->parseCommandLine(&parser);

    mw->show();
    const int val = app.exec();
    return val;
}
