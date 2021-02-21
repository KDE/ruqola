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

#pragma once

#include "libruqolawidgets_export.h"
#include <QWidget>
class KPasswordLineEdit;
class QCheckBox;
class QPushButton;
class QStackedWidget;
class QLabel;
class Room;
class QToolButton;
class SystemMessagesComboBox;
class ChannelInfoPruneWidget;
class LIBRUQOLAWIDGETS_EXPORT ChangeTextWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChangeTextWidget(QWidget *parent = nullptr);
    ~ChangeTextWidget() override;
    void setAllowEmptyText(bool b);
    void setText(const QString &str);

    void setLabelText(const QString &str);
    Q_REQUIRED_RESULT bool allowEmptyText() const;

Q_SIGNALS:
    void textChanged(const QString &str);

private:
    void slotChangeText();
    QString mLabelText;
    bool mAllowEmptyText = false;
    QLabel *mLabel = nullptr;
    QToolButton *mChangeTextToolButton = nullptr;
};

class LIBRUQOLAWIDGETS_EXPORT ChannelInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelInfoWidget(QWidget *parent = nullptr);
    ~ChannelInfoWidget() override;

    void setRoom(Room *room);

Q_SIGNALS:
    void channelDeleted();
    void fnameChanged(const QString &fname);

private:
    void updateReadOnlyChannelInfo();
    void updateEditableChannelInfo();
    void connectReadOnlyWidget();
    void connectEditableWidget();
    void joinCodeChanged();

    ChangeTextWidget *mName = nullptr;
    ChangeTextWidget *mComment = nullptr;
    ChangeTextWidget *mAnnouncement = nullptr;
    ChangeTextWidget *mDescription = nullptr;
    KPasswordLineEdit *mPasswordLineEdit = nullptr;
    QCheckBox *mReadOnly = nullptr;
    QCheckBox *mArchive = nullptr;
    QCheckBox *mPrivate = nullptr;
    QCheckBox *mEncrypted = nullptr;
    QPushButton *mDeleteChannel = nullptr;
    QStackedWidget *mStackedWidget = nullptr;
    QWidget *mEditableChannel = nullptr;
    QWidget *mReadOnlyChannel = nullptr;
    QWidget *mEncryptedLabel = nullptr;
    QLabel *mNameReadOnly = nullptr;
    QLabel *mCommentReadOnly = nullptr;
    QLabel *mAnnouncementReadOnly = nullptr;
    QLabel *mDescriptionReadOnly = nullptr;
    ChannelInfoPruneWidget *const mChannelInfoPruneWidget;
    Room *mRoom = nullptr;
    SystemMessagesComboBox *const mSystemMessageCombox;
};

