/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class E2eDisableDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit E2eDisableDialogTest(QObject *parent = nullptr);
    ~E2eDisableDialogTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
