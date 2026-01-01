/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "roomencryptwaitpasswordwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QVBoxLayout>

RoomEncryptWaitPasswordWidget::RoomEncryptWaitPasswordWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);
    // TODO
}

RoomEncryptWaitPasswordWidget::~RoomEncryptWaitPasswordWidget() = default;

#include "moc_roomencryptwaitpasswordwidget.cpp"
