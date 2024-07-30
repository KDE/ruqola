/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "memorymanager.h"
#include <QTimer>
#include <chrono>
using namespace std::chrono_literals;

MemoryManager::MemoryManager(QObject *parent)
    : QObject{parent}
    , mClearApplicationSettingsModel(new QTimer(this))
{
    connect(mClearApplicationSettingsModel, &QTimer::timeout, this, &MemoryManager::clearApplicationSettingsModelRequested);
}

MemoryManager::~MemoryManager() = default;

void MemoryManager::startClearApplicationSettingsModelTimer()
{
    // 30 minutes seems ok.
    mClearApplicationSettingsModel->start(30min);
    // TODO
}

// TODO: test if a room has a big history and we don't go in => clean history
// TODO: clean application market model

#include "moc_memorymanager.cpp"
