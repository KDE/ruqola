/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageListDelegateTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageListDelegateTest(QObject *parent = nullptr);
    ~MessageListDelegateTest() override = default;

private Q_SLOTS:
    void layoutChecks_data();
    void layoutChecks();

private:
};

