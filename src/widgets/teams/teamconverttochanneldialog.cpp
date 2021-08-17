/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "teamconverttochanneldialog.h"
#include "teamconverttochannelwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
namespace
{
const char myTeamConvertToChannelDialogConfigGroupName[] = "TeamConvertToChannelDialog";
}
TeamConvertToChannelDialog::TeamConvertToChannelDialog(QWidget *parent)
    : QDialog(parent)
    , mTeamConvertToChannelWidget(new TeamConvertToChannelWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Convert Team to Channel"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mTeamConvertToChannelWidget->setObjectName(QStringLiteral("mTeamConvertToChannelWidget"));
    mainLayout->addWidget(mTeamConvertToChannelWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &TeamConvertToChannelDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &TeamConvertToChannelDialog::accept);
    readConfig();
}

TeamConvertToChannelDialog::~TeamConvertToChannelDialog()
{
    writeConfig();
}

QStringList TeamConvertToChannelDialog::roomIdsToDelete() const
{
    return mTeamConvertToChannelWidget->roomIdsToDelete();
}

void TeamConvertToChannelDialog::setTeamRooms(const QVector<TeamRoom> &rooms)
{
    mTeamConvertToChannelWidget->setTeamRooms(rooms);
}

void TeamConvertToChannelDialog::setTeamName(const QString &name)
{
    setWindowTitle(i18nc("@title:window", "Convert Team \'%1\' to Channel", name));
}

void TeamConvertToChannelDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTeamConvertToChannelDialogConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void TeamConvertToChannelDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTeamConvertToChannelDialogConfigGroupName);
    group.writeEntry("Size", size());
}
