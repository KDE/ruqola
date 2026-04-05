/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaswitchofflinemodewidget.h"
#include <QHBoxLayout>

using namespace Qt::Literals::StringLiterals;
RuqolaSwitchOfflineModeWidget::RuqolaSwitchOfflineModeWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    // TODO
}

RuqolaSwitchOfflineModeWidget::~RuqolaSwitchOfflineModeWidget() = default;

#include "moc_ruqolaswitchofflinemodewidget.cpp"
