/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportmessagesdialog.h"

#include "exportmessageswidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myExportMessagesDialogConfigGroupName[] = "ExportMessagesDialog";
}
using namespace Qt::Literals::StringLiterals;
ExportMessagesDialog::ExportMessagesDialog(QWidget *parent)
    : QDialog(parent)
    , mExportMessagesWidget(new ExportMessagesWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Export Messages"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mExportMessagesWidget->setObjectName(u"mExportMessagesWidget"_s);
    mainLayout->addWidget(mExportMessagesWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ExportMessagesDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ExportMessagesDialog::accept);
    readConfig();
}

ExportMessagesDialog::~ExportMessagesDialog() = default;

RocketChatRestApi::RoomsExportJob::RoomsExportInfo ExportMessagesDialog::roomExportInfo() const
{
    return mExportMessagesWidget->roomExportInfo();
}

void ExportMessagesDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myExportMessagesDialogConfigGroupName), QSize(400, 300));
}

#include "moc_exportmessagesdialog.cpp"
