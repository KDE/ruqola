/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "channellistview.h"
#include "channellistwidget.h"
#include "statuscombobox.h"
#include "dialogs/modifystatusdialog.h"
#include "model/statusmodel.h"
#include "model/roomfilterproxymodel.h"

#include "ruqola.h"
#include "rocketchataccount.h"
#include "ruqolautils.h"

#include <KMessageBox>

#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <KLocalizedString>
#include <QLineEdit>
#include <QKeyEvent>
#include <QPointer>

ChannelListWidget::ChannelListWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mSearchRoom = new QLineEdit(this);
    // dummy action just for getting the icon)
    mSearchRoom->addAction(QIcon::fromTheme(QStringLiteral("view-filter")), QLineEdit::LeadingPosition);
    mSearchRoom->setObjectName(QStringLiteral("mSearchRoom"));
    mSearchRoom->setPlaceholderText(i18n("Search Rooms (CTRL + K)"));
    mSearchRoom->setClearButtonEnabled(true);
    mSearchRoom->installEventFilter(this);
    mainLayout->addWidget(mSearchRoom);
    connect(mSearchRoom, &QLineEdit::textChanged, this, &ChannelListWidget::slotSearchRoomTextChanged);

    mChannelView = new ChannelListView(this);
    mChannelView->setObjectName(QStringLiteral("mChannelView"));
    mainLayout->addWidget(mChannelView);
    connect(mChannelView, &ChannelListView::channelSelected, this, &ChannelListWidget::channelSelected);

    auto *statusComboBoxLayout = new QHBoxLayout;
    mainLayout->addLayout(statusComboBoxLayout);
    QLabel *label = new QLabel(i18n("Status:"), this);
    label->setObjectName(QStringLiteral("label"));
    statusComboBoxLayout->addWidget(label);

    mStatusComboBox = new StatusCombobox(true, this);
    mStatusComboBox->setObjectName(QStringLiteral("mStatusComboBox"));
    statusComboBoxLayout->addWidget(mStatusComboBox);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect(mStatusComboBox, QOverload<int>::of(&StatusCombobox::currentIndexChanged), this, &ChannelListWidget::slotStatusChanged);
#else
    connect(mStatusComboBox, QOverload<int, const QString &>::of(&StatusCombobox::currentIndexChanged), this, &ChannelListWidget::slotStatusChanged);
#endif

    //BEGIN: Actions
    auto searchRoomAction = new QAction(i18n("Search Rooms"), this);
    searchRoomAction->setShortcut(Qt::CTRL + Qt::Key_K);
    connect(searchRoomAction, &QAction::triggered, this, [this]() {
        mSearchRoom->setFocus();
    });
    addAction(searchRoomAction); // TODO: Add to MainWindow's action collection instead?
    //END: Actions
}

ChannelListWidget::~ChannelListWidget()
{
}

void ChannelListWidget::clearFilterChannel()
{
    if (auto *model = mChannelView->model()) {
        model->setFilterString(QString());
        mSearchRoom->clear();
    }
}

void ChannelListWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    clearFilterChannel();
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, nullptr, this, nullptr);
    }
    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::userStatusUpdated, this, &ChannelListWidget::setUserStatusUpdated);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::openLinkRequested, this, &ChannelListWidget::slotOpenLinkRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::selectRoomByRoomNameRequested, mChannelView, &ChannelListView::selectChannelByRoomNameRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::selectRoomByRoomIdRequested, mChannelView, &ChannelListView::selectChannelRequested);


    mChannelView->setModel(mCurrentRocketChatAccount->roomFilterProxyModel());
}

ChannelListView *ChannelListWidget::channelListView() const
{
    return mChannelView;
}

QString ChannelListWidget::currentSelectedRoom() const
{
    if (mChannelView->selectionModel()) { //For autotest
        const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
        if (selectedIndex.isValid()) {
            return selectedIndex.data(Qt::DisplayRole).toString();
        }
    }
    return QString();
}

