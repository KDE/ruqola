/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PreviewUrlCacheManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit PreviewUrlCacheManagerTest(QObject *parent = nullptr);
    ~PreviewUrlCacheManagerTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldTestRemoveOldFiles();
};
