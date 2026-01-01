/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DirectoryContainerWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit DirectoryContainerWidgetTest(QObject *parent = nullptr);
    ~DirectoryContainerWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
