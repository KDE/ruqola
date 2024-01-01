/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DiscussionTest : public QObject
{
    Q_OBJECT
public:
    explicit DiscussionTest(QObject *parent = nullptr);
    ~DiscussionTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldLoadDiscussion();
    void shouldLoadDiscussion_data();
};
