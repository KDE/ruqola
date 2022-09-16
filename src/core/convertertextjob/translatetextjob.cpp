/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatetextjob.h"
#include "convertertextjob/translatetext/translatorenginemanager.h"
#include "convertertextjob/translatetext/translatorutil.h"

TranslateTextJob::TranslateTextJob(QObject *parent)
    : QObject(parent)
{
    connect(TranslatorEngineManager::self(), &TranslatorEngineManager::translateDone, this, &TranslateTextJob::translateDone);
    connect(TranslatorEngineManager::self(), &TranslatorEngineManager::translateFailed, this, &TranslateTextJob::translateFailed);
}

TranslateTextJob::~TranslateTextJob() = default;

void TranslateTextJob::translate()
{
    auto translatorEngine = TranslatorEngineManager::self()->translatorEngineBase();
    translatorEngine->setInputText(mInputText);
    translatorEngine->setFrom(mFrom);
    translatorEngine->setTo(mTo);
    translatorEngine->translate();
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
