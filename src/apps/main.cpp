/*

 * SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
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

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
#include <KDBusService>
#endif

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("ruqola")));

    KCrash::initialize();

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif

    KLocalizedString::setApplicationDomain("ruqola");

    KAboutData aboutData(QStringLiteral("ruqola"),
                         i18n("Ruqola"),
                         QStringLiteral(RUQOLA_VERSION),
                         i18n("Rocket Chat Client"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2020-2022 Ruqola authors"));

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
