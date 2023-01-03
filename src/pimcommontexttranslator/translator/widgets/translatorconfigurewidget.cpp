/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorconfigurewidget.h"
#include "translatorconfigurecombowidget.h"
#include <KLocalizedString>
#include <KSharedConfig>
#include <QLabel>
#include <QVBoxLayout>
using namespace PimCommonTextTranslator;

class Q_DECL_HIDDEN PimCommonTextTranslator::TranslatorConfigureWidget::TranslatorConfigureWidgetPrivate
{
public:
    TranslatorConfigureWidgetPrivate(TranslatorConfigureWidget *parent)
        : mEngineConfigureComboWidget(new TranslatorConfigureComboWidget(parent))
    {
    }

    TranslatorConfigureComboWidget *const mEngineConfigureComboWidget;
};

TranslatorConfigureWidget::TranslatorConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , d(new TranslatorConfigureWidgetPrivate(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    d->mEngineConfigureComboWidget->setObjectName(QStringLiteral("mEngineConfigureComboWidget"));
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(i18n("Engine:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(d->mEngineConfigureComboWidget);
}

TranslatorConfigureWidget::~TranslatorConfigureWidget() = default;

void TranslatorConfigureWidget::saveSettings()
{
    d->mEngineConfigureComboWidget->save();
}

void TranslatorConfigureWidget::loadSettings()
{
    d->mEngineConfigureComboWidget->load();
}
