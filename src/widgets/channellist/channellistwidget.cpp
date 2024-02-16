/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistwidget.h"
#include "channellistview.h"
#include "model/roomfilterproxymodel.h"
#include "room/roomutil.h"
#include "ruqola_jitsi_debug.h"
#include "ruqolawidgets_debug.h"

#include "accountmanager.h"
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
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins({});

    mChannelView->setObjectName(QStringLiteral("mChannelView"));
    mChannelView->setProperty("_breeze_force_frame", false);
    mainLayout->addWidget(mChannelView);
    connect(mChannelView, &ChannelListView::selectMessageIdRequested, this, &ChannelListWidget::selectMessageIdRequested);
    connect(mChannelView, &ChannelListView::roomPressed, this, &ChannelListWidget::roomPressed);
    connect(mChannelView, &ChannelListView::roomSelected, this, [this](const ChannelListView::ChannelSelectedInfo &roomInfo) {
        // retain focus on the search line edit when this is triggering the room change
        const auto wasFocused = mSearchRoomLineEdit->hasFocus();

        Q_EMIT roomSelected(roomInfo);

        if (wasFocused) {
            mSearchRoomLineEdit->setFocus();
        }
    });

    // dummy action just for getting the icon)
    mSearchRoomLineEdit->addAction(QIcon::fromTheme(QStringLiteral("view-filter")), QLineEdit::LeadingPosition);
    mSearchRoomLineEdit->setObjectName(QStringLiteral("mSearchRoom"));
    mSearchRoomLineEdit->setPlaceholderText(i18n("Filter channels (CTRL + K)"));
    mSearchRoomLineEdit->setClearButtonEnabled(true);
    mSearchRoomLineEdit->installEventFilter(this);
    mSearchRoomLineEdit->setProperty("_breeze_borders_sides", QVariant::fromValue(QFlags{Qt::TopEdge}));
    mSearchRoomLineEdit->setMinimumHeight(36); // match the default size of the message text field
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
        mChannelView->selectNextChannel(ChannelListView::Direction::Up);
        mSearchRoomLineEdit->clear();
    });
    addAction(previousRoomAction); // TODO: Add to MainWindow's action collection instead?

    auto nextRoomAction = new QAction(i18n("Next Channel"), this);
    nextRoomAction->setShortcut(Qt::CTRL | Qt::Key_Down);
    connect(nextRoomAction, &QAction::triggered, this, [this]() {
        mChannelView->selectNextChannel(ChannelListView::Direction::Down);
        mSearchRoomLineEdit->clear();
    });
    addAction(nextRoomAction); // TODO: Add to MainWindow's action collection instead?
    // END: Actions
}

ChannelListWidget::~ChannelListWidget() = default;

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
    connect(mCurrentRocketChatAccount, &RocketChatAccount::selectChannelAndMessage, this, &ChannelListWidget::slotSelectMessageRequested);

    mChannelView->setCurrentRocketChatAccount(account);
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
            // The search line edit wants to restore focus to itself, but Enter is supposed to
            // explicitly switch to the message line edit, so distract it from doing that
            setFocus();
            applyChannelSelection();
        } else if (keyValue == Qt::Key_Up || keyValue == Qt::Key_Down) {
            mChannelView->selectNextChannel(keyValue == Qt::Key_Up ? ChannelListView::Direction::Up : ChannelListView::Direction::Down);
            return true; // eat event
        } else if (keyValue == Qt::Key_Escape) {
            mSearchRoomLineEdit->clear();
        }
    }

    return QWidget::eventFilter(object, event);
}

