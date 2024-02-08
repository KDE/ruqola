/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"

#include <QObject>

class LIBRUQOLAWIDGETS_TESTS_EXPORT E2eKeyManager : public QObject
{
    Q_OBJECT
public:
    explicit E2eKeyManager(QObject *parent = nullptr);
    ~E2eKeyManager() override;
};
