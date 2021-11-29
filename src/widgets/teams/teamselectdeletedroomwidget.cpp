/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teamselectdeletedroomwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/teamroomsfilterproxymodel.h"
#include "model/teamroomsmodel.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
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

    mSearchLineEdit->setPlaceholderText(i18n("Search Room..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

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
