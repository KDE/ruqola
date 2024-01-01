/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LocalMessageDatabaseTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void shouldStoreMessages();
    void shouldLoadExistingDb();
    void shouldDeleteMessages();
    void shouldReturnNullIfDoesNotExist();
    void shouldExtractMessages();

    void shouldExtractSpecificNumberOfMessages();
    void shouldExtractSpecificNumberOfMessages_data();

    void shouldGenerateQuery();
    void shouldGenerateQuery_data();
    void shouldVerifyDbFileName();
};
