/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

class QWidget;

#include "misc/searchtreebasewidget.h"

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    enum DirectoryType {
        Unknown = 0,
        Room = 1,
        User = 2,
        Team = 3,
    };
    explicit DirectoryWidget(RocketChatAccount *account, DirectoryType type, QWidget *parent = nullptr);
    ~DirectoryWidget() override;

    void fillDirectory();

    Q_REQUIRED_RESULT DirectoryType type() const;

protected:
    void showEvent(QShowEvent *event) override;
    void updateLabel() override;
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;

private:
    void slotOpen(const QModelIndex &index);
    void slotSearchRequested(const QString &str);
    void slotSearchCleared();
    Q_REQUIRED_RESULT QString displayShowMessageInRoom() const;
    Q_REQUIRED_RESULT QString noFoundInfo() const;
    void finishSearching();
    const DirectoryType mType;
    bool mWasInitialized = false;
};
