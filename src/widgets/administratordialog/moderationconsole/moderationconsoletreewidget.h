/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratormoderationrangewidget.h"
#include "libruqolawidgets_private_export.h"
#include "misc/searchtreebasewidget.h"
class QWidget;
class ModerationMessagesModel;
class CommonMessageFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationConsoleTreeWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit ModerationConsoleTreeWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ModerationConsoleTreeWidget() override;

    void setModerationRanges(const AdministratorModerationRangeWidget::DateTimeRange &range);

protected:
    void updateLabel() override;
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;

Q_SIGNALS:
    void refreshList();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowReportedMessages(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDismissReport(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDeleteAllMessages(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowMessages(const QModelIndex &newModelIndex);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessage() const;
    AdministratorModerationRangeWidget::DateTimeRange mModerationRanges;
    ModerationMessagesModel *const mCommonMessagesModel;
    CommonMessageFilterProxyModel *mCommonMessageFilterProxyModel = nullptr;
};
