/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
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
#include <QIcon>

#include <KAboutData>

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
#include <KIconTheme>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("ruqola")));

    KCrash::initialize();

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    // call KIconTheme to make sure KIconTheme is linked
    KIconTheme::list();
#endif

    KLocalizedString::setApplicationDomain("ruqola");
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));

    KAboutData aboutData(QStringLiteral("ruqola"),
                         i18n("Ruqola"),
                         QStringLiteral(RUQOLA_VERSION),
                         i18n("Rocket Chat Client"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2017-2018 Ruqola authors"));

    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    aboutData.addAuthor(i18n("Riccardo Iaconelli"), i18n("Original author"), QStringLiteral("riccardo@kde.org"));
    aboutData.addAuthor(i18n("Vasudha Mathur"), i18n("Former core developer"), QStringLiteral("vasudhamathur96@gmail.com"));

    aboutData.setOrganizationDomain(QByteArrayLiteral("kde.org"));
    aboutData.setProductName(QByteArrayLiteral("ruqola"));
    aboutData.addCredit(i18n("Paul Lemire"), i18n("Help for debugging QML"), QStringLiteral("paul.lemire@kdab.com"));
    aboutData.addCredit(i18n("Veluri Mithun"), i18n("Autotest Improvment and created some tests apps"), QStringLiteral("velurimithun38@gmail.com"));

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    RuqolaRegisterEngine ruqolaEngine;

    if (!ruqolaEngine.initialize()) {
        return -1;
    }

    return app.exec();
}
