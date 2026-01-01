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
class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationReportedMessageConsoleTreeWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit ModerationReportedMessageConsoleTreeWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ModerationReportedMessageConsoleTreeWidget() override;

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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDesactivateUser(const QModelIndex &index);
    AdministratorModerationRangeWidget::DateTimeRange mModerationRanges;
    ModerationMessagesModel *const mCommonMessagesModel;
    CommonMessageFilterProxyModel *mCommonMessageFilterProxyModel = nullptr;
};
