/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class WhatsNewWidgetTest : public QWidget
{
    Q_OBJECT
public:
    explicit WhatsNewWidgetTest(QWidget *parent = nullptr);
    ~WhatsNewWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
