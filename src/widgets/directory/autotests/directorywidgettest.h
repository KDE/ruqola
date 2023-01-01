/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class DirectoryWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit DirectoryWidgetTest(QObject *parent = nullptr);
    ~DirectoryWidgetTest() override = default;
private
    Q_SLOT : void shouldHaveDefaultValues();
};
