/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class DeeplEngineConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeeplEngineConfigureWidget(QWidget *parent = nullptr);
    ~DeeplEngineConfigureWidget() override;
};
