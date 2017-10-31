/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "aboutdatatest.h"

#include <QApplication>
#include <QStandardPaths>
#include <KAboutData>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <ruqola.h>
#include <ruqolautils.h>

#include <aboutdata/ruqolaaboutdata.h>
#include <aboutdata/ruqolaaboutdataauthormodel.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    KAboutData aboutData(QStringLiteral("ruqola"),
                         QStringLiteral("Ruqola"),
                         QStringLiteral("1.0.0"),
                         QStringLiteral("Rocket Chat Client"),
                         KAboutLicense::GPL_V2,
                         QStringLiteral("Copyright © 2017 Ruqola authors"));

    aboutData.addAuthor(QStringLiteral("Laurent Montel"), QStringLiteral("Maintainer"), QStringLiteral("montel@kde.org"));
    aboutData.addAuthor(QStringLiteral("Riccardo Iaconelli"), QStringLiteral("Original author"), QStringLiteral("riccardo@kde.org"));
    aboutData.addAuthor(QStringLiteral("Vasudha Mathur"), QStringLiteral("Former core developer"), QStringLiteral("vasudhamathur96@gmail.com"));

    aboutData.setOrganizationDomain(QByteArrayLiteral("kde.org"));
    aboutData.setProductName(QByteArrayLiteral("ruqola"));
    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    qmlRegisterSingletonType<Ruqola>("KDE.Ruqola.Ruqola", 1, 0, "Ruqola", ruqola_singletontype_provider);
    qmlRegisterSingletonType<RuqolaUtils>("KDE.Ruqola.RuqolaUtils", 1, 0, "RuqolaUtils", ruqolautils_singletontype_provider);
    qmlRegisterType<RuqolaAboutData>("KDE.Ruqola.RuqolaAboutData", 1, 0, "RuqolaAboutData");
    qmlRegisterType<RuqolaAboutDataAuthorModel>("KDE.Ruqola.RuqolaAboutDataAuthorModel", 1, 0, "RuqolaAboutDataAuthorModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/aboutdata.qml")));
    return app.exec();
}
