/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>
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

#include <QApplication>
#include <QCommandLineParser>
#include <KLocalizedString>
#include <KCrash>
#include "ruqolaregisterengine.h"
#include "config-ruqola.h"
#include "ruqola.h"
#include "managerdatapaths.h"
#include <iostream>

#include <KAboutData>
#include <QIcon>
#include <QDirIterator>

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
#include <KIconTheme>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("ruqola")));

    KCrash::initialize();

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    // call KIconTheme to make sure KIconTheme is linked
    KIconTheme::list();
#endif

    KLocalizedString::setApplicationDomain("ruqola");
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));

    KAboutData aboutData(QStringLiteral("ruqola"),
                         i18n("Ruqola"),
                         QStringLiteral(RUQOLA_VERSION),
                         i18n("Rocket Chat Client"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2017-2019 Ruqola authors"));

    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    aboutData.addAuthor(i18n("Riccardo Iaconelli"), i18n("Original author"), QStringLiteral("riccardo@kde.org"));
    aboutData.addAuthor(i18n("Vasudha Mathur"), i18n("Former core developer"), QStringLiteral("vasudhamathur96@gmail.com"));

    aboutData.setOrganizationDomain(QByteArrayLiteral("kde.org"));
    aboutData.setProductName(QByteArrayLiteral("ruqola"));
    aboutData.addCredit(i18n("David Faure"), i18n("Bug fixing"), QStringLiteral("faure@kde.org"));
    aboutData.addCredit(i18n("Paul Lemire"), i18n("Help for debugging QML"), QStringLiteral("paul.lemire@kdab.com"));
    aboutData.addCredit(i18n("Veluri Mithun"), i18n("Autotest improvement and created some tests apps"), QStringLiteral("velurimithun38@gmail.com"));
    aboutData.addCredit(i18n("Franck Arrecot"), i18n("Fix some QML bugs"), QStringLiteral("franck.arrecot@kdab.com"));
    aboutData.addCredit(i18n("Volker Krause"), i18n("Bug fixing"), QStringLiteral("vkrause@kde.org"));

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    parser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("list-accounts"), i18n("Return lists of accounts")));
    parser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("account"), i18n("Start with specific account"), QStringLiteral("accountname")));

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    if (parser.isSet(QStringLiteral("list-accounts"))) {
        const QString configPath = ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
        QDirIterator it(configPath, QStringList() << QStringLiteral(
                            "ruqola.conf"), QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        std::cout << i18n("The following accounts are available:").toLocal8Bit().data() << std::endl;
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

    RuqolaRegisterEngine ruqolaEngine;

    if (!ruqolaEngine.initialize()) {
        return -1;
    }

    return app.exec();
}
