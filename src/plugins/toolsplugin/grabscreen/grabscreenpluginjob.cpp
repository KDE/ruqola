/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenpluginjob.h"
#include "grabscreenplugintoolconfig.h"
#include "utils.h"
#include <QProcess>

GrabScreenPluginJob::GrabScreenPluginJob(QObject *parent)
    : QObject{parent}
{
}

GrabScreenPluginJob::~GrabScreenPluginJob() = default;

bool GrabScreenPluginJob::canStart() const
{
    return !mFilePath.isEmpty();
}

void GrabScreenPluginJob::start()
{
    if (!canStart()) {
        Q_EMIT captureCanceled();
        deleteLater();
        return;
    }
    const QString path = Utils::findExecutable(QStringLiteral("spectacle"));
    auto proc = new QProcess(this);
    QStringList arguments;
    arguments << QStringLiteral("-n") << QStringLiteral("-d") << QString::number(GrabScreenPluginToolConfig::self()->delay());
    arguments << QStringLiteral("-bro") << mFilePath;
    proc->start(path, arguments);
    connect(proc, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        Q_EMIT captureDone();
        deleteLater();
    });
}

QString GrabScreenPluginJob::filePath() const
{
    return mFilePath;
}

void GrabScreenPluginJob::setFilePath(const QString &newFilePath)
{
    mFilePath = newFilePath;
}

#include "moc_grabscreenpluginjob.cpp"
