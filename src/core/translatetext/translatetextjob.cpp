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
            Q_EMIT translateDone(mTranslateRequest.messageId, result);
        });
        connect(mTranslatorEnginePlugin, &TextTranslator::TranslatorEnginePlugin::translateFailed, this, [this](const QString &errorMessage) {
            disconnectFromEngine();
            Q_EMIT translateFailed(mTranslateRequest.messageId, errorMessage);
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
    if (mTranslateRequest.isValid()) {
        if (!mTranslatorEnginePlugin) {
            Q_EMIT translateFailed(mTranslateRequest.messageId, i18n("No translator engine available."));
            return;
        }
        mTranslatorEnginePlugin->setInputText(mTranslateRequest.inputText);
        mTranslatorEnginePlugin->setFrom(mTranslateRequest.from);
        mTranslatorEnginePlugin->setTo(mTranslateRequest.to);
        mTranslatorEnginePlugin->translate();
    } else {
        Q_EMIT translateFailed(mTranslateRequest.messageId, i18n("Missing translator info. It's a bug"));
        qCDebug(RUQOLA_LOG) << " Invalid translate info " << mTranslateRequest;
    }
}

const TranslatorEngineManager::TranslateRequest &TranslateTextJob::translateRequest() const
{
    return mTranslateRequest;
}

void TranslateTextJob::setTranslateRequest(const TranslatorEngineManager::TranslateRequest &newInfo)
{
    mTranslateRequest = newInfo;
}

#include "moc_translatetextjob.cpp"
