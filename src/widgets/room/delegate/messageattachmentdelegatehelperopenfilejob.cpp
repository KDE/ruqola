/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperopenfilejob.h"

MessageAttachmentDelegateHelperOpenFileJob::MessageAttachmentDelegateHelperOpenFileJob(QObject *parent)
    : TextAddonsWidgets::OpenFileJob{parent}
{
}

MessageAttachmentDelegateHelperOpenFileJob::~MessageAttachmentDelegateHelperOpenFileJob() = default;
