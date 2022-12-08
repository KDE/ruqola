/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class LingvaEngineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LingvaEngineWidget(QWidget *parent = nullptr);
    ~LingvaEngineWidget() override;
};
