/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>

class LIBRUQOLAWIDGETS_TESTS_EXPORT E2eCopyPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit E2eCopyPasswordDialog(QWidget *parent = nullptr);
    ~E2eCopyPasswordDialog() override;
};
