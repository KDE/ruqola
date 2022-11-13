/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AutoCorrectionSettingsTest : public QObject
{
    Q_OBJECT
public:
    explicit AutoCorrectionSettingsTest(QObject *parent = nullptr);
    ~AutoCorrectionSettingsTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
