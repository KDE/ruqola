/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallwidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>

ConferenceCallWidget::ConferenceCallWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

ConferenceCallWidget::~ConferenceCallWidget() = default;
