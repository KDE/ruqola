/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatetextjob.h"
#include "convertertextjob/translatetext/translatorutil.h"

TranslateTextJob::TranslateTextJob(RocketChatAccount *account, QObject *parent)
    : ConverterTextAbstractJob(account, parent)
{
}

TranslateTextJob::~TranslateTextJob() = default;

void TranslateTextJob::start()
{
#if 0
    mTranslatorEngineBase = TranslatorUtil::switchEngine(TranslatorUtil::loadEngineSettings(), this);
    mTranslatorEngineBase->setInputText();
    mTranslatorEngineBase->setFrom();
    mTranslatorEngineBase->setTo();
    mTranslatorEngineBase->translate();
#endif
}
