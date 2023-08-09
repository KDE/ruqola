/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>
#include <texttranslator_version.h>
namespace TextTranslator
{
class TranslatorEnginePlugin;
}
class LIBRUQOLACORE_EXPORT TranslatorEngineManager : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorEngineManager(QObject *parent = nullptr);
    ~TranslatorEngineManager() override;
    static TranslatorEngineManager *self();

    void translatorConfigChanged();

    Q_REQUIRED_RESULT TextTranslator::TranslatorEnginePlugin *translatorEngineBase() const;

Q_SIGNALS:
    void updateTranslateEngine();
    void translateDone(const QString &translatedText);
#if TEXTTRANSLATE_VERSION < QT_VERSION_CHECK(1, 4, 40)
    void translateFailed(bool result, const QString &errorMessage = QString());
#else
    void translateFailed(const QString &errorMessage = QString());
#endif

private:
    void initializeTranslateEngine();
    void slotTranslateDone();
    TextTranslator::TranslatorEnginePlugin *mTranslatorEnginePlugin = nullptr;
};
