/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatetextjob.h"

TranslateTextJob::TranslateTextJob(RocketChatAccount *account, QObject *parent)
    : ConverterTextAbstractJob(account, parent)
{
}

TranslateTextJob::~TranslateTextJob() = default;

void TranslateTextJob::start()
{
    // TODO
}
