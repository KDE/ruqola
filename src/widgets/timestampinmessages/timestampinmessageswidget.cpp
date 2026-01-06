/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessageswidget.h"
#include <QFormLayout>
using namespace Qt::Literals::StringLiterals;
TimeStampInMessagesWidget::TimeStampInMessagesWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
}

TimeStampInMessagesWidget::~TimeStampInMessagesWidget() = default;
