/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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
using namespace TextTranslator;
class Q_DECL_HIDDEN TextTranslator::TranslatorConfigureListsWidget::TranslatorConfigureListsWidgetPrivate
{
public:
    TranslatorConfigureListsWidgetPrivate(TranslatorConfigureListsWidget *parent)
        : mEngineConfigureComboWidget(new TranslatorConfigureComboWidget(parent))
        , mFromLanguageWidget(new TranslatorConfigureLanguageListWidget(i18n("From:"), parent))
        , mToLanguageWidget(new TranslatorConfigureLanguageListWidget(i18n("To:"), parent))
    {
    }
    bool mLanguageListLoaded = false;
    TranslatorConfigureComboWidget *const mEngineConfigureComboWidget;
    TextTranslator::TranslatorConfigureLanguageListWidget *const mFromLanguageWidget;
    TextTranslator::TranslatorConfigureLanguageListWidget *const mToLanguageWidget;
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
    d->mEngineConfigureComboWidget->load();
}

void TranslatorConfigureListsWidget::loadLanguagesList()
{
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), TranslatorUtil::groupTranslateName());
    const auto fromLanguages = groupTranslate.readEntry(QStringLiteral("From"), QStringList());
    const auto toLanguages = groupTranslate.readEntry(QStringLiteral("To"), QStringList());
    d->mFromLanguageWidget->setSelectedLanguages(fromLanguages);
    d->mToLanguageWidget->setSelectedLanguages(toLanguages);
}

void TranslatorConfigureListsWidget::fillLanguages(const QMap<TextTranslator::TranslatorUtil::Language, QString> &listLanguage)
{
    QMapIterator<TranslatorUtil::Language, QString> i(listLanguage);
    TranslatorUtil translatorUtil;
    while (i.hasNext()) {
        i.next();
        const QString languageCode = TranslatorUtil::languageCode(i.key());
        d->mFromLanguageWidget->addItem(i.value(), languageCode);
        if ((i.key() != TranslatorUtil::automatic)) {
            d->mToLanguageWidget->addItem(i.value(), languageCode);
        }
    }
}

void TranslatorConfigureListsWidget::slotEngineChanged(const QString &engine)
{
    const QMap<TextTranslator::TranslatorUtil::Language, QString> listLanguage = TextTranslator::TranslatorEngineLoader::self()->supportedLanguages(engine);

    if (!d->mLanguageListLoaded) {
        fillLanguages(listLanguage);
        loadLanguagesList();
        d->mLanguageListLoaded = true;
    } else {
        const QStringList fromLanguages = d->mFromLanguageWidget->selectedLanguages();
        const QStringList toLanguages = d->mToLanguageWidget->selectedLanguages();

        d->mFromLanguageWidget->clear();
        d->mToLanguageWidget->clear();
        fillLanguages(listLanguage);

        // Restore if possible
        d->mFromLanguageWidget->setSelectedLanguages(fromLanguages);
        d->mToLanguageWidget->setSelectedLanguages(toLanguages);
    }
}
