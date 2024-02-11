/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamselectdeletedroomwidget.h"
#include "model/teamroomsfilterproxymodel.h"
#include "model/teamroomsmodel.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

TeamSelectDeletedRoomWidget::TeamSelectDeletedRoomWidget(QWidget *parent)
    : QWidget(parent)
    , mListView(new QListView(this))
    , mSearchLineEdit(new QLineEdit(this))
    , mTeamRoomsModel(new TeamRoomsModel(this))
    , mTeamSelectProxyModel(new TeamRoomsFilterProxyModel(mTeamRoomsModel, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &TeamSelectDeletedRoomWidget::slotTextChanged);

    mainLayout->addWidget(mSearchLineEdit);

    mSearchLineEdit->setPlaceholderText(i18n("Search room..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);

    mListView->setObjectName(QStringLiteral("mListView"));
    mainLayout->addWidget(mListView);

    mTeamRoomsModel->setIsCheckable(true);
    mListView->setModel(mTeamSelectProxyModel);
}

TeamSelectDeletedRoomWidget::~TeamSelectDeletedRoomWidget() = default;

void TeamSelectDeletedRoomWidget::slotTextChanged(const QString &str)
{
    mTeamSelectProxyModel->setFilterString(str);
}

void TeamSelectDeletedRoomWidget::setTeamRooms(const QVector<TeamRoom> &rooms)
{
    mTeamRoomsModel->setTeamRooms(rooms);
}

QStringList TeamSelectDeletedRoomWidget::roomsId() const
{
    QStringList lst;
    for (int i = 0, cnt = mTeamRoomsModel->rowCount(); i < cnt; ++i) {
        const QModelIndex idx = mTeamRoomsModel->index(i, 0, QModelIndex());
        if (mTeamRoomsModel->data(idx, Qt::CheckStateRole).toInt() == Qt::Checked) {
            lst.append(mTeamRoomsModel->data(idx, TeamRoomsModel::Identifier).toString());
        }
    }
    return lst;
}

#include "moc_teamselectdeletedroomwidget.cpp"
