/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QList>
#include <QObject>
namespace TextTranslator
{
class TranslatorEnginePlugin;
}
class QDebug;
class TranslateTextJob;
class LIBRUQOLACORE_EXPORT TranslatorEngineManager : public QObject
{
    Q_OBJECT
public:
    ~TranslatorEngineManager() override;
    static TranslatorEngineManager *self();

    struct LIBRUQOLACORE_EXPORT TranslateRequest {
        QString from;
        QString to;
        QString inputText;
        QByteArray messageId;
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] bool operator==(const TranslatorEngineManager::TranslateRequest &other) const;
    };

    void addPendingTranslation(const TranslateRequest &request);

    void translatorConfigChanged();

    [[nodiscard]] TextTranslator::TranslatorEnginePlugin *translatorEngineBase() const;
Q_SIGNALS:
    void updateTranslateEngine();
    void translateDone(const QByteArray &identifier, const QString &translatedText);
    void translateFailed(const QByteArray &identifier, const QString &errorMessage = QString());

private:
    explicit TranslatorEngineManager(QObject *parent = nullptr);
    LIBRUQOLACORE_NO_EXPORT void initializeTranslateEngine();
    LIBRUQOLACORE_NO_EXPORT void startNextTranslation();
    LIBRUQOLACORE_NO_EXPORT void scheduleNextTranslation();
    LIBRUQOLACORE_NO_EXPORT void finishCurrentTranslation();
    LIBRUQOLACORE_NO_EXPORT void cancelCurrentTranslation();
    TextTranslator::TranslatorEnginePlugin *mTranslatorEnginePlugin = nullptr;
    // The request being translated stays at the front of the list until it's done.
    QList<TranslateRequest> mPendingTranslateRequests;
    TranslateTextJob *mCurrentJob = nullptr;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const TranslatorEngineManager::TranslateRequest &t);
