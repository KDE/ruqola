/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorconfigurewidget.h"
#include "translatetext/translatorutil.h"
#include "translatorconfigurelanguagelistwidget.h"
#include "translatorconfigureutil.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QComboBox>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

TranslatorConfigureWidget::TranslatorConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mEngine(new QComboBox(this))
    , mConfigureEngine(new QToolButton(this))
    , mFromLanguageWidget(new TranslatorConfigureLanguageListWidget(i18n("From:"), this))
    , mToLanguageWidget(new TranslatorConfigureLanguageListWidget(i18n("To:"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mEngine->setObjectName(QStringLiteral("mEngine"));
    mConfigureEngine->setObjectName(QStringLiteral("mConfigureEngine"));
    mConfigureEngine->setEnabled(false); // Disable by default
    mConfigureEngine->setIcon(QIcon::fromTheme(QStringLiteral("settings-configure")));
    connect(mConfigureEngine, &QToolButton::clicked, this, &TranslatorConfigureWidget::slotConfigureEngine);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(i18n("Engine:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(mEngine);
    hboxLayout->addWidget(mConfigureEngine);
    hboxLayout->addStretch(0);

    auto hLanguageListboxLayout = new QHBoxLayout;
    hLanguageListboxLayout->setObjectName(QStringLiteral("hLanguageListboxLayout"));
    hLanguageListboxLayout->setContentsMargins({});
    mainLayout->addLayout(hLanguageListboxLayout);

    mFromLanguageWidget->setObjectName(QStringLiteral("mFromLanguageWidget"));
    mToLanguageWidget->setObjectName(QStringLiteral("mToLanguageWidget"));
    hLanguageListboxLayout->addWidget(mFromLanguageWidget);
    hLanguageListboxLayout->addWidget(mToLanguageWidget);

    connect(mEngine, &QComboBox::currentIndexChanged, this, &TranslatorConfigureWidget::slotEngineChanged);

    fillEngine();
}

TranslatorConfigureWidget::~TranslatorConfigureWidget() = default;

void TranslatorConfigureWidget::fillEngine()
{
    TranslatorConfigureUtil::fillComboboxSettings(mEngine);
}

void TranslatorConfigureWidget::save()
{
    const QString engine = mEngine->currentData().toString();
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), TranslatorUtil::groupTranslateName());
    groupTranslate.writeEntry(TranslatorUtil::engineTranslateName(), engine);
    groupTranslate.writeEntry(QStringLiteral("From"), mFromLanguageWidget->selectedLanguages());
    groupTranslate.writeEntry(QStringLiteral("To"), mToLanguageWidget->selectedLanguages());
}

void TranslatorConfigureWidget::load()
{
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), TranslatorUtil::groupTranslateName());
    const QString engine = groupTranslate.readEntry(TranslatorUtil::engineTranslateName(), TranslatorUtil::defaultEngineName()); // Google by default
    const int index = mEngine->findData(engine);
    if (index != -1) {
        mEngine->setCurrentIndex(index);
    }
    mFromLanguageWidget->setSelectedLanguages(groupTranslate.readEntry(QStringLiteral("From"), QStringList()));
    mToLanguageWidget->setSelectedLanguages(groupTranslate.readEntry(QStringLiteral("To"), QStringList()));
}

void TranslatorConfigureWidget::slotEngineChanged(int index)
{
    const QStringList fromLanguages = mFromLanguageWidget->selectedLanguages();
    const QStringList toLanguages = mToLanguageWidget->selectedLanguages();

    const QString engine = mEngine->itemData(index).toString();
    const QVector<QPair<QString, QString>> listLanguage = TranslatorUtil::supportedLanguages(engine);
    mFromLanguageWidget->clear();
    mToLanguageWidget->clear();

    const int fullListLanguageSize(listLanguage.count());
    for (int i = 0; i < fullListLanguageSize; ++i) {
        const QPair<QString, QString> currentLanguage = listLanguage.at(i);
        mFromLanguageWidget->addItem(currentLanguage);
        if ((i != 0)) { // Remove auto
            mToLanguageWidget->addItem(currentLanguage);
        }
    }
    // Restore if possible
    mFromLanguageWidget->setSelectedLanguages(fromLanguages);
    mToLanguageWidget->setSelectedLanguages(toLanguages);
    mConfigureEngine->setEnabled(TranslatorUtil::hasConfigureDialog(TranslatorUtil::convertStringToTranslatorEngine(engine)));
}

void TranslatorConfigureWidget::slotConfigureEngine()
{
    const QString engine = mEngine->currentData().toString();
    if (TranslatorUtil::hasConfigureDialog(TranslatorUtil::convertStringToTranslatorEngine(engine))) { }
    // TODO
}
