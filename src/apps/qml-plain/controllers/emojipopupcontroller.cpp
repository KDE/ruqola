/*
 * Copyright 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "emojipopupcontroller.h"

#include "accountmanager.h"
#include "rocketchataccount.h"
#include "ruqolaqmlplain_debug.h"

#include "QtQuick/private/qquicktextedit_p.h"
#include <QDebug>
#include <QQuickItem>


namespace RuqolaQmlPlain {
namespace Controllers {

EmojiPopupController::EmojiPopupController(AccountManager *accountManager, QObject *parent)
    : QObject(parent)
    , mAccountManager(accountManager)
{
}

int EmojiPopupController::xpos() const
{
    return mXpos;
}

int EmojiPopupController::ypos() const
{
    return mYpos;
}

void EmojiPopupController::placePopup(
    int desiredTopLeftX, int desiredTopLeftY, int popupWidth, int popupHeight, int containerWidth,
    int containerHeight)
{
    mXpos = desiredTopLeftX;
    mYpos = desiredTopLeftY;

    qDebug() << "ASDFASDFDASF" << mXpos << mYpos << popupWidth << popupHeight << containerWidth
             << containerHeight;

    mYpos = std::max(mYpos, 10);
    mYpos = std::min(mYpos, containerHeight - popupHeight - 10);

    mXpos = std::max(mXpos, 10);
    mXpos = std::min(mXpos, containerWidth - popupWidth - 10);

    qDebug() << "ASDFASDFDASF" << mXpos << mYpos << popupWidth << popupHeight << containerWidth
             << containerHeight;

    Q_EMIT xposChanged();
    Q_EMIT yposChanged();
}

bool EmojiPopupController::visible() const
{
    return mPickMode != EmojiPopupController::PickMode::PopupNotShown;
}

void EmojiPopupController::setPickMode(EmojiPopupController::PickMode mode)
{
    if (mode != mPickMode) {
        mPickMode = mode;
        Q_EMIT open();
    }
}

void EmojiPopupController::startPickForMessageReaction(const QString &messageId)
{
    mMessageIdTemp = messageId;
    setPickMode(EmojiPopupController::PickMode::MessageReaction);
}

void EmojiPopupController::startPickForMessageTyping(QQuickTextEdit *textMessageItem)
{
    mTextMessageItemTemp = textMessageItem;
    setPickMode(EmojiPopupController::PickMode::TypingMessage);
}

void EmojiPopupController::pick(const QString &emoji)
{
    switch (mPickMode) {
    case EmojiPopupController::PickMode::PopupNotShown:
        qCWarning(RUQOLAQMLPLAIN_LOG)
            << Q_FUNC_INFO << QStringLiteral("Can't pick an emoji while the popup is not visible");
        break;
    case EmojiPopupController::PickMode::MessageReaction:
        // TODO: make sure account is valid?
        mAccountManager->account()->reactOnMessage(mMessageIdTemp, emoji, true);
        break;
    case EmojiPopupController::PickMode::TypingMessage:
        if (!mTextMessageItemTemp) {
            qCWarning(RUQOLAQMLPLAIN_LOG) << Q_FUNC_INFO << "Invalid text item";
            break;
        }
        mTextMessageItemTemp->setText(mTextMessageItemTemp->text() + emoji);
        break;
    }

    Q_EMIT close();
}

void EmojiPopupController::reset()
{
    mTextMessageItemTemp = nullptr;
    mMessageIdTemp = QStringLiteral("");
    if (mPickMode != PickMode::PopupNotShown) {
        mPickMode = PickMode::PopupNotShown;
        Q_EMIT close();
    }
}

} // namespace Controllers
} // namespace RuqolaQmlPlain