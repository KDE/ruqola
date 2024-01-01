/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportmessagesdialog.h"
#include "exportmessageswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myExportMessagesDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExportMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myExportMessagesDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_exportmessagesdialog.cpp"
