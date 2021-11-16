/*

 * Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "config-ruqola.h"
#include "managerdatapaths.h"
#include "ruqola.h"
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
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("ruqola")));

    KCrash::initialize();

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif

    KLocalizedString::setApplicationDomain("ruqola");
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));

    KAboutData aboutData(QStringLiteral("ruqola"),
                         i18n("Ruqola"),
                         QStringLiteral(RUQOLA_VERSION),
                         i18n("Rocket Chat Client"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2020-2021 Ruqola authors"));

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

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("list-accounts"), i18n("Return lists of accounts")));
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("account"), i18n("Start with specific account"), QStringLiteral("accountname")));

#if HAVE_KUSERFEEDBACK
    parser.addOption(QCommandLineOption(QStringLiteral("feedback"), i18n("Lists the available options for user feedback")));
#endif

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
#if HAVE_KUSERFEEDBACK
    if (parser.isSet(QStringLiteral("feedback"))) {
        auto userFeedback = new RuqolaUserFeedbackProvider(nullptr);
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

    QString loadAccount;
    if (parser.isSet(QStringLiteral("account"))) {
        loadAccount = parser.value(QStringLiteral("account"));
    }
    (void)Ruqola::self();

    if (!loadAccount.isEmpty()) {
        Ruqola::self()->setCurrentAccount(loadAccount);
    }

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
    // TODO Port to something like KDSingleApplication
    KDBusService service(KDBusService::Unique);
#endif

    if (RuqolaGlobalConfig::self()->useCustomFont()) {
        qApp->setFont(RuqolaGlobalConfig::self()->generalFont());
    } else {
#ifdef Q_OS_WIN
        qApp->setFont(QFont(QStringLiteral("Segoe UI Emoji")));
#endif
    }

    auto mw = new RuqolaMainWindow();
    mw->show();
    const int val = app.exec();
    return val;
}
