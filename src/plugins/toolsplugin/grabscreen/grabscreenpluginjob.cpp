/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

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
    auto proc = new QProcess(this);
    const QStringList arguments = QStringList() << u"-n"_s << u"-d"_s << QString::number(GrabScreenPluginToolConfig::self()->delay()) << u"-bro"_s << mFilePath;
    connect(proc, &QProcess::finished, this, [this]([[maybe_unused]] int exitCode, [[maybe_unused]] QProcess::ExitStatus exitStatus) {
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
