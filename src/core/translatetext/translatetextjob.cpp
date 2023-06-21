/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatetextjob.h"
#include "ruqola_debug.h"
#include "translatetext/translatorenginemanager.h"
#include <KLocalizedString>
#include <TextTranslator/TranslatorEnginePlugin>

TranslateTextJob::TranslateTextJob(QObject *parent)
    : QObject(parent)
{
    connect(TranslatorEngineManager::self(), &TranslatorEngineManager::translateDone, this, &TranslateTextJob::translateDone);
    connect(TranslatorEngineManager::self(), &TranslatorEngineManager::translateFailed, this, &TranslateTextJob::translateFailed);
}

TranslateTextJob::~TranslateTextJob() = default;

void TranslateTextJob::translate()
{
    if (mInfo.isValid()) {
        auto translatorEngine = TranslatorEngineManager::self()->translatorEngineBase();
        translatorEngine->setInputText(mInfo.inputText);
        translatorEngine->setFrom(mInfo.from);
        translatorEngine->setTo(mInfo.to);
        translatorEngine->translate();
    } else {
        Q_EMIT translateFailed(false, i18n("Missing translator info. It's a bug"));
        qCDebug(RUQOLA_LOG) << " Invalid translate info " << mInfo;
    }
}

const TranslateTextJob::TranslateInfo &TranslateTextJob::info() const
{
    return mInfo;
}

void TranslateTextJob::setInfo(const TranslateInfo &newInfo)
{
    mInfo = newInfo;
}

bool TranslateTextJob::TranslateInfo::isValid() const
{
    return !from.isEmpty() && !to.isEmpty() && !inputText.isEmpty();
}

QDebug operator<<(QDebug d, const TranslateTextJob::TranslateInfo &t)
{
    d << "From " << t.from;
    d << "To " << t.to;
    d << "inputtext " << t.inputText;
    return d;
}

#include "moc_translatetextjob.cpp"