void ChannelListWidget::slotAccountInitialized()
{
    mChannelView->selectChannelRequested(mCurrentRocketChatAccount->settings()->lastSelectedRoom(), QString());
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

void ChannelListWidget::slotSelectMessageRequested(const QString &messageId,
                                                   const QString &roomId,
                                                   ParseMessageUrlUtils::RoomIdType roomType,
                                                   ParseMessageUrlUtils::ChannelType channelType)
{
    switch (roomType) {
    case ParseMessageUrlUtils::RoomIdType::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "Room type undefined!";
        break;
    case ParseMessageUrlUtils::RoomIdType::RoomId: {
        const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
        if (selectedIndex.isValid()) {
            const QString currentRoomId = selectedIndex.data(RoomModel::RoomId).toString();
            if (roomId == currentRoomId) {
                Q_EMIT selectMessageIdRequested(messageId);
                return;
            }
            switch (channelType) {
            case ParseMessageUrlUtils::ChannelType::Channel: {
                if (!mChannelView->selectChannelByRoomIdRequested(roomId)) {
                    mCurrentRocketChatAccount->openChannel(roomId, RocketChatAccount::ChannelTypeInfo::RoomId);
                    // TODO implement scroll to message
                } else {
                    Q_EMIT selectMessageIdRequested(messageId);
                }
                break;
            }
            case ParseMessageUrlUtils::ChannelType::Group: {
                // TODO ?
                if (!mChannelView->selectChannelByRoomIdRequested(roomId)) {
                    mCurrentRocketChatAccount->openChannel(roomId, RocketChatAccount::ChannelTypeInfo::RoomId);
                    // TODO implement scroll to message
                } else {
                    Q_EMIT selectMessageIdRequested(messageId);
                }
                break;
            }
            case ParseMessageUrlUtils::ChannelType::Direct: {
                if (!mChannelView->selectChannelByRoomIdRequested(roomId)) {
                    // TODO add support for roomId or roomName
                    // mCurrentRocketChatAccount->openDirectChannel(roomId /*, RocketChatAccount::ChannelTypeInfo::RoomId*/);
                    // Workaround RC 4.7.x where openDirectChannel doesn't accept userId as direct open channel REST API
                    mCurrentRocketChatAccount->ddp()->openDirectChannel(roomId);
                    // TODO implement scroll to message
                } else {
                    Q_EMIT selectMessageIdRequested(messageId);
                }
                break;
            }
            case ParseMessageUrlUtils::ChannelType::Unknown: {
                qCWarning(RUQOLAWIDGETS_LOG) << "ChannelType undefined!";
                break;
            }
            }
        }
        break;
    }
    case ParseMessageUrlUtils::RoomIdType::RoomName: {
        const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
        if (selectedIndex.isValid()) {
            const QString currentRoomName = selectedIndex.data(RoomModel::RoomName).toString();
            if (roomId == currentRoomName) {
                Q_EMIT selectMessageIdRequested(messageId);
                return;
            }
            switch (channelType) {
            case ParseMessageUrlUtils::ChannelType::Channel: {
                if (!mChannelView->selectChannelByRoomNameRequested(roomId)) {
                    mCurrentRocketChatAccount->openChannel(roomId, RocketChatAccount::ChannelTypeInfo::RoomName);
                    // TODO implement scroll to message
                } else {
                    Q_EMIT selectMessageIdRequested(messageId);
                }
                break;
            }
            case ParseMessageUrlUtils::ChannelType::Direct: {
                if (!mChannelView->selectChannelByRoomNameRequested(roomId)) {
                    // TODO add support for roomId or roomName
                    mCurrentRocketChatAccount->openDirectChannel(roomId /*, RocketChatAccount::ChannelTypeInfo::RoomName*/);
                } else {
                    Q_EMIT selectMessageIdRequested(messageId);
                }
                break;
            }
            case ParseMessageUrlUtils::ChannelType::Group: {
                if (!mChannelView->selectChannelByRoomNameRequested(roomId)) {
                    // TODO add support for roomId or roomName
                    mCurrentRocketChatAccount->openDirectChannel(roomId /*, RocketChatAccount::ChannelTypeInfo::RoomName*/);
                } else {
                    Q_EMIT selectMessageIdRequested(messageId);
                }
                break;
            }
            case ParseMessageUrlUtils::ChannelType::Unknown: {
                qCWarning(RUQOLAWIDGETS_LOG) << "ChannelType undefined!";
                break;
            }
            }
        }
        break;
    }
    }
}

void ChannelListWidget::slotOpenLinkRequested(const QString &link)
{
    // qDebug() << " void ChannelListWidget::slotOpenLinkRequested(const QString &link)" << link;
    if (link.startsWith(QLatin1String("ruqola:"))) {
        const QString roomOrUserId = RuqolaUtils::self()->extractRoomUserFromUrl(link);
        const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
        if (selectedIndex.isValid()) {
            const QString currentRoomId = selectedIndex.data(RoomModel::RoomId).toString();
            if (roomOrUserId == currentRoomId) {
                return;
            }
        }
        if (link.startsWith(QLatin1String("ruqola:/room/"))) {
            if (!mChannelView->selectChannelByRoomIdRequested(roomOrUserId)) {
                mCurrentRocketChatAccount->openChannel(roomOrUserId, RocketChatAccount::ChannelTypeInfo::RoomId);
            }
        } else if (link.startsWith(QLatin1String("ruqola:/user/"))) {
            if (!Utils::validUser(roomOrUserId)) {
                return;
            }
            if (!mChannelView->selectChannelByRoomIdRequested(roomOrUserId)) {
                if (roomOrUserId != mCurrentRocketChatAccount->userName()) {
                    if (mCurrentRocketChatAccount->hasPermission(QStringLiteral("create-d"))) {
                        // Workaround RC 4.7.x where openDirectChannel doesn't accept userId as direct open channel REST API
                        mCurrentRocketChatAccount->ddp()->openDirectChannel(roomOrUserId);
                    }

                    // mCurrentRocketChatAccount->openDirectChannel(roomOrUserId);
                }
            }
        } else if (link == QLatin1String("ruqola:/jitsicall/")) {
            const QModelIndex jitsiSelectedIndex = mChannelView->selectionModel()->currentIndex();
            if (jitsiSelectedIndex.isValid()) {
                const QString roomId = jitsiSelectedIndex.data(RoomModel::RoomId).toString();
                qCDebug(RUQOLA_JITSI_LOG) << " roomId " << roomId;
                mCurrentRocketChatAccount->joinJitsiConfCall(roomId);
            }
        }
    } else {
        ParseMessageUrlUtils parseUrl;
        if (parseUrl.parseUrl(link)) {
            if (Ruqola::self()->accountManager()->showMessage(parseUrl)) {
                return;
            }
        }
        RuqolaUtils::self()->openUrl(link);
    }
}

void ChannelListWidget::setLayoutSpacing(int spacing)
{
    layout()->setSpacing(spacing);
}

void ChannelListWidget::applyChannelSelection()
{
    const auto selectedIndex = mChannelView->selectionModel()->currentIndex();
    if (selectedIndex.isValid()) {
        mChannelView->channelSelected(selectedIndex);
        mSearchRoomLineEdit->clear();
    }
}

#include "moc_channellistwidget.cpp"
