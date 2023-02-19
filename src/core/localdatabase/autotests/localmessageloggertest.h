/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LocalMessageLoggerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void shouldStoreMessages();
    void shouldLoadExistingDb();
    void shouldDeleteMessages();
    void shouldReturnNullIfDoesNotExist();
};
