/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <TextAddonsWidgets/OpenFileJob>

class MessageAttachmentDelegateHelperOpenFileJob : public TextAddonsWidgets::OpenFileJob
{
    Q_OBJECT
public:
    explicit MessageAttachmentDelegateHelperOpenFileJob(QObject *parent = nullptr);
    ~MessageAttachmentDelegateHelperOpenFileJob() override;
};
