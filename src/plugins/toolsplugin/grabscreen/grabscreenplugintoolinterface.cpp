/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenplugintoolinterface.h"
#include "grabscreenpluginjob.h"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

GrabScreenPluginToolInterface::GrabScreenPluginToolInterface(QObject *parent)
    : PluginToolInterface{parent}
{
}

GrabScreenPluginToolInterface::~GrabScreenPluginToolInterface() = default;

void GrabScreenPluginToolInterface::activateTool()
{
    // TODO add account name ???
    QString imagePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + QStringLiteral("/ruqola/");
    QDir dir(imagePath);

    // TODO add filename
    imagePath += QStringLiteral("screenshot.png");

    qDebug() << " CXXXXXXXXXXXXXXXXXXXXXXX GrabScreenPluginToolInterface::activateTool ";
    auto job = new GrabScreenPluginJob(this);
    job->setFilePath(imagePath);
    connect(job, &GrabScreenPluginJob::captureCanceled, this, []() {
        qDebug() << " CANCELED";
        // TODO
    });
    connect(job, &GrabScreenPluginJob::captureDone, this, [this]() {
        qDebug() << " DONE";
        // TODO open attachment message
        // TODO
        Q_EMIT executed();
    });
    job->start();
}
