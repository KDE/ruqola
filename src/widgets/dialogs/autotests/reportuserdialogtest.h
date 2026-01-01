/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ReportUserDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit ReportUserDialogTest(QObject *parent = nullptr);
    ~ReportUserDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldUpdateOkButton();
};
