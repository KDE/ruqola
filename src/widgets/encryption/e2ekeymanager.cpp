/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ekeymanager.h"

E2eKeyManager::E2eKeyManager(QObject *parent)
    : QObject{parent}
{
}

E2eKeyManager::~E2eKeyManager() = default;

#include "moc_e2ekeymanager.cpp"
