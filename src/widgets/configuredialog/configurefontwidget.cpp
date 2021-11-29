/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurefontwidget.h"
#include "ruqolaglobalconfig.h"
#include <KFontChooser>
#include <KLocalizedString>
#include <kwidgetsaddons_version.h>

#include <QCheckBox>
#include <QVBoxLayout>

ConfigureFontWidget::ConfigureFontWidget(QWidget *parent)
    : QWidget(parent)
    , mCustomFontCheck(new QCheckBox(i18n("&Use custom fonts"), this))
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 86, 0)
    , mFontChooser(new KFontChooser(KFontChooser::DisplayFrame, this))
#else
    , mFontChooser(new KFontChooser(this, KFontChooser::DisplayFrame, QStringList(), 4))
#endif
{
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 86, 0)
    mFontChooser->setMinVisibleItems(4);
#endif
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

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
