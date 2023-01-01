/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SlackBridgeWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SlackBridgeWidgetTest(QObject *parent = nullptr);
    ~SlackBridgeWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
