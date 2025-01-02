/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenpluginjob.h"
#include "grabscreenpluginsettings.h"
#include "utils.h"
#include <QProcess>

GrabScreenPluginJob::GrabScreenPluginJob(QObject *parent)
    : QObject{parent}
{
}

GrabScreenPluginJob::~GrabScreenPluginJob() = default;

void GrabScreenPluginJob::start()
{
    const QString path = Utils::findExecutable(QStringLiteral("spectacle"));
    auto proc = new QProcess(this);
    QStringList arguments;
    arguments << QStringLiteral("-n") << QStringLiteral("-d") << QString::number(2000);
    // TODO use grabscreenpluginsettings for delay
    arguments << QStringLiteral("-bro") << QStringLiteral("path");
    proc->start(path, arguments);
    connect(proc, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        Q_EMIT captureDone();
        deleteLater();
        // TODO
    });
}

#include "moc_grabscreenpluginjob.cpp"
