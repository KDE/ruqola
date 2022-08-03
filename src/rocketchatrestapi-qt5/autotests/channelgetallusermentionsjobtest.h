/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelGetAllUserMentionsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelGetAllUserMentionsJobTest(QObject *parent = nullptr);
    ~ChannelGetAllUserMentionsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveParameterSupport();
    void shouldGenerateRequest();
    void shouldNotStarting();
    void shouldHaveParameterSupportSorting();
    void shouldHaveParameterSupportSortingTwoParameters();
};
