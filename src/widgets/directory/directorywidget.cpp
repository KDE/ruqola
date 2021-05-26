/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "directorywidget.h"
#include "misc/directoryjob.h"
#include "misc/lineeditcatchreturnkey.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QTreeView>
#include <QVBoxLayout>

DirectoryWidget::DirectoryWidget(DirectoryType type, QWidget *parent)
    : QWidget(parent)
    , mType(type)
    , mLabelResultSearch(new QLabel(this))
    , mSearchLineEdit(new QLineEdit(this))
    , mTreeView(new QTreeView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mainLayout->addWidget(mSearchLineEdit);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

    mLabelResultSearch->setObjectName(QStringLiteral("mLabelResultSearch"));
    mainLayout->addWidget(mLabelResultSearch);

    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mainLayout->addWidget(mTreeView);

    switch (mType) {
    case Room:
        mSearchLineEdit->setPlaceholderText(i18n("Search Channels"));
        break;
    case User:
        mSearchLineEdit->setPlaceholderText(i18n("Search Users"));
        break;
    case Team:
        mSearchLineEdit->setPlaceholderText(i18n("Search Teams"));
        break;
    case Unknown:
        break;
    }
    // TODO add model
}

DirectoryWidget::~DirectoryWidget()
{
}

void DirectoryWidget::fillDirectory()
{
    RocketChatRestApi::DirectoryJob::DirectoryInfo info;
    switch (mType) {
    case Room:
        info.searchType = RocketChatRestApi::DirectoryJob::Rooms;
        break;
    case User:
        info.searchType = RocketChatRestApi::DirectoryJob::Users;
        mSearchLineEdit->setPlaceholderText(i18n("Search Users"));
        break;
    case Team:
        info.searchType = RocketChatRestApi::DirectoryJob::Teams;
        mSearchLineEdit->setPlaceholderText(i18n("Search Teams"));
        break;
    case Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "Invalid type it's a bug";
        return;
    }
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::DirectoryJob(this);
    job->setDirectoryInfo(info);
    rcAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::DirectoryJob::directoryDone, this, &DirectoryWidget::slotSearchDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start searchRoomUser job";
    }
}

void DirectoryWidget::slotSearchDone(const QJsonObject &obj)
{
    qDebug() << " obj " << obj;
    switch (mType) {
    case Room:
        break;
    case User:
        break;
    case Team:
        break;
    case Unknown:
        break;
    }
}

DirectoryWidget::DirectoryType DirectoryWidget::type() const
{
    return mType;
}
