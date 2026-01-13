/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class MyAccountFeaturePreviewConfigureWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit MyAccountFeaturePreviewConfigureWidgetTest(QObject *parent = nullptr);
    ~MyAccountFeaturePreviewConfigureWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
