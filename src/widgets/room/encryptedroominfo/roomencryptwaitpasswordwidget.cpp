/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "roomencryptwaitpasswordwidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>

RoomEncryptWaitPasswordWidget::RoomEncryptWaitPasswordWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);
    // TODO
}

RoomEncryptWaitPasswordWidget::~RoomEncryptWaitPasswordWidget() = default;

#include "moc_roomencryptwaitpasswordwidget.cpp"