bool ChannelListWidget::eventFilter(QObject *object, QEvent *event)
{
    if (object == mSearchRoom && event->type() == QEvent::KeyPress) {
        const auto *model = mChannelView->model();
        const auto *keyEvent = static_cast<QKeyEvent *>(event);
        const int keyValue = keyEvent->key();
        if (keyValue == Qt::Key_Return || keyValue == Qt::Key_Enter) {
            const auto selectedIndex = mChannelView->selectionModel()->currentIndex();
            if (selectedIndex.isValid()) {
                Q_EMIT channelSelected(selectedIndex);
                mSearchRoom->setText({});
            }
        } else if (keyValue == Qt::Key_Up || keyValue == Qt::Key_Down) {
            const QModelIndex currentIndex = mChannelView->selectionModel()->currentIndex();
            int selectRow = -1;
            if (keyValue == Qt::Key_Up) {
                if (!currentIndex.isValid()) {
                    selectRow = model->rowCount() - 1;
                } else if (currentIndex.row()-1 >= 0) {
                    selectRow = currentIndex.row() - 1;
                }
            } else { // Qt::Key_Down
                if (!currentIndex.isValid()) {
                    selectRow = 0;
                } else if (currentIndex.row()+1 < model->rowCount()) {
                    selectRow = currentIndex.row() + 1;
                }
            }

            if (selectRow != -1) {
                mChannelView->selectionModel()->setCurrentIndex(model->index(selectRow, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            }
            return true; // eat event
        }
    }

    return QWidget::eventFilter(object, event);
}

void ChannelListWidget::setUserStatusUpdated(User::PresenceStatus status)
{
    mStatusComboBox->setStatus(status);
}

void ChannelListWidget::slotStatusChanged()
{
    User::PresenceStatus status = mStatusComboBox->status();
    QString messageStatus;
    if (status == User::PresenceStatus::Unknown) {
        QPointer<ModifyStatusDialog> dlg = new ModifyStatusDialog(this);
        dlg->setMessageStatus(Ruqola::self()->rocketChatAccount()->statusModel()->currentStatusInfo().displayText);
        dlg->setStatus(Ruqola::self()->rocketChatAccount()->statusModel()->currentStatusInfo().status);
        if (dlg->exec()) {
            messageStatus = dlg->messageStatus();
            status = dlg->status();
            delete dlg;
        } else {
            mStatusComboBox->setStatus(Ruqola::self()->rocketChatAccount()->statusModel()->currentUserStatus());
            delete dlg;
            return;
        }
    }
    Ruqola::self()->rocketChatAccount()->setDefaultStatus(status, messageStatus);
}

void ChannelListWidget::slotSearchRoomTextChanged()
{
    mChannelView->model()->setFilterString(mSearchRoom->text());
}

void ChannelListWidget::slotOpenLinkRequested(const QString &link)
{
    if (link.startsWith(QLatin1String("ruqola:"))) {
        const QString roomOrUser = RuqolaUtils::self()->extractRoomUserFromUrl(link);
        const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
        if (selectedIndex.isValid()) {
            const QString currentRoomId = selectedIndex.data(RoomModel::RoomName).toString();
            if (roomOrUser == currentRoomId) {
                return;
            }
        }
        if (link.startsWith(QLatin1String("ruqola:/room/"))) {
            if (!mChannelView->selectChannelByRoomNameRequested(roomOrUser)) {
                mCurrentRocketChatAccount->openChannel(roomOrUser, RocketChatAccount::ChannelTypeInfo::RoomName);
            }
        } else if (link.startsWith(QLatin1String("ruqola:/user/"))) {
            if (!mChannelView->selectChannelByRoomNameRequested(roomOrUser)) {
                if (roomOrUser != mCurrentRocketChatAccount->userName()) {
                    if( KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to open direct conversation with %1", roomOrUser))) {
                        mCurrentRocketChatAccount->openDirectChannel(roomOrUser);
                    }
                }
            }
        } else if (link == QLatin1String("ruqola:/jitsicall/")) {
            const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
            if (selectedIndex.isValid()) {
                const QString roomId = selectedIndex.data(RoomModel::RoomID).toString();
                mCurrentRocketChatAccount->joinJitsiConfCall(roomId);
            }
        }
    } else {
        RuqolaUtils::self()->openUrl(link);
    }
}
