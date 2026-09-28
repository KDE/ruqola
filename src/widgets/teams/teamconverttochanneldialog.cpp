/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamconverttochanneldialog.h"

#include "teamconverttochannelwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myTeamConvertToChannelDialogConfigGroupName[] = "TeamConvertToChannelDialog";
}
using namespace Qt::Literals::StringLiterals;
TeamConvertToChannelDialog::TeamConvertToChannelDialog(QWidget *parent)
    : QDialog(parent)
    , mTeamConvertToChannelWidget(new TeamConvertToChannelWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Convert Team to Channel"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mTeamConvertToChannelWidget->setObjectName(u"mTeamConvertToChannelWidget"_s);
    mainLayout->addWidget(mTeamConvertToChannelWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &TeamConvertToChannelDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &TeamConvertToChannelDialog::accept);
    readConfig();
}

TeamConvertToChannelDialog::~TeamConvertToChannelDialog() = default;

QList<QByteArray> TeamConvertToChannelDialog::roomIdsToDelete() const
{
    return mTeamConvertToChannelWidget->roomIdsToDelete();
}

void TeamConvertToChannelDialog::setTeamRooms(QList<TeamRoom> rooms)
{
    mTeamConvertToChannelWidget->setTeamRooms(std::move(rooms));
}

void TeamConvertToChannelDialog::setTeamName(const QString &name)
{
    setWindowTitle(i18nc("@title:window", "Convert Team \'%1\' to Channel", name));
}

void TeamConvertToChannelDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myTeamConvertToChannelDialogConfigGroupName), QSize(400, 300));
}

#include "moc_teamconverttochanneldialog.cpp"
