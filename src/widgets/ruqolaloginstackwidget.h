/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QStackedWidget>

class RuqolaLoginStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit RuqolaLoginStackWidget(QWidget *parent = nullptr);
    ~RuqolaLoginStackWidget() override;
};
