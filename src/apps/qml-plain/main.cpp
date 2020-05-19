/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>
 * Copyright 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
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

#include "applicationengine.h"
#include "config-ruqola.h"
#include "managerdatapaths.h"
#include "ruqola.h"
#include "ruqolaregisterengine.h"
#include <KCrash>
#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <iostream>

#include <KAboutData>
#include <QDirIterator>
#include <QIcon>
#include <QQuickWindow>

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
#include <KIconTheme>
#endif

int Q_DECL_EXPORT main(int argc, char *argv[])
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

    KAboutData aboutData(
        QStringLiteral("ruqolaqmlplain"), i18n("Ruqolaqmlplain"), QStringLiteral(RUQOLA_VERSION),
        i18n("QML Rocket Chat Client"), KAboutLicense::GPL_V2,
        i18n("Copyright ┬⌐ 2017-2020 Ruqola authors"));

    aboutData.addAuthor(
        i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    aboutData.addAuthor(
        i18n("Riccardo Iaconelli"), i18n("Original author"), QStringLiteral("riccardo@kde.org"));
    aboutData.addAuthor(
        i18n("Vasudha Mathur"), i18n("Former core developer"),
        QStringLiteral("vasudhamathur96@gmail.com"));

    aboutData.setOrganizationDomain(QByteArrayLiteral("kde.org"));
    aboutData.setProductName(QByteArrayLiteral("ruqola"));
    aboutData.addCredit(i18n("David Faure"), i18n("Bug fixing"), QStringLiteral("faure@kde.org"));
    aboutData.addCredit(
        i18n("Paul Lemire"), i18n("Help for debugging QML"),
        QStringLiteral("paul.lemire@kdab.com"));
    aboutData.addCredit(
        i18n("Veluri Mithun"), i18n("Autotest improvement and created some tests apps"),
        QStringLiteral("velurimithun38@gmail.com"));
    aboutData.addCredit(
        i18n("Franck Arrecot"), i18n("Fix some QML bugs"),
        QStringLiteral("franck.arrecot@kdab.com"));
    aboutData.addCredit(
        i18n("Volker Krause"), i18n("Bug fixing"), QStringLiteral("vkrause@kde.org"));

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    parser.addOption(QCommandLineOption(
        QStringList() << QStringLiteral("list-accounts"), i18n("Return lists of accounts")));
    parser.addOption(QCommandLineOption(
        QStringList() << QStringLiteral("account"), i18n("Start with specific account"),
        QStringLiteral("accountname")));

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    if (parser.isSet(QStringLiteral("list-accounts"))) {
        const QString configPath =
            ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
        QDirIterator it(
            configPath, QStringList() << QStringLiteral("ruqola.conf"),
            QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
            QDirIterator::Subdirectories);
        std::cout << i18n("The following accounts are available:").toLocal8Bit().data()
                  << std::endl;
        while (it.hasNext()) {
            QString result = it.next();
            result.remove(configPath + QLatin1Char('/'));
            result.remove(QStringLiteral("/ruqola.conf"));
            std::cout << "   " << result.toLocal8Bit().data() << std::endl;
        }
        return 0;
    }

    // QString loadAccount;
    // if (parser.isSet(QStringLiteral("account"))) {
    //     loadAccount = parser.value(QStringLiteral("account"));
    // }
    // (void)Ruqola::self();
    // if (!loadAccount.isEmpty()) {
    //     Ruqola::self()->setCurrentAccount(loadAccount);
    // }

    // For desktop we'd like to use native text rendering
#ifndef Q_OS_ANDROID
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
#endif

    RuqolaQmlPlain::ApplicationEngine ruqolaEngine;

    if (!ruqolaEngine.initialize()) {
        return -1;
    }

    return app.exec();
}
