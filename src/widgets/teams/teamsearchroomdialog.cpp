/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsearchroomdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "teamsearchroomwidget.h"

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
const char myTeamSearchRoomDialogConfigGroupName[] = "TeamSearchRoomDialog";
}
TeamSearchRoomDialog::TeamSearchRoomDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mTeamSearchRoomWidget(new TeamSearchRoomWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Search Rooms"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mTeamSearchRoomWidget->setObjectName(u"mTeamSearchRoomWidget"_s);
    mainLayout->addWidget(mTeamSearchRoomWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &TeamSearchRoomDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &TeamSearchRoomDialog::accept);
    auto okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mTeamSearchRoomWidget, &TeamSearchRoomWidget::enableOkButton, okButton, &QPushButton::setEnabled);
    readConfig();
}

TeamSearchRoomDialog::~TeamSearchRoomDialog()
{
    writeConfig();
}

QList<QByteArray> TeamSearchRoomDialog::roomIds() const
{
    return mTeamSearchRoomWidget->roomIds();
}

void TeamSearchRoomDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myTeamSearchRoomDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void TeamSearchRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myTeamSearchRoomDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_teamsearchroomdialog.cpp"
