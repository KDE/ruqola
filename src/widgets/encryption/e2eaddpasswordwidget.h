/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KMessageWidget>

class E2eAddPasswordWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit E2eAddPasswordWidget(QWidget *parent = nullptr);
    ~E2eAddPasswordWidget() override;
};
