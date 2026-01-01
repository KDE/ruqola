/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class AutoGenerateInteractionUiViewTextTest : public QObject
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewTextTest(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewTextTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadJson();
    void shouldLoadJson_data();
};
