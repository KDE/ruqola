/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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
#include "listdiscussiondelegate.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <KLineEdit>
#include <QLabel>
#include <QListView>

ShowDiscussionsWidget::ShowDiscussionsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mSearchDiscussionLineEdit = new KLineEdit(this);
    mSearchDiscussionLineEdit->setObjectName(QStringLiteral("mSearchDiscussionLineEdit"));
    mSearchDiscussionLineEdit->setClearButtonEnabled(true);
    mSearchDiscussionLineEdit->setTrapReturnKey(true);
    mSearchDiscussionLineEdit->setPlaceholderText(i18n("Search Discussion..."));
    connect(mSearchDiscussionLineEdit, &KLineEdit::textChanged, this, &ShowDiscussionsWidget::slotSearchMessageTextChanged);
    mainLayout->addWidget(mSearchDiscussionLineEdit);

    mInfo = new QLabel(this);
    mInfo->setObjectName(QStringLiteral("mInfo"));
    mInfo->setTextFormat(Qt::RichText);
    mainLayout->addWidget(mInfo);
    QFont labFont = mInfo->font();
    labFont.setBold(true);
    mInfo->setFont(labFont);
    connect(mInfo, &QLabel::linkActivated, this, &ShowDiscussionsWidget::loadMoreDiscussion);

    mListDiscussions = new QListView(this);
    mListDiscussions->setObjectName(QStringLiteral("mListDiscussions"));
    mainLayout->addWidget(mListDiscussions);
    mListDiscussions->setItemDelegate(new ListDiscussionDelegate(this));

    //TODO need to update label !!!

}

ShowDiscussionsWidget::~ShowDiscussionsWidget()
{

}

void ShowDiscussionsWidget::slotSearchMessageTextChanged(const QString &str)
{
    //mModel->setFilterString(str);
}
