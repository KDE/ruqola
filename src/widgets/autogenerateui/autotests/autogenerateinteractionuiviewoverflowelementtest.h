/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AutoGenerateInteractionUiViewOverflowElementTest : public QObject
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewOverflowElementTest(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewOverflowElementTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
