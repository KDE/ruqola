/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageDelegateHelperFileTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageDelegateHelperFileTest(QObject *parent = nullptr);
    ~MessageDelegateHelperFileTest() override = default;

private Q_SLOTS:
    void shouldLayoutMultipleFiles();

private:
};
