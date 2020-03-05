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

#include "configuregeneralwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <KLocalizedString>
#include <QCheckBox>
#include "ruqolaglobalconfig.h"

ConfigureGeneralWidget::ConfigureGeneralWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mSetOnlineForAllAccount = new QCheckBox(i18n("Set Online Account"), this);
    mSetOnlineForAllAccount->setObjectName(QStringLiteral("mSetOnlineForAllAccount"));
    mainLayout->addWidget(mSetOnlineForAllAccount);

    mainLayout->addStretch(1);
}

ConfigureGeneralWidget::~ConfigureGeneralWidget()
{
}

void ConfigureGeneralWidget::save()
{
    RuqolaGlobalConfig::self()->setSetOnlineAccounts(mSetOnlineForAllAccount->isChecked());
    //TODO fix load default user status when we start apps.
    RuqolaGlobalConfig::self()->save();
}

void ConfigureGeneralWidget::load()
{
    mSetOnlineForAllAccount->setChecked(RuqolaGlobalConfig::self()->setOnlineAccounts());
}
