/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "../gitlabauthenticationjob.h"
#include <QCommandLineParser>
#include <QGuiApplication>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QCommandLineParser parser;

    const QCommandLineOption clientId(QStringList() << QStringLiteral("i") << QStringLiteral("client-id"),
                                      QStringLiteral("Specifies the application client id"),
                                      QStringLiteral("client_id"));

    parser.addOptions({clientId});
    parser.process(app);

    if (parser.isSet(clientId)) {
        const QString value = parser.value(clientId);
        auto job = new GitLabAuthenticationJob();
        GitLabAuthenticationJob::GitLabInfo info;
        // Make url
        info.url = QStringLiteral("foo");
        info.clientId = value;
        job->setGitLabInfo(info);
        job->start();
        // TODO
        app.exec();
    } else {
        parser.showHelp();
    }
    return 0;
}
