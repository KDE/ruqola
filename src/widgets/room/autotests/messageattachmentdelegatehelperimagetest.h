/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageDelegateHelperImageTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageDelegateHelperImageTest(QObject *parent = nullptr);
    ~MessageDelegateHelperImageTest() override = default;

private Q_SLOTS:
    void shouldExtractMessageData();
};
