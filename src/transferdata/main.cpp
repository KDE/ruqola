/*
 * SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "config-ruqola.h"
#include <QApplication>
#include <QCommandLineParser>

#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>
#include <QDirIterator>
#include <QIcon>

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
                         i18n("Copyright © 2020-2023 Ruqola authors"));

    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    aboutData.setOrganizationDomain(QByteArrayLiteral("kde.org"));
    aboutData.setProductName(QByteArrayLiteral("ruqola"));

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    const int val = app.exec();
    return val;
}
