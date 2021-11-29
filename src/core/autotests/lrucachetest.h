/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LRUCacheTest : public QObject
{
    Q_OBJECT
public:
    explicit LRUCacheTest(QObject *parent = nullptr);
    ~LRUCacheTest() override = default;

private Q_SLOTS:
    void shouldCacheLastFiveEntries();
    void shouldWorkWithUniquePtr();
};

