/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DeeplEngineClientTest : public QObject
{
    Q_OBJECT
public:
    explicit DeeplEngineClientTest(QObject *parent = nullptr);
    ~DeeplEngineClientTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
