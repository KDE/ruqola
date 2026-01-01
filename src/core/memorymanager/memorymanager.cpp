/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "memorymanager.h"
#include "ruqola_memory_management_debug.h"
#include <QTimer>
#include <chrono>
using namespace std::chrono_literals;

MemoryManager::MemoryManager(QObject *parent)
    : QObject{parent}
    , mClearApplicationSettingsModel(new QTimer(this))
    , mClearRoomsHistory(new QTimer(this))
{
    connect(mClearApplicationSettingsModel, &QTimer::timeout, this, [this]() {
        qCDebug(RUQOLA_MEMORY_MANAGEMENT_LOG) << "Clear Application settings model";
        Q_EMIT clearApplicationSettingsModelRequested();
    });

    mClearRoomsHistory->setInterval(1h);
    connect(mClearRoomsHistory, &QTimer::timeout, this, [this]() {
        qCDebug(RUQOLA_MEMORY_MANAGEMENT_LOG) << "Clean room history";
        Q_EMIT cleanRoomHistoryRequested();
    });
    mClearRoomsHistory->start();
}

MemoryManager::~MemoryManager() = default;

void MemoryManager::startClearApplicationSettingsModelTimer()
{
    qCDebug(RUQOLA_MEMORY_MANAGEMENT_LOG) << "Start clear application timer";
    mClearApplicationSettingsModel->start(15min);
}

void MemoryManager::stopClearApplicationSettingsModelTimer()
{
    qCDebug(RUQOLA_MEMORY_MANAGEMENT_LOG) << "Stop clear application timer";
    mClearApplicationSettingsModel->stop();
}

// TODO: test if a room has a big history and we don't go in => clean history

#include "moc_memorymanager.cpp"
