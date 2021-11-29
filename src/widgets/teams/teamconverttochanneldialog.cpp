/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
