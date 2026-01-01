/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurefontwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqolaglobalconfig.h"
#include <KFontChooser>
#include <KLocalizedString>

#include <QCheckBox>
#include <QVBoxLayout>

ConfigureFontWidget::ConfigureFontWidget(QWidget *parent)
    : QWidget(parent)
    , mCustomFontCheck(new QCheckBox(i18nc("@option:check", "&Use custom fonts"), this))
    , mFontChooser(new KFontChooser(KFontChooser::DisplayFrame, this))
{
    mFontChooser->setMinVisibleItems(4);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mCustomFontCheck->setObjectName(u"mCustomFontCheck"_s);
    mainLayout->addWidget(mCustomFontCheck);

    mFontChooser->setObjectName(u"mFontChooser"_s);
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

void ConfigureFontWidget::restoreToDefaults()
{
    const bool bUseDefaults = RuqolaGlobalConfig::self()->useDefaults(true);
    const bool customFontCheck = RuqolaGlobalConfig::self()->useCustomFont();
    const QFont generalFont = RuqolaGlobalConfig::self()->generalFont();
    mCustomFontCheck->setChecked(customFontCheck);
    mFontChooser->setFont(generalFont);
    RuqolaGlobalConfig::self()->useDefaults(bUseDefaults);
}

#include "moc_configurefontwidget.cpp"
