/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageCacheTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageCacheTest(QObject *parent = nullptr);
    ~MessageCacheTest() override = default;
private Q_SLOTS:
    void shouldLoadSingleMessage();
};

