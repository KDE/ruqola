﻿/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "myaccountconfigurewidget.h"
#include "myaccount2faconfigurewidget.h"
#include "myaccountpreferenceconfigurewidget.h"
#include "myaccountprofileconfigurewidget.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>

MyAccountConfigureWidget::MyAccountConfigureWidget(QWidget *parent)
    : QWidget(parent)
    , mMyAccount2FaConfigureWidget(new MyAccount2FaConfigureWidget(this))
    , mMyAccount2ProfileConfigureWidget(new MyAccountProfileConfigureWidget(this))
    , mMyAccountPreferenceConfigureWidget(new MyAccountPreferenceConfigureWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto tabWidget = new QTabWidget(this);
    tabWidget->setObjectName(QStringLiteral("tabWidget"));
    mainLayout->addWidget(tabWidget);

    mMyAccount2ProfileConfigureWidget->setObjectName(QStringLiteral("mMyAccount2ProfileConfigureWidget"));
    tabWidget->addTab(mMyAccount2ProfileConfigureWidget, i18n("Profile"));

    mMyAccount2FaConfigureWidget->setObjectName(QStringLiteral("mMyAccount2FaConfigureWidget"));
    tabWidget->addTab(mMyAccount2FaConfigureWidget, i18n("Two Authentication Factor"));
    if (!Ruqola::self()->rocketChatAccount()->allowProfileChange()) {
        mMyAccount2ProfileConfigureWidget->setVisible(false);
    }

    mMyAccountPreferenceConfigureWidget->setObjectName(QStringLiteral("mMyAccount2FaConfigureWidget"));
    tabWidget->addTab(mMyAccountPreferenceConfigureWidget, i18n("Preference"));
}

MyAccountConfigureWidget::~MyAccountConfigureWidget()
{
}

void MyAccountConfigureWidget::save()
{
    mMyAccount2ProfileConfigureWidget->save();
    mMyAccount2FaConfigureWidget->save();
    mMyAccountPreferenceConfigureWidget->save();
}

void MyAccountConfigureWidget::load()
{
    mMyAccount2ProfileConfigureWidget->load();
    mMyAccount2FaConfigureWidget->load();
    mMyAccountPreferenceConfigureWidget->load();
}
