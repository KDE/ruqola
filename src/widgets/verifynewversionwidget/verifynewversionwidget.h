/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class VerifyNewVersionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VerifyNewVersionWidget(QWidget *parent = nullptr);
    ~VerifyNewVersionWidget() override;
};
