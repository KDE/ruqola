/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomemojicreateorupdatewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScreen>

#include <KColorScheme>
#include <KLocalizedString>
#include <KStatefulBrush>
#include <KUrlRequester>

AdministratorCustomEmojiCreateOrUpdateWidget::AdministratorCustomEmojiCreateOrUpdateWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mAlias(new QLineEdit(this))
    , mSelectFile(new KUrlRequester(this))
    , mWarningLabel(new QLabel(i18n("The custom emoji name and their aliases should be different."), this))
    , mIconLabel(new QLabel(this))
{
    mWarningLabel->setObjectName(QStringLiteral("mWarningLabel"));
    const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeText);
    const QColor color = bgBrush.brush(palette()).color();

    QPalette pal = mWarningLabel->palette();
    pal.setColor(QPalette::WindowText, color);
    mWarningLabel->setPalette(pal);
    mWarningLabel->hide();

    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mIconLabel->setObjectName(QStringLiteral("mIconLabel"));

    mName->setObjectName(QStringLiteral("mName"));
    mName->setClearButtonEnabled(true);
    mAlias->setObjectName(QStringLiteral("mAlias"));
    mAlias->setClearButtonEnabled(true);
    mSelectFile->setObjectName(QStringLiteral("mSelectFile"));
    new LineEditCatchReturnKey(mName, this);
    new LineEditCatchReturnKey(mAlias, this);
    mainLayout->addRow(i18n("Name:"), mName);
    mainLayout->addRow(i18n("Alias:"), mAlias);
    mainLayout->addRow(i18n("File:"), mSelectFile);
    mainLayout->addWidget(mIconLabel);
    mainLayout->addWidget(mWarningLabel);
    connect(mName, &QLineEdit::textChanged, this, &AdministratorCustomEmojiCreateOrUpdateWidget::slotUpdateOkButton);
    connect(mAlias, &QLineEdit::textChanged, this, &AdministratorCustomEmojiCreateOrUpdateWidget::slotUpdateOkButton);
    connect(mSelectFile, &KUrlRequester::urlSelected, this, &AdministratorCustomEmojiCreateOrUpdateWidget::slotAddNewEmoji);
    connect(mSelectFile, &KUrlRequester::textChanged, this, &AdministratorCustomEmojiCreateOrUpdateWidget::slotUpdateOkButton);
}

AdministratorCustomEmojiCreateOrUpdateWidget::~AdministratorCustomEmojiCreateOrUpdateWidget() = default;

void AdministratorCustomEmojiCreateOrUpdateWidget::setCustomEmojiInfo(const CustomEmojiCreateInfo &info)
{
    mName->setText(info.name);
    mAlias->setText(info.alias);
    const QSize pixmapAvatarSize = QSize(60, 60) * screen()->devicePixelRatio();
    mIconLabel->setPixmap(info.icon.pixmap(pixmapAvatarSize));
    // TODO url ???
    slotUpdateOkButton();
}

AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo AdministratorCustomEmojiCreateOrUpdateWidget::info() const
{
    AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo info;
    info.name = mName->text().trimmed();
    info.alias = mAlias->text().trimmed();
    info.fileNameUrl = mSelectFile->url();
    return info;
}

void AdministratorCustomEmojiCreateOrUpdateWidget::slotAddNewEmoji()
{
    const QSize pixmapAvatarSize = QSize(60, 60) * screen()->devicePixelRatio();
    mIconLabel->setPixmap(QIcon(mSelectFile->url().toLocalFile()).pixmap(pixmapAvatarSize));

    slotUpdateOkButton();
}

void AdministratorCustomEmojiCreateOrUpdateWidget::slotUpdateOkButton()
{
    const QString nameTrimmed{mName->text().trimmed()};
    const QString aliasTrimmed{mAlias->text().trimmed()};
    if (nameTrimmed.isEmpty() && aliasTrimmed.isEmpty()) {
        Q_EMIT updateOkButton(false);
        return;
    }
    if (nameTrimmed != aliasTrimmed) {
        mWarningLabel->hide();
        switch (mType) {
        case Create:
            Q_EMIT updateOkButton(!nameTrimmed.isEmpty() && mSelectFile->url().isValid());
            break;
        case Update:
            Q_EMIT updateOkButton(!nameTrimmed.isEmpty());
            break;
        }
    } else {
        mWarningLabel->show();
        Q_EMIT updateOkButton(false);
    }
}

AdministratorCustomEmojiCreateOrUpdateWidget::AdministratorCustomEmojiCreateOrUpdateType AdministratorCustomEmojiCreateOrUpdateWidget::type() const
{
    return mType;
}

void AdministratorCustomEmojiCreateOrUpdateWidget::setType(AdministratorCustomEmojiCreateOrUpdateType newType)
{
    mType = newType;
}

QDebug operator<<(QDebug d, const AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo &t)
{
    d << "alias " << t.alias;
    d << "name " << t.name;
    d << "fileNameUrl " << t.fileNameUrl;
    return d;
}

#include "moc_administratorcustomemojicreateorupdatewidget.cpp"
