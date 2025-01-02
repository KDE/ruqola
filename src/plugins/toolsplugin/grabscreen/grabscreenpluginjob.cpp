/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenpluginjob.h"
#include "utils.h"

GrabScreenPluginJob::GrabScreenPluginJob(QObject *parent)
    : QObject{parent}
{
}

GrabScreenPluginJob::~GrabScreenPluginJob() = default;

void GrabScreenPluginJob::start()
{
    // TODO
}

#include "moc_grabscreenpluginjob.cpp"
