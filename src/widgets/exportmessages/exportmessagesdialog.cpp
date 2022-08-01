/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportmessagesdialog.h"
#include "exportmessageswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
namespace
{
const char myExportMessagesDialogConfigGroupName[] = "ExportMessagesDialog";
}
ExportMessagesDialog::ExportMessagesDialog(QWidget *parent)
    : QDialog(parent)
    , mExportMessagesWidget(new ExportMessagesWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Export Messages"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mExportMessagesWidget->setObjectName(QStringLiteral("mExportMessagesWidget"));
    mainLayout->addWidget(mExportMessagesWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ExportMessagesDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ExportMessagesDialog::accept);
    readConfig();
}

ExportMessagesDialog::~ExportMessagesDialog()
{
    writeConfig();
}

RocketChatRestApi::RoomsExportJob::RoomsExportInfo ExportMessagesDialog::roomExportInfo() const
{
    return mExportMessagesWidget->roomExportInfo();
}

void ExportMessagesDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myExportMessagesDialogConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ExportMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myExportMessagesDialogConfigGroupName);
    group.writeEntry("Size", size());
}
