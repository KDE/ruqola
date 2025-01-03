/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenplugintoolinterface.h"
#include "grabscreenpluginjob.h"

GrabScreenPluginToolInterface::GrabScreenPluginToolInterface(QObject *parent)
    : PluginToolInterface{parent}
{
}

GrabScreenPluginToolInterface::~GrabScreenPluginToolInterface() = default;

void GrabScreenPluginToolInterface::activateTool()
{
    auto job = new GrabScreenPluginJob(this);
    // TODO
    job->setFilePath(QString());
    connect(job, &GrabScreenPluginJob::captureCanceled, this, []() {
        // TODO
    });
    connect(job, &GrabScreenPluginJob::captureDone, this, []() {
        // TODO
    });
    job->start();
}
