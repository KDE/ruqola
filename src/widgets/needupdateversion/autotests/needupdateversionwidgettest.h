/*
  SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class NeedUpdateVersionWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit NeedUpdateVersionWidgetTest(QObject *parent = nullptr);
    ~NeedUpdateVersionWidgetTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
