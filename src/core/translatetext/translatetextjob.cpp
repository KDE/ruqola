/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

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
    mTranslatorEnginePlugin = TranslatorEngineManager::self()->translatorEngineBase();
    if (mTranslatorEnginePlugin) {
        connect(mTranslatorEnginePlugin, &TextTranslator::TranslatorEnginePlugin::translateDone, this, [this]() {
            const QString result = mTranslatorEnginePlugin->resultTranslate();
            disconnectFromEngine();
            Q_EMIT translateDone(mInfo.messageId, result);
        });
        connect(mTranslatorEnginePlugin, &TextTranslator::TranslatorEnginePlugin::translateFailed, this, [this](const QString &errorMessage) {
            disconnectFromEngine();
            Q_EMIT translateFailed(mInfo.messageId, errorMessage);
        });
    }
}

void TranslateTextJob::disconnectFromEngine()
{
    // The engine plugin is shared by every job: once we got our own result we must stop
    // listening to it, otherwise the result of the next translation would be delivered
    // here as well (this job is only deleteLater()'ed, so it outlives its result).
    if (mTranslatorEnginePlugin) {
        mTranslatorEnginePlugin->disconnect(this);
        mTranslatorEnginePlugin = nullptr;
    }
}

TranslateTextJob::~TranslateTextJob() = default;

void TranslateTextJob::translate()
{
    if (mInfo.isValid()) {
        if (!mTranslatorEnginePlugin) {
            Q_EMIT translateFailed(mInfo.messageId, i18n("No translator engine available."));
            return;
        }
        mTranslatorEnginePlugin->setInputText(mInfo.inputText);
        mTranslatorEnginePlugin->setFrom(mInfo.from);
        mTranslatorEnginePlugin->setTo(mInfo.to);
        mTranslatorEnginePlugin->translate();
    } else {
        Q_EMIT translateFailed(mInfo.messageId, i18n("Missing translator info. It's a bug"));
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
    d.space() << "From:" << t.from;
    d.space() << "To:" << t.to;
    d.space() << "inputtext:" << t.inputText;
    d.space() << "messageId:" << t.messageId;
    return d;
}

#include "moc_translatetextjob.cpp"
