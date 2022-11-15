/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorenginemanager.h"
#include "translatorutil.h"

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
    delete mTranslatorEngineBase;
    mTranslatorEngineBase = TranslatorUtil::switchEngine(TranslatorUtil::loadEngineSettings(), this);
    connect(mTranslatorEngineBase, &TranslatorEngineBase::translateDone, this, &TranslatorEngineManager::slotTranslateDone);
    connect(mTranslatorEngineBase, &TranslatorEngineBase::translateFailed, this, &TranslatorEngineManager::translateFailed);
}

TranslatorEngineBase *TranslatorEngineManager::translatorEngineBase() const
{
    return mTranslatorEngineBase;
}

void TranslatorEngineManager::slotTranslateDone()
{
    const QString result = mTranslatorEngineBase->resultTranslate();
    Q_EMIT translateDone(result);
}
