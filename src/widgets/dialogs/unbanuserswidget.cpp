/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "unbanuserswidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
UnbanUsersWidget::UnbanUsersWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18nc("@label:textbox", "Some users are banned from this room. Do you want to unban and add them back?"), this);
    label->setObjectName(u"label"_s);
    mainLayout->addWidget(label);
}

UnbanUsersWidget::~UnbanUsersWidget() = default;

#include "moc_unbanuserswidget.cpp"
