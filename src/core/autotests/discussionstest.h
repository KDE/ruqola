/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
