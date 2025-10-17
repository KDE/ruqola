/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include <QObject>

class RuqolaQuickSearchMessageSettingsTest : public QObject
{
    Q_OBJECT
public:
    explicit RuqolaQuickSearchMessageSettingsTest(QObject *parent = nullptr);
    ~RuqolaQuickSearchMessageSettingsTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldClear();
    void shouldTestNextSearchStringIndex();
    void shouldTestPreviousSearchStringIndex();
};
