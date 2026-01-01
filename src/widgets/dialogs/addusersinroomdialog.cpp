/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addusersinroomdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "addusersinroomwidget.h"
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
const char myConfigAddUsersInRoomDialogGroupName[] = "AddUsersInRoomDialog";
}
AddUsersInRoomDialog::AddUsersInRoomDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mAddUsersInRoomWidget(new AddUsersInRoomWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Add Users in Room"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mAddUsersInRoomWidget->setObjectName(u"mAddUsersInRoomWidget"_s);
    mainLayout->addWidget(mAddUsersInRoomWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AddUsersInRoomDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AddUsersInRoomDialog::accept);
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    connect(mAddUsersInRoomWidget, &AddUsersInRoomWidget::updateOkButton, okButton, &QPushButton::setEnabled);
    okButton->setEnabled(false);
    readConfig();
}

AddUsersInRoomDialog::~AddUsersInRoomDialog()
{
    writeConfig();
}

QList<QByteArray> AddUsersInRoomDialog::userIds() const
{
    return mAddUsersInRoomWidget->userIds();
}

QStringList AddUsersInRoomDialog::userNames() const
{
    return mAddUsersInRoomWidget->userNames();
}

void AddUsersInRoomDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 300));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigAddUsersInRoomDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void AddUsersInRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigAddUsersInRoomDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_addusersinroomdialog.cpp"
