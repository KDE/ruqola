/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2fadisabletotpwidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>

MyAccount2FaDisableTotpWidget::MyAccount2FaDisableTotpWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

MyAccount2FaDisableTotpWidget::~MyAccount2FaDisableTotpWidget()
{
}
