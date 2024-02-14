/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ForwardMessageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ForwardMessageDialog(QWidget *parent = nullptr);
    ~ForwardMessageDialog() override;
};
