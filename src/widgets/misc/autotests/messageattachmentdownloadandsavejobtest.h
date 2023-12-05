/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageAttachmentDownloadAndSaveJobTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageAttachmentDownloadAndSaveJobTest(QObject *parent = nullptr);
    ~MessageAttachmentDownloadAndSaveJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldMessageAttachmentDownloadJobInfoHaveDefaultValues();
};
