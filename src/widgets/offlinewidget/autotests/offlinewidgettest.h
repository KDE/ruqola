/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class OffLineWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit OffLineWidgetTest(QObject *parent = nullptr);
    ~OffLineWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
