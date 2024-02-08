/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqola_private_export.h"
#include <QObject>

class LIBRUQOLACORE_TESTS_EXPORT EncryptionUtils : public QObject
{
    Q_OBJECT
public:
    explicit EncryptionUtils(QObject *parent = nullptr);
    ~EncryptionUtils() override;
};
