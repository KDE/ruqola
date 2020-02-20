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

#include "createnewdiscussiondialog.h"
#include "createnewdiscussionwidget.h"
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPushButton>

namespace {
static const char myConfigGroupName[] = "CreateNewDiscussionDialog";
}
CreateNewDiscussionDialog::CreateNewDiscussionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Create Discussion"));
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateNewDiscussionWidget = new CreateNewDiscussionWidget(this);
    mCreateNewDiscussionWidget->setObjectName(QStringLiteral("mCreateNewDiscussionWidget"));
    mainLayout->addWidget(mCreateNewDiscussionWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateNewDiscussionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateNewDiscussionDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
    QPushButton *button = buttonBox->button(QDialogButtonBox::Ok);
    button->setText(i18n("Create"));
    button->setEnabled(false);
    connect(mCreateNewDiscussionWidget, &CreateNewDiscussionWidget::updateOkButton, this, [button](bool state) {
        button->setEnabled(state);
    });
}

CreateNewDiscussionDialog::~CreateNewDiscussionDialog()
{
    writeConfig();
}

void CreateNewDiscussionDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void CreateNewDiscussionDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

CreateNewDiscussionDialog::NewDiscussionInfo CreateNewDiscussionDialog::newDiscussionInfo() const
{
    NewDiscussionInfo info;
    info.channelName = mCreateNewDiscussionWidget->channelName();
    info.message = mCreateNewDiscussionWidget->message();
    info.users = mCreateNewDiscussionWidget->usersId();
    info.discussionName = mCreateNewDiscussionWidget->discussionName();
    return info;
}

void CreateNewDiscussionDialog::setChannelName(const QString &name)
{
    mCreateNewDiscussionWidget->setChannelName(name);
}

void CreateNewDiscussionDialog::setDiscussionName(const QString &name)
{
    mCreateNewDiscussionWidget->setDiscussionName(name);
}
