/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenpluginjob.h"
using namespace Qt::Literals::StringLiterals;

#include "grabscreenplugin_debug.h"
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
    const QString path = Utils::findExecutable(u"spectacle"_s);
    auto proc = new QProcess(this);
    const QStringList arguments = QStringList() << u"-n"_s << u"-d"_s << QString::number(GrabScreenPluginToolConfig::self()->delay()) << u"-bro"_s << mFilePath;
    connect(proc, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        Q_UNUSED(exitCode);
        Q_UNUSED(exitStatus);
        Q_EMIT captureDone();
        deleteLater();
    });

    connect(proc, &QProcess::errorOccurred, this, [this](QProcess::ProcessError errors) {
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
