/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratormoderationrangewidget.h"
#include "libruqolawidgets_private_export.h"
#include "misc/searchtreebasewidget.h"
class QWidget;
class ModerationMessagesModel;
class CommonMessageFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationReportedUserConsoleTreeWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit ModerationReportedUserConsoleTreeWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ModerationReportedUserConsoleTreeWidget() override;

    void setModerationRanges(const AdministratorModerationRangeWidget::DateTimeRange &range);

protected:
    void updateLabel() override;
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;

Q_SIGNALS:
    void refreshList();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowReportedUsers(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDismissReport(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowReports(const QModelIndex &newModelIndex);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDesactivateUser(const QModelIndex &index);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessage() const;
    AdministratorModerationRangeWidget::DateTimeRange mModerationRanges;
    ModerationMessagesModel *const mCommonMessagesModel;
    CommonMessageFilterProxyModel *mCommonMessageFilterProxyModel = nullptr;
};
