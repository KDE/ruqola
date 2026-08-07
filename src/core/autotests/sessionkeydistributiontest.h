/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QObject>

class SessionKeyDistributionTest : public QObject
{
    Q_OBJECT
public:
    explicit SessionKeyDistributionTest(QObject *parent = nullptr);
    ~SessionKeyDistributionTest() override = default;
private Q_SLOTS:
    void testJsonPayload();
    void testCanStartValidation();
};
