/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ShowImagePrevNextImageWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ShowImagePrevNextImageWidgetTest(QObject *parent = nullptr);
    ~ShowImagePrevNextImageWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
