/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorconfigurelistswidget.h"
#include "translator/misc/translatorutil.h"
#include "translator/translatorengineloader.h"
#include "translatorconfigurecombowidget.h"
#include "translatorconfigurelanguagelistwidget.h"
#include "translatorconfigurelistswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QLabel>
#include <QVBoxLayout>
using namespace PimCommonTextTranslator;
class Q_DECL_HIDDEN PimCommonTextTranslator::TranslatorConfigureListsWidget::TranslatorConfigureListsWidgetPrivate
{
public:
    TranslatorConfigureListsWidgetPrivate(TranslatorConfigureListsWidget *parent)
        : mEngineConfigureComboWidget(new TranslatorConfigureComboWidget(parent))
        , mFromLanguageWidget(new TranslatorConfigureLanguageListWidget(i18n("From:"), parent))
        , mToLanguageWidget(new TranslatorConfigureLanguageListWidget(i18n("To:"), parent))
    {
    }
    TranslatorConfigureComboWidget *const mEngineConfigureComboWidget;
    PimCommonTextTranslator::TranslatorConfigureLanguageListWidget *const mFromLanguageWidget;
    PimCommonTextTranslator::TranslatorConfigureLanguageListWidget *const mToLanguageWidget;
};

TranslatorConfigureListsWidget::TranslatorConfigureListsWidget(QWidget *parent)
    : QWidget{parent}
    , d(new TranslatorConfigureListsWidgetPrivate(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    d->mEngineConfigureComboWidget->setObjectName(QStringLiteral("mEngineConfigureComboWidget"));

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(i18n("Engine:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(d->mEngineConfigureComboWidget);
    hboxLayout->addStretch(0);

    auto hLanguageListboxLayout = new QHBoxLayout;
    hLanguageListboxLayout->setObjectName(QStringLiteral("hLanguageListboxLayout"));
    hLanguageListboxLayout->setContentsMargins({});
    mainLayout->addLayout(hLanguageListboxLayout);

    d->mFromLanguageWidget->setObjectName(QStringLiteral("mFromLanguageWidget"));
    d->mToLanguageWidget->setObjectName(QStringLiteral("mToLanguageWidget"));
    hLanguageListboxLayout->addWidget(d->mFromLanguageWidget);
    hLanguageListboxLayout->addWidget(d->mToLanguageWidget);

    connect(d->mEngineConfigureComboWidget, &TranslatorConfigureComboWidget::engineChanged, this, &TranslatorConfigureListsWidget::slotEngineChanged);
}

TranslatorConfigureListsWidget::~TranslatorConfigureListsWidget() = default;

void TranslatorConfigureListsWidget::save()
{
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), TranslatorUtil::groupTranslateName());
    groupTranslate.writeEntry(QStringLiteral("From"), d->mFromLanguageWidget->selectedLanguages());
    groupTranslate.writeEntry(QStringLiteral("To"), d->mToLanguageWidget->selectedLanguages());
    d->mEngineConfigureComboWidget->save();
}

void TranslatorConfigureListsWidget::load()
{
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), TranslatorUtil::groupTranslateName());
    d->mFromLanguageWidget->setSelectedLanguages(groupTranslate.readEntry(QStringLiteral("From"), QStringList()));
    d->mToLanguageWidget->setSelectedLanguages(groupTranslate.readEntry(QStringLiteral("To"), QStringList()));
    d->mEngineConfigureComboWidget->load();
}

void TranslatorConfigureListsWidget::slotEngineChanged(const QString &engine)
{
    const QStringList fromLanguages = d->mFromLanguageWidget->selectedLanguages();
    const QStringList toLanguages = d->mToLanguageWidget->selectedLanguages();

    const QVector<QPair<QString, QString>> listLanguage = PimCommonTextTranslator::TranslatorEngineLoader::self()->supportedLanguages(engine);
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
}
