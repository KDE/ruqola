/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DiscussionsTest : public QObject
{
    Q_OBJECT
public:
    explicit DiscussionsTest(QObject *parent = nullptr);
    ~DiscussionsTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadDiscussions_data();
    void shouldLoadDiscussions();
};

