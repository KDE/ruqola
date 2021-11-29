/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class ServerInfoWidgetTest : public QWidget
{
    Q_OBJECT
public:
    explicit ServerInfoWidgetTest(QWidget *parent = nullptr);
    ~ServerInfoWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

