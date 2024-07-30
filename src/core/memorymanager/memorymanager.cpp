/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

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
{
    connect(mClearApplicationSettingsModel, &QTimer::timeout, this, [this]() {
        qCDebug(RUQOLA_MEMORY_MANAGEMENT_LOG) << "clear Application settings model";
        Q_EMIT clearApplicationSettingsModelRequested();
    });
}

MemoryManager::~MemoryManager() = default;

void MemoryManager::startClearApplicationSettingsModelTimer()
{
    mClearApplicationSettingsModel->start(15min);
}

// TODO: test if a room has a big history and we don't go in => clean history

#include "moc_memorymanager.cpp"
