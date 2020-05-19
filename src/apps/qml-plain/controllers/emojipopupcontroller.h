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

#pragma once

#include <QObject>

class AccountManager;
class QQuickTextEdit;

namespace RuqolaQmlPlain {
namespace Controllers {

class EmojiPopupController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int xpos READ xpos NOTIFY xposChanged)
    Q_PROPERTY(int ypos READ ypos NOTIFY yposChanged)
    Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged)

    enum class PickMode {
        PopupNotShown,
        MessageReaction,
        TypingMessage,
    };

  public:
    explicit EmojiPopupController(AccountManager *accountManager, QObject *parent = nullptr);
    ~EmojiPopupController() override = default;

    Q_REQUIRED_RESULT int xpos() const;
    Q_REQUIRED_RESULT int ypos() const;
    Q_REQUIRED_RESULT bool visible() const;

    Q_INVOKABLE void placePopup(
        int desiredTopLeftX, int desiredTopLeftY, int popupWidth, int popupHeight,
        int containerWidth, int containerHeight);
    Q_INVOKABLE void startPickForMessageReaction(const QString &messageId);
    Q_INVOKABLE void startPickForMessageTyping(QQuickTextEdit *textMessageItem);
    Q_INVOKABLE void pick(const QString &emoji);
    Q_INVOKABLE void reset();

  Q_SIGNALS:
    void xposChanged();
    void yposChanged();
    void visibleChanged();
    void open();
    void close();

    // void pickMessageReactionDone(const QString &messageId, const QString &emoji);
    // void pickForMessageTypingDone(const QString &emoji);

  private:
    AccountManager *mAccountManager = nullptr;
    QString mMessageIdTemp;
    QQuickTextEdit *mTextMessageItemTemp = nullptr;
    int mXpos = -1;
    int mYpos = -1;
    PickMode mPickMode = PickMode::PopupNotShown;

    void setPickMode(PickMode mode);
};

} // namespace Controllers
} // namespace RuqolaQmlPlain

Q_DECLARE_METATYPE(RuqolaQmlPlain::Controllers::EmojiPopupController *)
