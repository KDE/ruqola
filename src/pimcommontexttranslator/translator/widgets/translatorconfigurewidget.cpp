/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorconfigurewidget.h"
#include "translator/misc/translatorutil.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>
using namespace PimCommonTextTranslator;

class Q_DECL_HIDDEN PimCommonTextTranslator::TranslatorConfigureWidget::TranslatorConfigureWidgetPrivate
{
public:
    TranslatorConfigureWidgetPrivate(TranslatorConfigureWidget *parent)
        : mEngine(new QComboBox(parent))
        , mStackedWidget(new QStackedWidget(parent))
        , mEmptyWidget(new QWidget(parent))
        , mLibreTranslateWidget(new QWidget(parent))
    {
    }

    QComboBox *const mEngine;
    QStackedWidget *const mStackedWidget;
    QWidget *const mEmptyWidget;
    QWidget *const mLibreTranslateWidget;
};

TranslatorConfigureWidget::TranslatorConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , d(new TranslatorConfigureWidgetPrivate(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    d->mEngine->setObjectName(QStringLiteral("mEngine"));
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(i18n("Engine:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(d->mEngine);

    d->mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(d->mStackedWidget);

    d->mEmptyWidget->setObjectName(QStringLiteral("mEmptyWidget"));
    d->mStackedWidget->addWidget(d->mEmptyWidget);

    d->mLibreTranslateWidget->setObjectName(QStringLiteral("mLibreTranslateWidget"));
    d->mStackedWidget->addWidget(d->mLibreTranslateWidget);

    connect(d->mEngine, &QComboBox::currentIndexChanged, this, &TranslatorConfigureWidget::switchEngine);
    d->mStackedWidget->setCurrentWidget(d->mEmptyWidget);
    fillEngine();
}

TranslatorConfigureWidget::~TranslatorConfigureWidget() = default;

void TranslatorConfigureWidget::fillEngine()
{
    TranslatorUtil::fillComboboxSettings(d->mEngine);
}

void TranslatorConfigureWidget::saveSettings()
{
    TranslatorUtil::saveEngineSettings(d->mEngine->currentData().toString());
}

void TranslatorConfigureWidget::loadSettings()
{
    const QString engine = TranslatorUtil::loadEngine();
    const int index = d->mEngine->findData(engine);
    if (index != -1) {
        d->mEngine->setCurrentIndex(index);
    }
}

void TranslatorConfigureWidget::switchEngine(int index)
{
    // TODO
}
