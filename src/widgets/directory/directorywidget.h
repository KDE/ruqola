/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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
    enum class DirectoryType {
        Unknown = 0,
        Room = 1,
        User = 2,
        Team = 3,
    };
    explicit DirectoryWidget(RocketChatAccount *account, DirectoryType type, QWidget *parent = nullptr);
    ~DirectoryWidget() override;

    void fillDirectory();

    [[nodiscard]] DirectoryType type() const;

    void slotJoin();
Q_SIGNALS:
    void updateJoinButton(bool status);

protected:
    void showEvent(QShowEvent *event) override;
    void updateLabel() override;
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpen(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchRequested(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchCleared();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessageInRoom() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString noFoundInfo() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void finishSearching();
    const DirectoryType mType;
    bool mWasInitialized = false;
};
