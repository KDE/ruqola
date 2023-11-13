/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurefontwidget.h"
#include "ruqolaglobalconfig.h"
#include <KFontChooser>
#include <KLocalizedString>

#include <QCheckBox>
#include <QVBoxLayout>

ConfigureFontWidget::ConfigureFontWidget(QWidget *parent)
    : QWidget(parent)
    , mCustomFontCheck(new QCheckBox(i18n("&Use custom fonts"), this))
    , mFontChooser(new KFontChooser(KFontChooser::DisplayFrame, this))
{
    mFontChooser->setMinVisibleItems(4);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCustomFontCheck->setObjectName(QStringLiteral("mCustomFontCheck"));
    mainLayout->addWidget(mCustomFontCheck);

    mFontChooser->setObjectName(QStringLiteral("mFontChooser"));
    mFontChooser->setEnabled(false); // since !mCustomFontCheck->isChecked()
    mainLayout->addWidget(mFontChooser);
    connect(mCustomFontCheck, &QCheckBox::toggled, mFontChooser, &KFontChooser::setEnabled);
}

ConfigureFontWidget::~ConfigureFontWidget() = default;

void ConfigureFontWidget::save()
{
    RuqolaGlobalConfig::self()->setUseCustomFont(mCustomFontCheck->isChecked());
    RuqolaGlobalConfig::self()->setGeneralFont(mFontChooser->font());
    RuqolaGlobalConfig::self()->save();
}

void ConfigureFontWidget::load()
{
    mCustomFontCheck->setChecked(RuqolaGlobalConfig::self()->useCustomFont());
    mFontChooser->setFont(RuqolaGlobalConfig::self()->generalFont());
}

#include "moc_configurefontwidget.cpp"
