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
    mTranslatorEngineBase = TranslatorUtil::switchEngine(TranslatorUtil::loadEngineSettings(), this);
    mTranslatorEngineBase->setInputText(mInputText);
    mTranslatorEngineBase->setFrom(mFrom);
    mTranslatorEngineBase->setTo(mTo);
    mTranslatorEngineBase->translate();
    connect(mTranslatorEngineBase, &TranslatorEngineBase::translateDone, this, &TranslateTextJob::slotTranslateDone);
    connect(mTranslatorEngineBase, &TranslatorEngineBase::translateFailed, this, &TranslateTextJob::translateFailed);
}

const QString &TranslateTextJob::from() const
{
    return mFrom;
}

void TranslateTextJob::setFrom(const QString &newFrom)
{
    mFrom = newFrom;
}

const QString &TranslateTextJob::to() const
{
    return mTo;
}

void TranslateTextJob::setTo(const QString &newTo)
{
    mTo = newTo;
}

const QString &TranslateTextJob::inputText() const
{
    return mInputText;
}

void TranslateTextJob::setInputText(const QString &newInputText)
{
    mInputText = newInputText;
}

void TranslateTextJob::slotTranslateDone()
{
    const QString result = mTranslatorEngineBase->resultTranslate();
    Q_EMIT translateDone(result);
}
