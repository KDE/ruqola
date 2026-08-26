/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "translatorenginemanager.h"
#include <QObject>
class QDebug;
namespace TextTranslator
{
class TranslatorEnginePlugin;
}
class LIBRUQOLACORE_EXPORT TranslateTextJob : public QObject
{
    Q_OBJECT
public:
    explicit TranslateTextJob(QObject *parent = nullptr);
    ~TranslateTextJob() override;

    void translate();

    [[nodiscard]] const TranslatorEngineManager::TranslateRequest &translateRequest() const;
    void setTranslateRequest(const TranslatorEngineManager::TranslateRequest &newInfo);

Q_SIGNALS:
    void translateDone(const QByteArray &messageId, const QString &translatedText);
    void translateFailed(const QByteArray &messageId, const QString &errorMessage = QString());

private:
    LIBRUQOLACORE_NO_EXPORT void disconnectFromEngine();
    TranslatorEngineManager::TranslateRequest mTranslateRequest;
    TextTranslator::TranslatorEnginePlugin *mTranslatorEnginePlugin = nullptr;
};
