/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportmessageswidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mFromDate->setObjectName(u"mFromDate"_s);
    mainLayout->addRow(i18n("From:"), mFromDate);

    mToDate->setObjectName(u"mToDate"_s);
    mainLayout->addRow(i18n("To:"), mToDate);

    mFormat->setObjectName(u"mFormat"_s);
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

#include "moc_exportmessageswidget.cpp"
