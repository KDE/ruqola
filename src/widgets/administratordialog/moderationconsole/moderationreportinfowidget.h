/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class ModerationReportInfoListView;
class QLineEdit;
class ModerationReportInfoModel;
class ModerationReportInfoFilterProxyModel;
class ModerationReportInfos;
namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationReportInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModerationReportInfoWidget(QWidget *parent = nullptr);
    ~ModerationReportInfoWidget() override;

    void setReportInfos(const ModerationReportInfos &infos);

private:
    void slotTextToSpeech(const QString &messageText);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotTextChanged(const QString &str);
    ModerationReportInfoListView *const mListNotificationsListView;
    QLineEdit *const mSearchLineEdit;
    ModerationReportInfoFilterProxyModel *const mModerationReportInfoFilterProxyModel;
    ModerationReportInfoModel *const mModerationReportInfoModel;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
