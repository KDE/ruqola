/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UnicodeEmoticonParserTest : public QObject
{
    Q_OBJECT
public:
    explicit UnicodeEmoticonParserTest(QObject *parent = nullptr);
    ~UnicodeEmoticonParserTest() override = default;
};

