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

#include "exportmessageswidget.h"
#include <KLocalizedString>
#include <QComboBox>
#include <QDateEdit>
#include <QVBoxLayout>

ExportMessagesWidget::ExportMessagesWidget(QWidget *parent)
    : QWidget(parent)
    , mFromDate(new QDateEdit(this))
    , mToDate(new QDateEdit(this))
    , mFormat(new QComboBox(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mFromDate->setObjectName(QStringLiteral("mFromDate"));
    mainLayout->addWidget(mFromDate);

    mToDate->setObjectName(QStringLiteral("mToDate"));
    mainLayout->addWidget(mToDate);

    mFormat->setObjectName(QStringLiteral("mFormat"));
    mainLayout->addWidget(mFormat);
    fillFormat();
}

ExportMessagesWidget::~ExportMessagesWidget()
{
}

void ExportMessagesWidget::fillFormat()
{
    mFormat->addItem(i18n("Json"), QVariant::fromValue(RocketChatRestApi::RoomsExportJob::RoomsExportInfo::FileFormat::Json));
    mFormat->addItem(i18n("Html"), QVariant::fromValue(RocketChatRestApi::RoomsExportJob::RoomsExportInfo::FileFormat::Html));
}

RocketChatRestApi::RoomsExportJob::RoomsExportInfo ExportMessagesWidget::roomExportInfo() const
{
    RocketChatRestApi::RoomsExportJob::RoomsExportInfo info;
    info.exportAs = RocketChatRestApi::RoomsExportJob::RoomsExportInfo::ExportAs::File; //For the moment we can't choose Email

    info.fileFormat = mFormat->currentData().value<RocketChatRestApi::RoomsExportJob::RoomsExportInfo::FileFormat>();
    info.dateFrom = mFromDate->dateTime();
    info.dateTo = mToDate->dateTime();
    return info;
}
