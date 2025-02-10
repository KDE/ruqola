/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>

class LIBRUQOLAWIDGETS_TESTS_EXPORT E2eDisableDialog : public QDialog
{
    Q_OBJECT
public:
    explicit E2eDisableDialog(QWidget *parent = nullptr);
    ~E2eDisableDialog() override;
};
