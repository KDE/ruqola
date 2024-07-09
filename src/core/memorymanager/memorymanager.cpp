/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "memorymanager.h"

MemoryManager::MemoryManager(QObject *parent)
    : QObject{parent}
{
}

MemoryManager::~MemoryManager() = default;
