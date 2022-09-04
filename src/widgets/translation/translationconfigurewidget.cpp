/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translationconfigurewidget.h"
#include "convertertextjob/translatetext/translatorutil.h"
#include "translationconfigureutil.h"
#include <KLocalizedString>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

TranslationConfigureWidget::TranslationConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mEngine(new QComboBox(this))
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

    auto vboxLayout = new QVBoxLayout;
    vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
    vboxLayout->setContentsMargins({});
    mainLayout->addLayout(vboxLayout);

    fillEngine();
}

TranslationConfigureWidget::~TranslationConfigureWidget() = default;

void TranslationConfigureWidget::fillEngine()
{
    TranslationConfigureUtil::fillComboboxSettings(mEngine);
}
