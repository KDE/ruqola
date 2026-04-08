/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RuqolaSwitchOfflineModeWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit RuqolaSwitchOfflineModeWidgetTest(QObject *parent = nullptr);
    ~RuqolaSwitchOfflineModeWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
