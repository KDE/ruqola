/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageAttachmentTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageAttachmentTest(QObject *parent = nullptr);
    ~MessageAttachmentTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSerializeData();

    void shouldAllowToDownloadAttachment();
};

