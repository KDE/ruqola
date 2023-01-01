/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DeeplEngineConfigureDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit DeeplEngineConfigureDialogTest(QObject *parent = nullptr);
    ~DeeplEngineConfigureDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
