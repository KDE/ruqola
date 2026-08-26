/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorenginemanager.h"
#include "ruqola_debug.h"
#include "translatetext/translatetextjob.h"

#include <KLocalizedString>
#include <TextTranslator/TranslatorEngineClient>
#include <TextTranslator/TranslatorEngineLoader>
#include <TextTranslator/TranslatorEnginePlugin>
#include <TextTranslator/TranslatorUtil>
TranslatorEngineManager::TranslatorEngineManager(QObject *parent)
    : QObject{parent}
{
    initializeTranslateEngine();
}

TranslatorEngineManager::~TranslatorEngineManager() = default;

TranslatorEngineManager *TranslatorEngineManager::self()
{
    static TranslatorEngineManager s_self;
    return &s_self;
}

bool TranslatorEngineManager::TranslateRequest::operator==(const TranslatorEngineManager::TranslateRequest &other) const = default;

void TranslatorEngineManager::addPendingTranslation(const TranslateRequest &request)
{
    if (!request.isValid()) {
        qCWarning(RUQOLA_LOG) << " Invalid translate request " << request;
        return;
    }
    if (mPendingTranslateRequests.contains(request)) {
        // Queued already, or being translated right now: don't translate it twice.
        return;
    }
    mPendingTranslateRequests.append(request);
    // mCurrentJob, not the list, tells whether something is in flight: the request being
    // translated stays in the list so that the check above can see it.
    if (!mCurrentJob) {
        startNextTranslation();
    }
}

void TranslatorEngineManager::startNextTranslation()
{
    if (mCurrentJob || mPendingTranslateRequests.isEmpty()) {
        return;
    }
    // The engine plugin is shared and holds the text being translated, so it can only
    // carry one translation at a time: they are run one after the other.
    const TranslateRequest request = mPendingTranslateRequests.constFirst();
    TranslateTextJob::TranslateInfo info;
    info.from = request.from;
    info.to = request.to;
    info.inputText = request.inputText;
    info.messageId = request.messageId;
    mCurrentJob = new TranslateTextJob(this);
    mCurrentJob->setInfo(info);
    connect(mCurrentJob, &TranslateTextJob::translateDone, this, [this](const QByteArray &messageId, const QString &str) {
        finishCurrentTranslation();
        Q_EMIT translateDone(messageId, str);
        scheduleNextTranslation();
    });
    connect(mCurrentJob, &TranslateTextJob::translateFailed, this, [this](const QByteArray &messageId, const QString &errorMessage) {
        finishCurrentTranslation();
        Q_EMIT translateFailed(messageId, errorMessage);
        scheduleNextTranslation();
    });
    // Careful: this can fail synchronously (invalid info, no engine, or the plugin
    // rejecting identical from/to languages), so the state above must be complete.
    mCurrentJob->translate();
}

void TranslatorEngineManager::finishCurrentTranslation()
{
    if (!mPendingTranslateRequests.isEmpty()) {
        mPendingTranslateRequests.removeFirst();
    }
    if (mCurrentJob) {
        mCurrentJob->deleteLater();
        mCurrentJob = nullptr;
    }
}

void TranslatorEngineManager::scheduleNextTranslation()
{
    // Deferred on purpose: the next translation must not start while we are still inside
    // the signal emission of the one which just finished. The job which finished is only
    // deleteLater()'ed, so it's still connected to the shared plugin, and a receiver of
    // translateFailed() may well open a modal dialog and spin an event loop here.
    QMetaObject::invokeMethod(this, &TranslatorEngineManager::startNextTranslation, Qt::QueuedConnection);
}

void TranslatorEngineManager::cancelCurrentTranslation()
{
    if (!mCurrentJob) {
        return;
    }
    QByteArray messageId;
    if (!mPendingTranslateRequests.isEmpty()) {
        messageId = mPendingTranslateRequests.constFirst().messageId;
    }
    finishCurrentTranslation();
    Q_EMIT translateFailed(messageId, i18n("Translation was interrupted as the translator engine changed."));
    // Requests still queued are untouched: they will be translated by the new engine.
    scheduleNextTranslation();
}

void TranslatorEngineManager::translatorConfigChanged()
{
    initializeTranslateEngine();
    Q_EMIT updateTranslateEngine();
}

void TranslatorEngineManager::initializeTranslateEngine()
{
    delete mTranslatorEnginePlugin;
    // Clear it right away: createTranslatorClient() can emit loadingTranslatorFailed()
    // and translatorEngineBase() must not hand out a dangling pointer in between.
    mTranslatorEnginePlugin = nullptr;
    const QString engineName = TextTranslator::TranslatorUtil::loadEngine();
    TextTranslator::TranslatorEngineClient *translatorClient = TextTranslator::TranslatorEngineLoader::self()->createTranslatorClient(engineName);
    if (translatorClient) {
        mTranslatorEnginePlugin = translatorClient->createTranslator();
    }
    // Deleting the previous plugin also destroyed the network reply of the translation
    // which was still in flight, together with the connections carrying its result: the
    // current job would wait for a signal which never comes and block the queue forever.
    // Done last, once the new engine is in place, as this reaches the UI.
    cancelCurrentTranslation();
}

TextTranslator::TranslatorEnginePlugin *TranslatorEngineManager::translatorEngineBase() const
{
    return mTranslatorEnginePlugin;
}

QDebug operator<<(QDebug d, const TranslatorEngineManager::TranslateRequest &t)
{
    d.space() << "From " << t.from;
    d.space() << "To " << t.to;
    d.space() << "inputtext " << t.inputText;
    d.space() << "messageId " << t.messageId;
    return d;
}

bool TranslatorEngineManager::TranslateRequest::isValid() const
{
    return !from.isEmpty() && !to.isEmpty() && !inputText.isEmpty() && !messageId.isEmpty();
}

#include "moc_translatorenginemanager.cpp"
