/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class AutoGenerateInteractionUiViewImageTest : public QObject
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewImageTest(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewImageTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
