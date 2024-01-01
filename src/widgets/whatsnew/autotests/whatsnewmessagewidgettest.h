/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class WhatsNewMessageWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit WhatsNewMessageWidgetTest(QObject *parent = nullptr);
    ~WhatsNewMessageWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
