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

#include "myaccountprofileconfigurewidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

MyAccountProfileConfigureWidget::MyAccountProfileConfigureWidget(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mUserName = new QLineEdit(this);
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mainLayout->addRow(i18n("UserName:"), mUserName);

    mEmail = new QLineEdit(this);
    mEmail->setObjectName(QStringLiteral("mEmail"));
    mainLayout->addRow(i18n("Email:"), mEmail);

    mName = new QLineEdit(this);
    mName->setObjectName(QStringLiteral("mName"));
    mainLayout->addRow(i18n("Name:"), mName);

    mNickName = new QLineEdit(this);
    mNickName->setObjectName(QStringLiteral("mNickName"));
    mainLayout->addRow(i18n("NickName:"), mNickName);

    mStatusText = new QLineEdit(this);
    mStatusText->setObjectName(QStringLiteral("mStatusText"));
    mainLayout->addRow(i18n("Status Text:"), mStatusText);

    //TODO add password.

}

MyAccountProfileConfigureWidget::~MyAccountProfileConfigureWidget()
{

}
