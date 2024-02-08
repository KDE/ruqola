/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "encryptionutils.h"

EncryptionUtils::EncryptionUtils(QObject *parent)
    : QObject{parent}
{
}

EncryptionUtils::~EncryptionUtils() = default;

#include "moc_encryptionutils.cpp"
