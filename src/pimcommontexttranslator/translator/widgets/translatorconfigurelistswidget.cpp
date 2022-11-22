/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorconfigurelistswidget.h"
#include "translator/misc/translatorutil.h"
#include "translatorconfigurelanguagelistwidget.h"
#include "translatorconfigurelistswidget.h"
#include "translatorconfigureutil.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QComboBox>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
using namespace PimCommonTextTranslator;
class Q_DECL_HIDDEN PimCommonTextTranslator::TranslatorConfigureListsWidget::TranslatorConfigureListsWidgetPrivate
{
public:
    TranslatorConfigureListsWidgetPrivate(TranslatorConfigureListsWidget *parent)
        : mEngine(new QComboBox(parent))
        , mConfigureEngine(new QToolButton(parent))
        , mFromLanguageWidget(new TranslatorConfigureLanguageListWidget(i18n("From:"), parent))
        , mToLanguageWidget(new TranslatorConfigureLanguageListWidget(i18n("To:"), parent))
    {
    }
    QComboBox *const mEngine;
    QToolButton *const mConfigureEngine;
    PimCommonTextTranslator::TranslatorConfigureLanguageListWidget *const mFromLanguageWidget;
    PimCommonTextTranslator::TranslatorConfigureLanguageListWidget *const mToLanguageWidget;
};

TranslatorConfigureListsWidget::TranslatorConfigureListsWidget(QWidget *parent)
    : QWidget{parent}
    , d(new TranslatorConfigureListsWidgetPrivate(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    d->mEngine->setObjectName(QStringLiteral("mEngine"));
    d->mConfigureEngine->setObjectName(QStringLiteral("mConfigureEngine"));
    d->mConfigureEngine->setEnabled(false); // Disable by default
    d->mConfigureEngine->setIcon(QIcon::fromTheme(QStringLiteral("settings-configure")));
    connect(d->mConfigureEngine, &QToolButton::clicked, this, &TranslatorConfigureListsWidget::slotConfigureEngine);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(i18n("Engine:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(d->mEngine);
    hboxLayout->addWidget(d->mConfigureEngine);
    hboxLayout->addStretch(0);

    auto hLanguageListboxLayout = new QHBoxLayout;
    hLanguageListboxLayout->setObjectName(QStringLiteral("hLanguageListboxLayout"));
    hLanguageListboxLayout->setContentsMargins({});
    mainLayout->addLayout(hLanguageListboxLayout);

    d->mFromLanguageWidget->setObjectName(QStringLiteral("mFromLanguageWidget"));
    d->mToLanguageWidget->setObjectName(QStringLiteral("mToLanguageWidget"));
    hLanguageListboxLayout->addWidget(d->mFromLanguageWidget);
    hLanguageListboxLayout->addWidget(d->mToLanguageWidget);

    connect(d->mEngine, &QComboBox::currentIndexChanged, this, &TranslatorConfigureListsWidget::slotEngineChanged);

    fillEngine();
}

TranslatorConfigureListsWidget::~TranslatorConfigureListsWidget() = default;

void TranslatorConfigureListsWidget::fillEngine()
{
    TranslatorConfigureUtil::fillComboboxSettings(d->mEngine);
}

void TranslatorConfigureListsWidget::save()
{
    const QString engine = d->mEngine->currentData().toString();
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), TranslatorUtil::groupTranslateName());
    groupTranslate.writeEntry(TranslatorUtil::engineTranslateName(), engine);
    groupTranslate.writeEntry(QStringLiteral("From"), d->mFromLanguageWidget->selectedLanguages());
    groupTranslate.writeEntry(QStringLiteral("To"), d->mToLanguageWidget->selectedLanguages());
}

void TranslatorConfigureListsWidget::load()
{
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), TranslatorUtil::groupTranslateName());
    const QString engine = groupTranslate.readEntry(TranslatorUtil::engineTranslateName(), TranslatorUtil::defaultEngineName()); // Google by default
    const int index = d->mEngine->findData(engine);
    if (index != -1) {
        d->mEngine->setCurrentIndex(index);
    }
    d->mFromLanguageWidget->setSelectedLanguages(groupTranslate.readEntry(QStringLiteral("From"), QStringList()));
    d->mToLanguageWidget->setSelectedLanguages(groupTranslate.readEntry(QStringLiteral("To"), QStringList()));
}

void TranslatorConfigureListsWidget::slotEngineChanged(int index)
{
    const QStringList fromLanguages = d->mFromLanguageWidget->selectedLanguages();
    const QStringList toLanguages = d->mToLanguageWidget->selectedLanguages();

    const QString engine = d->mEngine->itemData(index).toString();
    const QVector<QPair<QString, QString>> listLanguage = PimCommonTextTranslator::TranslatorUtil::supportedLanguages(engine);
    d->mFromLanguageWidget->clear();
    d->mToLanguageWidget->clear();

    const int fullListLanguageSize(listLanguage.count());
    for (int i = 0; i < fullListLanguageSize; ++i) {
        const QPair<QString, QString> currentLanguage = listLanguage.at(i);
        d->mFromLanguageWidget->addItem(currentLanguage);
        if ((i != 0)) { // Remove auto
            d->mToLanguageWidget->addItem(currentLanguage);
        }
    }
    // Restore if possible
    d->mFromLanguageWidget->setSelectedLanguages(fromLanguages);
    d->mToLanguageWidget->setSelectedLanguages(toLanguages);
    d->mConfigureEngine->setEnabled(TranslatorUtil::hasConfigureDialog(PimCommonTextTranslator::TranslatorUtil::convertStringToTranslatorEngine(engine)));
}

void TranslatorConfigureListsWidget::slotConfigureEngine()
{
    const QString engine = d->mEngine->currentData().toString();
    if (TranslatorUtil::hasConfigureDialog(PimCommonTextTranslator::TranslatorUtil::convertStringToTranslatorEngine(engine))) { }
    // TODO
}
