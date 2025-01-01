/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <QObject>

class E2eCopyPasswordWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit E2eCopyPasswordWidgetTest(QObject *parent = nullptr);
    ~E2eCopyPasswordWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
