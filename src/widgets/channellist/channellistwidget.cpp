/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "channellistwidget.h"
#include "channellistview.h"
#include "model/roomfilterproxymodel.h"

#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolautils.h"

#include <KLocalizedString>
#include <QAction>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>

ChannelListWidget::ChannelListWidget(QWidget *parent)
    : QWidget(parent)
    , mChannelView(new ChannelListView(this))
    , mSearchRoomLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins({});

    mChannelView->setObjectName(QStringLiteral("mChannelView"));
    mainLayout->addWidget(mChannelView);
    connect(mChannelView, &ChannelListView::roomSelected, this, &ChannelListWidget::roomSelected);

    // dummy action just for getting the icon)
    mSearchRoomLineEdit->addAction(QIcon::fromTheme(QStringLiteral("view-filter")), QLineEdit::LeadingPosition);
    mSearchRoomLineEdit->setObjectName(QStringLiteral("mSearchRoom"));
    mSearchRoomLineEdit->setPlaceholderText(i18n("Filter Channels (CTRL + K)"));
    mSearchRoomLineEdit->setClearButtonEnabled(true);
    mSearchRoomLineEdit->installEventFilter(this);
    mainLayout->addWidget(mSearchRoomLineEdit);
    connect(mSearchRoomLineEdit, &QLineEdit::textChanged, this, &ChannelListWidget::slotSearchRoomTextChanged);

    // BEGIN: Actions
    auto searchRoomAction = new QAction(i18n("Search Channels"), this);
    searchRoomAction->setShortcut(Qt::CTRL | Qt::Key_K);
    connect(searchRoomAction, &QAction::triggered, this, [this]() {
        mSearchRoomLineEdit->setFocus();
    });
    addAction(searchRoomAction); // TODO: Add to MainWindow's action collection instead?

    auto previousRoomAction = new QAction(i18n("Previous Channel"), this);
    previousRoomAction->setShortcut(Qt::CTRL | Qt::Key_Up);
    connect(previousRoomAction, &QAction::triggered, this, [this]() {
        selectNextChannel(Qt::Key_Up);
        applyChannelSelection();
    });
    addAction(previousRoomAction); // TODO: Add to MainWindow's action collection instead?

    auto nextRoomAction = new QAction(i18n("Next Channel"), this);
    nextRoomAction->setShortcut(Qt::CTRL | Qt::Key_Down);
    connect(nextRoomAction, &QAction::triggered, this, [this]() {
        selectNextChannel(Qt::Key_Down);
        applyChannelSelection();
    });
    addAction(nextRoomAction); // TODO: Add to MainWindow's action collection instead?
    // END: Actions
}

ChannelListWidget::~ChannelListWidget()
{
}

void ChannelListWidget::clearFilterChannel()
{
    if (auto *model = mChannelView->filterModel()) {
        model->setFilterString(QString());
        mSearchRoomLineEdit->clear();
    }
}

void ChannelListWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    clearFilterChannel();
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, nullptr, this, nullptr);
    }
    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::accountInitialized, this, &ChannelListWidget::slotAccountInitialized);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::openLinkRequested, this, &ChannelListWidget::slotOpenLinkRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::openTeamNameRequested, this, &ChannelListWidget::slotOpenTeamRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::selectRoomByRoomNameRequested, mChannelView, &ChannelListView::selectChannelByRoomNameRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::selectRoomByRoomIdRequested, mChannelView, &ChannelListView::selectChannelRequested);

    mChannelView->setCurrentRocketChatAccount(account);
    mChannelView->setFilterModel(mCurrentRocketChatAccount->roomFilterProxyModel());
}

ChannelListView *ChannelListWidget::channelListView() const
{
    return mChannelView;
}

bool ChannelListWidget::eventFilter(QObject *object, QEvent *event)
{
    if (object == mSearchRoomLineEdit && event->type() == QEvent::KeyPress) {
        const auto keyEvent = static_cast<QKeyEvent *>(event);
        const int keyValue = keyEvent->key();
        if (keyValue == Qt::Key_Return || keyValue == Qt::Key_Enter) {
            applyChannelSelection();
        } else if (keyValue == Qt::Key_Up || keyValue == Qt::Key_Down) {
            selectNextChannel(keyValue);
            return true; // eat event
        }
    }

    return QWidget::eventFilter(object, event);
}

