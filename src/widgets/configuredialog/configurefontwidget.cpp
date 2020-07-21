/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "configurefontwidget.h"
#include "ruqolaglobalconfig.h"
#include <KFontChooser>
#include <KLocalizedString>

#include <QCheckBox>
#include <QVBoxLayout>

ConfigureFontWidget::ConfigureFontWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mCustomFontCheck = new QCheckBox(i18n("&Use custom fonts"), this);
    mCustomFontCheck->setObjectName(QStringLiteral("mCustomFontCheck"));
    mainLayout->addWidget(mCustomFontCheck);

    mFontChooser = new KFontChooser(this, KFontChooser::DisplayFrame, QStringList(), 4);
    mFontChooser->setObjectName(QStringLiteral("mFontChooser"));
    mFontChooser->setEnabled(false);   // since !mCustomFontCheck->isChecked()
    mainLayout->addWidget(mFontChooser);
    connect(mCustomFontCheck, &QCheckBox::toggled, mFontChooser, &KFontChooser::setEnabled);
}

ConfigureFontWidget::~ConfigureFontWidget()
{
}

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
