/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LibreTranslateEngineConfigureWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit LibreTranslateEngineConfigureWidgetTest(QObject *parent = nullptr);
    ~LibreTranslateEngineConfigureWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
