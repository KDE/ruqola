/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "../gitlabauthenticationjob.h"
using namespace Qt::Literals::StringLiterals;

#include <QCommandLineParser>
#include <QGuiApplication>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QCommandLineParser parser;

    const QCommandLineOption clientId(QStringList() << u"i"_s << u"client-id"_s, u"Specifies the application client id"_s, u"client_id"_s);

    parser.addOptions({clientId});
    parser.process(app);

    if (parser.isSet(clientId)) {
        const QString value = parser.value(clientId);
        auto job = new GitLabAuthenticationJob();
        GitLabAuthenticationJob::GitLabInfo info;
        // Make url
        info.url = u"foo"_s;
        info.clientId = value;
        job->setGitLabInfo(std::move(info));
        job->start();
        app.exec();
    } else {
        parser.showHelp();
    }
    return 0;
}
