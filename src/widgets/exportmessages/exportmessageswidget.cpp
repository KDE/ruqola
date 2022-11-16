/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportmessageswidget.h"
#include <KLocalizedString>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>

ExportMessagesWidget::ExportMessagesWidget(QWidget *parent)
    : QWidget(parent)
    , mFromDate(new QDateEdit(QDate::currentDate(), this))
    , mToDate(new QDateEdit(QDate::currentDate(), this))
    , mFormat(new QComboBox(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mFromDate->setObjectName(QStringLiteral("mFromDate"));
    mainLayout->addRow(i18n("From:"), mFromDate);

    mToDate->setObjectName(QStringLiteral("mToDate"));
    mainLayout->addRow(i18n("To:"), mToDate);

    mFormat->setObjectName(QStringLiteral("mFormat"));
    mainLayout->addRow(i18n("Output Format:"), mFormat);
    fillFormat();
}

ExportMessagesWidget::~ExportMessagesWidget() = default;

void ExportMessagesWidget::fillFormat()
{
    mFormat->addItem(i18n("Json"), QVariant::fromValue(RocketChatRestApi::RoomsExportJob::RoomsExportInfo::FileFormat::Json));
    mFormat->addItem(i18n("Html"), QVariant::fromValue(RocketChatRestApi::RoomsExportJob::RoomsExportInfo::FileFormat::Html));
}

RocketChatRestApi::RoomsExportJob::RoomsExportInfo ExportMessagesWidget::roomExportInfo() const
{
    RocketChatRestApi::RoomsExportJob::RoomsExportInfo info;
    info.exportAs = RocketChatRestApi::RoomsExportJob::RoomsExportInfo::ExportAs::File; // For the moment we can't choose Email

    info.fileFormat = mFormat->currentData().value<RocketChatRestApi::RoomsExportJob::RoomsExportInfo::FileFormat>();
    info.dateFrom = mFromDate->dateTime();
    info.dateTo = mToDate->dateTime();
    return info;
}
