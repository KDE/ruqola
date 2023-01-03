/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TranslatorConfigureWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorConfigureWidgetTest(QObject *parent = nullptr);
    ~TranslatorConfigureWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
