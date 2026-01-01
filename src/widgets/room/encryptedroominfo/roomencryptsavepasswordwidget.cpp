/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomencryptsavepasswordwidget.h"

#include <KLocalizedString>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
RoomEncryptSavePasswordWidget::RoomEncryptSavePasswordWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);
    // TODO
}

RoomEncryptSavePasswordWidget::~RoomEncryptSavePasswordWidget() = default;

#include "moc_roomencryptsavepasswordwidget.cpp"
