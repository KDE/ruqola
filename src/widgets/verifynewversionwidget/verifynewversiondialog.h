/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QDialog>

class VerifyNewVersionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VerifyNewVersionDialog(QWidget *parent = nullptr);
    ~VerifyNewVersionDialog() override;
};
