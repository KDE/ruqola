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

#include "channellistview.h"
#include "channellistwidget.h"
#include "statuscombobox.h"
#include "dialogs/modifystatusdialog.h"

#include "ruqola.h"
#include "rocketchataccount.h"

#include <QHBoxLayout>
#include <QLabel>
#include <KLocalizedString>
#include <QLineEdit>
#include <QDebug>
#include <QPointer>

ChannelListWidget::ChannelListWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mSearchRoom = new QLineEdit(this);
    mSearchRoom->setObjectName(QStringLiteral("mSearchRoom"));
    mSearchRoom->setClearButtonEnabled(true);
    mainLayout->addWidget(mSearchRoom);

    mChannelView = new ChannelListView(this);
    mChannelView->setObjectName(QStringLiteral("mChannelView"));
    mainLayout->addWidget(mChannelView);
    connect(mChannelView, &ChannelListView::channelSelected, this, &ChannelListWidget::channelSelected);

    QHBoxLayout *statusComboBoxLayout = new QHBoxLayout;
    mainLayout->addLayout(statusComboBoxLayout);
    QLabel *label = new QLabel(i18n("Status:"), this);
    label->setObjectName(QStringLiteral("label"));
    statusComboBoxLayout->addWidget(label);

    mStatusComboBox = new StatusCombobox(true, this);
    mStatusComboBox->setObjectName(QStringLiteral("mStatusComboBox"));
    statusComboBoxLayout->addWidget(mStatusComboBox);
    connect(mStatusComboBox, QOverload<int>::of(&StatusCombobox::currentIndexChanged), this, &ChannelListWidget::slotStatusChanged);

    //TODO fix multiaccount
    connect(Ruqola::self()->rocketChatAccount(), &RocketChatAccount::userStatusUpdated, this, &ChannelListWidget::setUserStatusUpdated);
}

ChannelListWidget::~ChannelListWidget()
{
}

void ChannelListWidget::setUserStatusUpdated(User::PresenceStatus status)
{
    mStatusComboBox->setStatus(status);
}

void ChannelListWidget::slotStatusChanged()
{
    User::PresenceStatus status = mStatusComboBox->status();
    QString messageStatus;
    if (status == User::PresenceStatus::Unknown) {
        QPointer<ModifyStatusDialog> dlg = new ModifyStatusDialog(this);
        //Set current Status
        if (dlg->exec()) {
            messageStatus = dlg->messageStatus();
            status = dlg->status();
            delete dlg;
        } else {
            //restore status
            delete dlg;
            return;
        }
    }
    Ruqola::self()->rocketChatAccount()->setDefaultStatus(status, messageStatus);
}
