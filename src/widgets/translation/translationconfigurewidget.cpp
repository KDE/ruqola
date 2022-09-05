/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translationconfigurewidget.h"
#include "convertertextjob/translatetext/translatorutil.h"
#include "translationconfigurelanguagelistwidget.h"
#include "translationconfigureutil.h"
#include <KLocalizedString>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

TranslationConfigureWidget::TranslationConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mEngine(new QComboBox(this))
    , mFromLanguageWidget(new TranslationConfigureLanguageListWidget(i18n("From"), this))
    , mToLanguageWidget(new TranslationConfigureLanguageListWidget(i18n("To"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mEngine->setObjectName(QStringLiteral("mEngine"));

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(i18n("Engine:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(mEngine);

    auto hLanguageListboxLayout = new QHBoxLayout;
    hLanguageListboxLayout->setObjectName(QStringLiteral("hLanguageListboxLayout"));
    hLanguageListboxLayout->setContentsMargins({});
    mainLayout->addLayout(hLanguageListboxLayout);

    mFromLanguageWidget->setObjectName(QStringLiteral("mFromLanguageWidget"));
    mToLanguageWidget->setObjectName(QStringLiteral("mFromLanguageWidget"));
    hLanguageListboxLayout->addWidget(mFromLanguageWidget);
    hLanguageListboxLayout->addWidget(mToLanguageWidget);

    connect(mEngine, &QComboBox::currentIndexChanged, this, &TranslationConfigureWidget::slotEngineChanged);

    fillEngine();
}

TranslationConfigureWidget::~TranslationConfigureWidget() = default;

void TranslationConfigureWidget::fillEngine()
{
    TranslationConfigureUtil::fillComboboxSettings(mEngine);
    uploadListLanguages();
}

void TranslationConfigureWidget::uploadListLanguages()
{
    // TODO
}

void TranslationConfigureWidget::saveSettings()
{
    // TODO
}

void TranslationConfigureWidget::loadSettings()
{
    // TODO
}

void TranslationConfigureWidget::slotEngineChanged(int index)
{
    // TODO
}
