/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class WebDavAddServerDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit WebDavAddServerDialogTest(QObject *parent = nullptr);
    ~WebDavAddServerDialogTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
