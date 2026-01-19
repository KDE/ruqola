/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class SearchMessageCommandTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchMessageCommandTest(QObject *parent = nullptr);
    ~SearchMessageCommandTest() override = default;
};
