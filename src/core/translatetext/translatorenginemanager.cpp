/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorenginemanager.h"
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

void TranslatorEngineManager::translatorConfigChanged()
{
    initializeTranslateEngine();
    Q_EMIT updateTranslateEngine();
}

void TranslatorEngineManager::initializeTranslateEngine()
{
    delete mTranslatorEnginePlugin;
    const QString engineName = TextTranslator::TranslatorUtil::loadEngine();
    TextTranslator::TranslatorEngineClient *translatorClient = TextTranslator::TranslatorEngineLoader::self()->createTranslatorClient(engineName);
    if (translatorClient) {
        mTranslatorEnginePlugin = translatorClient->createTranslator();
        connect(mTranslatorEnginePlugin, &TextTranslator::TranslatorEnginePlugin::translateDone, this, &TranslatorEngineManager::slotTranslateDone);
        connect(mTranslatorEnginePlugin, &TextTranslator::TranslatorEnginePlugin::translateFailed, this, &TranslatorEngineManager::translateFailed);
    } else {
        mTranslatorEnginePlugin = nullptr;
    }
}

TextTranslator::TranslatorEnginePlugin *TranslatorEngineManager::translatorEngineBase() const
{
    return mTranslatorEnginePlugin;
}

void TranslatorEngineManager::slotTranslateDone()
{
    const QString result = mTranslatorEnginePlugin->resultTranslate();
    Q_EMIT translateDone(result);
}
