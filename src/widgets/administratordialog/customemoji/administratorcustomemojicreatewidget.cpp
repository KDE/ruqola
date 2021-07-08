/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "administratorcustomemojicreatewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

#include <KLocalizedString>
#include <KUrlRequester>

AdministratorCustomEmojiCreateWidget::AdministratorCustomEmojiCreateWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mAlias(new QLineEdit(this))
    , mSelectFile(new KUrlRequester(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mName->setObjectName(QStringLiteral("mName"));
    mAlias->setObjectName(QStringLiteral("mAlias"));
    mSelectFile->setObjectName(QStringLiteral("mSelectFile"));
    new LineEditCatchReturnKey(mName, this);
    new LineEditCatchReturnKey(mAlias, this);
    mainLayout->addRow(i18n("Name:"), mName);
    mainLayout->addRow(i18n("Alias:"), mAlias);
    mainLayout->addRow(i18n("File:"), mSelectFile);
    connect(mName, &QLineEdit::textChanged, this, &AdministratorCustomEmojiCreateWidget::slotUpdateOkButton);
    connect(mAlias, &QLineEdit::textChanged, this, &AdministratorCustomEmojiCreateWidget::slotUpdateOkButton);
}

AdministratorCustomEmojiCreateWidget::~AdministratorCustomEmojiCreateWidget()
{
}

void AdministratorCustomEmojiCreateWidget::setCustomEmojiInfo(const CustomEmojiCreateInfo &info)
{
    mName->setText(info.name);
    mAlias->setText(info.alias);

    // TODO url ???
}

AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo AdministratorCustomEmojiCreateWidget::info() const
{
    AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo info;
    info.name = mName->text().trimmed();
    info.alias = mAlias->text().trimmed();
    info.fileNameUrl = mSelectFile->url();
    return info;
}

void AdministratorCustomEmojiCreateWidget::slotUpdateOkButton()
{
    Q_EMIT updateOkButton(!mName->text().trimmed().isEmpty() && !mAlias->text().trimmed().isEmpty());
}
