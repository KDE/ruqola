/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <QDialog>

class E2ePasswordDecodeKeyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit E2ePasswordDecodeKeyDialog(QWidget *parent = nullptr);
    ~E2ePasswordDecodeKeyDialog() override;
};
