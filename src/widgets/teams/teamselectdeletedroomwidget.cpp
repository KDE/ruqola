/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamselectdeletedroomwidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &TeamSelectDeletedRoomWidget::slotTextChanged);

    mainLayout->addWidget(mSearchLineEdit);

    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search room…"));
    mSearchLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);

    mListView->setObjectName(u"mListView"_s);
    mainLayout->addWidget(mListView);

    mTeamRoomsModel->setIsCheckable(true);
    mListView->setModel(mTeamSelectProxyModel);
}

TeamSelectDeletedRoomWidget::~TeamSelectDeletedRoomWidget() = default;

void TeamSelectDeletedRoomWidget::slotTextChanged(const QString &str)
{
    mTeamSelectProxyModel->setFilterString(str);
}

void TeamSelectDeletedRoomWidget::setTeamRooms(const QList<TeamRoom> &rooms)
{
    mTeamRoomsModel->setTeamRooms(rooms);
}

QList<QByteArray> TeamSelectDeletedRoomWidget::roomsId() const
{
    QList<QByteArray> lst;
    for (int i = 0, cnt = mTeamRoomsModel->rowCount(); i < cnt; ++i) {
        const QModelIndex idx = mTeamRoomsModel->index(i, 0, QModelIndex());
        if (mTeamRoomsModel->data(idx, Qt::CheckStateRole).toInt() == Qt::Checked) {
            lst.append(mTeamRoomsModel->data(idx, TeamRoomsModel::Identifier).toByteArray());
        }
    }
    return lst;
}

#include "moc_teamselectdeletedroomwidget.cpp"
