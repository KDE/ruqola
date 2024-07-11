/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class E2ePasswordDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit E2ePasswordDialogTest(QObject *parent = nullptr);
    ~E2ePasswordDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
