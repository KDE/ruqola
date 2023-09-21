/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class QLabel;
class DiscussionListView;
class DiscussionsFilterProxyModel;
class RocketChatAccount;

namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowDiscussionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowDiscussionsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowDiscussionsWidget() override;

    void setModel(DiscussionsFilterProxyModel *model);

Q_SIGNALS:
    void loadMoreDiscussion();

private:
    void slotSearchMessageTextChanged(const QString &str);
    void updateLabel();
    [[nodiscard]] QString displayShowDiscussionInRoom() const;
    QLineEdit *const mSearchDiscussionLineEdit;
    QLabel *const mDiscussionInfoLabel;
    DiscussionListView *const mListDiscussionsListView;
    DiscussionsFilterProxyModel *mDiscussionModel = nullptr;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
