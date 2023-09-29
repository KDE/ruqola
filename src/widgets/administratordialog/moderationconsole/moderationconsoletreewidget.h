/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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

private:
    void slotShowReportedMessages(const QJsonObject &obj);
    void slotDismissReport(const QModelIndex &index);
    void slotDeleteAllMessages(const QModelIndex &index);
    [[nodiscard]] QString displayShowMessage() const;
    AdministratorModerationRangeWidget::DateTimeRange mModerationRanges;
    ModerationMessagesModel *const mCommonMessagesModel;
    CommonMessageFilterProxyModel *mCommonMessageFilterProxyModel = nullptr;
};
