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
#include "model/directoryroomsmodel.h"
#include "model/directoryteamsmodel.h"
#include "model/directoryusersmodel.h"
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
    mLabelResultSearch->setTextFormat(Qt::RichText);
    mLabelResultSearch->setContextMenuPolicy(Qt::NoContextMenu);
    QFont labFont = mLabelResultSearch->font();
    labFont.setBold(true);
    mLabelResultSearch->setFont(labFont);
    connect(mLabelResultSearch, &QLabel::linkActivated, this, &DirectoryWidget::loadMoreElements);

    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mTreeView->setRootIsDecorated(false);
    mainLayout->addWidget(mTreeView);

    switch (mType) {
    case Room:
        mSearchLineEdit->setPlaceholderText(i18n("Search Channels"));
        mModel = new DirectoryRoomsModel(this);
        break;
    case User:
        mSearchLineEdit->setPlaceholderText(i18n("Search Users"));
        mModel = new DirectoryUsersModel(this);
        break;
    case Team:
        mSearchLineEdit->setPlaceholderText(i18n("Search Teams"));
        mModel = new DirectoryTeamsModel(this);
        break;
    case Unknown:
        break;
    }
    mTreeView->setModel(mModel);
    connect(mModel, &DirectoryBaseModel::hasFullListChanged, this, &DirectoryWidget::updateLabel);
    connect(mModel, &DirectoryBaseModel::totalChanged, this, &DirectoryWidget::updateLabel);
    connect(mModel, &DirectoryBaseModel::loadingInProgressChanged, this, &DirectoryWidget::updateLabel);
}

DirectoryWidget::~DirectoryWidget()
{
}

void DirectoryWidget::loadMoreElements()
{
    RocketChatRestApi::DirectoryJob::DirectoryInfo info;
    switch (mType) {
    case Room:
        info.searchType = RocketChatRestApi::DirectoryJob::Rooms;
        break;
    case User:
        info.searchType = RocketChatRestApi::DirectoryJob::Users;
        break;
    case Team:
        info.searchType = RocketChatRestApi::DirectoryJob::Teams;
        break;
    case Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "Invalid type it's a bug";
        return;
    }
#if 0
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::DirectoryJob(this);
    job->setDirectoryInfo(info);
    QueryParameters parameters;

    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("ts"), QueryParameters::SortOrder::Descendant);
    parameters.setSorting(map);
    parameters.setCount(count);
    parameters.setOffset(offset);
    job->setQueryParameters(parameters);

    rcAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::DirectoryJob::directoryDone, this, &DirectoryWidget::slotSearchDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start searchRoomUser job";
    }
#endif
    // TODO
}

void DirectoryWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? noFoundInfo() : displayShowMessageInRoom());
}

QString DirectoryWidget::noFoundInfo() const
{
    switch (mType) {
    case Room:
        return i18n("No Room found");
    case User:
        return i18n("No User found");
    case Team:
        return i18n("No Team found");
    case Unknown:
        return {};
    }
    return {};
}

QString DirectoryWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr;
    switch (mType) {
    case Room:
        displayMessageStr = i18np("%1 room (Total: %2)", "%1 rooms (Total: %2)", mModel->rowCount(), mModel->total());
        break;
    case User:
        displayMessageStr = i18np("%1 user (Total: %2)", "%1 users (Total: %2)", mModel->rowCount(), mModel->total());
        break;
    case Team:
        displayMessageStr = i18np("%1 team (Total: %2)", "%1 teams (Total: %2)", mModel->rowCount(), mModel->total());
        break;
    case Unknown:
        break;
    }
    if (!mModel->hasFullList()) {
        displayMessageStr += QStringLiteral(" <a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
    }
    return displayMessageStr;
}

void DirectoryWidget::fillDirectory()
{
    RocketChatRestApi::DirectoryJob::DirectoryInfo info;
    switch (mType) {
    case Room:
        info.searchType = RocketChatRestApi::DirectoryJob::Rooms;
        mSearchLineEdit->setPlaceholderText(i18n("Search Rooms"));
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
    mModel->initialize();
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
    // qDebug() << " obj " << obj;
    mModel->parseElements(obj);
}

DirectoryWidget::DirectoryType DirectoryWidget::type() const
{
    return mType;
}
