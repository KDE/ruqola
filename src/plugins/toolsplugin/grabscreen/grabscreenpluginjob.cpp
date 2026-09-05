/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenpluginjob.h"

#include "grabscreenplugin_debug.h"
#include "grabscreenplugintoolconfig.h"
#include <QProcess>
#include <TextAddonsWidgets/ExecutableUtils>

using namespace Qt::Literals::StringLiterals;
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
    const QString path = TextAddonsWidgets::ExecutableUtils::findExecutable(u"spectacle"_s);
    if (path.isEmpty()) {
        qCWarning(RUQOLA_GRABSCREEN_PLUGIN_LOG) << "Impossible to find spectacle";
        Q_EMIT captureCanceled();
        deleteLater();
        return;
    }
    auto proc = new QProcess(this);
    const QStringList arguments{u"-n"_s, u"-d"_s, QString::number(GrabScreenPluginToolConfig::self()->delay()), u"-bro"_s, mFilePath};
    connect(proc, &QProcess::finished, this, [this, proc](int exitCode, QProcess::ExitStatus exitStatus) {
        // finished() and errorOccurred() can both be emitted (e.g. on crash): only react to the first one.
        proc->disconnect(this);
        if (exitStatus != QProcess::NormalExit) {
            qCWarning(RUQOLA_GRABSCREEN_PLUGIN_LOG) << "spectacle crashed. Exit code:" << exitCode;
            Q_EMIT captureCanceled();
        } else {
            Q_EMIT captureDone();
        }
        deleteLater();
    });

    connect(proc, &QProcess::errorOccurred, this, [this, proc](QProcess::ProcessError errors) {
        proc->disconnect(this);
        qCWarning(RUQOLA_GRABSCREEN_PLUGIN_LOG) << "Error occurred " << errors;
        Q_EMIT captureCanceled();
        deleteLater();
    });
    proc->start(path, arguments);
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
