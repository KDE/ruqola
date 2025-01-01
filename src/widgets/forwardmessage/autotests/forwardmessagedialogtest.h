/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class ForwardMessageDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit ForwardMessageDialogTest(QObject *parent = nullptr);
    ~ForwardMessageDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
