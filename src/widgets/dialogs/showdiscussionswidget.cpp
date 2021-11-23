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

#include "showdiscussionswidget.h"
#include "discussion/listdiscussiondelegate.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/discussionsfilterproxymodel.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

ShowDiscussionsWidget::ShowDiscussionsWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchDiscussionLineEdit(new QLineEdit(this))
    , mDiscussionInfoLabel(new QLabel(this))
    , mListDiscussions(new QListView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchDiscussionLineEdit->setObjectName(QStringLiteral("mSearchDiscussionLineEdit"));
    mSearchDiscussionLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchDiscussionLineEdit, this);
    mSearchDiscussionLineEdit->setPlaceholderText(i18n("Search Discussion..."));
    connect(mSearchDiscussionLineEdit, &QLineEdit::textChanged, this, &ShowDiscussionsWidget::slotSearchMessageTextChanged);
    mainLayout->addWidget(mSearchDiscussionLineEdit);

    mDiscussionInfoLabel->setObjectName(QStringLiteral("mInfo"));
    mDiscussionInfoLabel->setTextFormat(Qt::RichText);
    mainLayout->addWidget(mDiscussionInfoLabel);
    QFont labFont = mDiscussionInfoLabel->font();
    labFont.setBold(true);
    mDiscussionInfoLabel->setFont(labFont);
    connect(mDiscussionInfoLabel, &QLabel::linkActivated, this, &ShowDiscussionsWidget::loadMoreDiscussion);

    mListDiscussions->setObjectName(QStringLiteral("mListDiscussions"));
    mainLayout->addWidget(mListDiscussions);
    mListDiscussions->setItemDelegate(new ListDiscussionDelegate(this));
}

ShowDiscussionsWidget::~ShowDiscussionsWidget() = default;

void ShowDiscussionsWidget::slotSearchMessageTextChanged(const QString &str)
{
    mDiscussionModel->setFilterString(str);
    updateLabel();
}

void ShowDiscussionsWidget::setModel(DiscussionsFilterProxyModel *model)
{
    mListDiscussions->setModel(model);
    mDiscussionModel = model;
    connect(mDiscussionModel, &DiscussionsFilterProxyModel::hasFullListChanged, this, &ShowDiscussionsWidget::updateLabel);
    connect(mDiscussionModel, &DiscussionsFilterProxyModel::loadingInProgressChanged, this, &ShowDiscussionsWidget::updateLabel);
    updateLabel();
}

void ShowDiscussionsWidget::updateLabel()
{
    if (mDiscussionModel->loadMoreDiscussionsInProgress()) {
        mDiscussionInfoLabel->setText(i18n("Loading..."));
    } else {
        mDiscussionInfoLabel->setText(mDiscussionModel->rowCount() == 0 ? i18n("No Discussion found") : displayShowDiscussionInRoom());
    }
}

QString ShowDiscussionsWidget::displayShowDiscussionInRoom() const
{
    QString displayMessageStr =
        i18np("%1 Discussion in room (Total: %2)", "%1 Discussions in room (Total: %2)", mDiscussionModel->rowCount(), mDiscussionModel->total());
    if (!mDiscussionModel->hasFullList()) {
        displayMessageStr += QStringLiteral(" <a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
    }
    return displayMessageStr;
}