void ChannelListWidget::slotAccountInitialized()
{
    mChannelView->selectChannelRequested(mCurrentRocketChatAccount->settings()->lastSelectedRoom());
}

void ChannelListWidget::slotSearchRoomTextChanged()
{
    mChannelView->filterModel()->setFilterString(mSearchRoomLineEdit->text());
}

void ChannelListWidget::slotOpenTeamRequested(const QString &identifier)
{
    const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
    if (selectedIndex.isValid()) {
        const QString currentRoomId = selectedIndex.data(RoomModel::RoomId).toString();
        if (identifier == currentRoomId) {
            return;
        }
    }
    if (!mChannelView->selectChannelByRoomIdRequested(identifier)) {
        mCurrentRocketChatAccount->openChannel(identifier, RocketChatAccount::ChannelTypeInfo::RoomId);
    }
}

void ChannelListWidget::slotOpenLinkRequested(const QString &link)
{
    if (link.startsWith(QLatin1String("ruqola:"))) {
        const QString roomOrUser = RuqolaUtils::self()->extractRoomUserFromUrl(link);
        const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
        if (selectedIndex.isValid()) {
            const QString currentRoomName = selectedIndex.data(RoomModel::RoomName).toString();
            if (roomOrUser == currentRoomName) {
                return;
            }
        }
        if (link.startsWith(QLatin1String("ruqola:/room/"))) {
            if (!mChannelView->selectChannelByRoomNameRequested(roomOrUser)) {
                mCurrentRocketChatAccount->openChannel(roomOrUser, RocketChatAccount::ChannelTypeInfo::RoomName);
            }
        } else if (link.startsWith(QLatin1String("ruqola:/user/"))) {
            if (roomOrUser == QLatin1String("here") || roomOrUser == QLatin1String("all")) {
                return;
            }
            if (!mChannelView->selectChannelByRoomNameRequested(roomOrUser)) {
                if (roomOrUser != mCurrentRocketChatAccount->userName()) {
                    mCurrentRocketChatAccount->openDirectChannel(roomOrUser);
                }
            }
        } else if (link == QLatin1String("ruqola:/jitsicall/")) {
            const QModelIndex jitsiSelectedIndex = mChannelView->selectionModel()->currentIndex();
            if (jitsiSelectedIndex.isValid()) {
                const QString roomId = jitsiSelectedIndex.data(RoomModel::RoomId).toString();
                mCurrentRocketChatAccount->joinJitsiConfCall(roomId);
            }
        }
    } else {
        RuqolaUtils::self()->openUrl(link);
    }
}

void ChannelListWidget::setLayoutSpacing(int spacing)
{
    layout()->setSpacing(spacing);
}

void ChannelListWidget::selectNextChannel(int direction)
{
    Q_ASSERT(direction == Qt::Key_Up || direction == Qt::Key_Down);

    const auto *model = mChannelView->model();
    const QModelIndex currentIndex = mChannelView->selectionModel()->currentIndex();
    const auto rowCount = model->rowCount();
    int selectRow = -1;
    if (direction == Qt::Key_Up) {
        if (!currentIndex.isValid()) {
            selectRow = rowCount - 1;
        } else if (currentIndex.row() - 1 >= 0) {
            selectRow = currentIndex.row() - 1;
        }
    } else { // Qt::Key_Down
        if (!currentIndex.isValid() || (currentIndex.row() + 1) == rowCount) {
            selectRow = 0;
        } else if (currentIndex.row() + 1 < rowCount) {
            selectRow = currentIndex.row() + 1;
        }
    }

    while (selectRow != -1 && selectRow != currentIndex.row() && !model->index(selectRow, 0).flags().testFlag(Qt::ItemIsSelectable)) {
        if (direction == Qt::Key_Up) {
            --selectRow;
            if (selectRow == -1) {
                selectRow = rowCount - 1;
            }
        } else { // Qt::Key_Down
            ++selectRow;
            if (selectRow == rowCount) {
                selectRow = 0;
            }
        }
    }

    if (selectRow != -1) {
        mChannelView->selectionModel()->setCurrentIndex(model->index(selectRow, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
}

void ChannelListWidget::applyChannelSelection()
{
    const auto selectedIndex = mChannelView->selectionModel()->currentIndex();
    if (selectedIndex.isValid()) {
        mChannelView->channelSelected(selectedIndex);
        mSearchRoomLineEdit->setText({});
    }
}
