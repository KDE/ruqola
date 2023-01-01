/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslateconfigurewidget.h"
#include "model/autotranslatelanguagesmodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

AutoTranslateConfigureWidget::AutoTranslateConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mAutoTranslate(new QCheckBox(i18n("Auto-Translate"), this))
    , mLanguageComboBox(new QComboBox(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAutoTranslate->setObjectName(QStringLiteral("mAutoTranslate"));
    mainLayout->addWidget(mAutoTranslate);
    connect(mAutoTranslate, &QCheckBox::clicked, this, &AutoTranslateConfigureWidget::slotChangeAutoTranslate);

    auto horizontalLayout = new QHBoxLayout;
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins({});
    mainLayout->addLayout(horizontalLayout);

    auto label = new QLabel(i18n("Language:"), this);
    label->setObjectName(QStringLiteral("label"));
    label->setTextFormat(Qt::PlainText);
    horizontalLayout->addWidget(label);

    mLanguageComboBox->setObjectName(QStringLiteral("mLanguage"));
    if (mRocketChatAccount) {
        mLanguageComboBox->setModel(mRocketChatAccount->autoTranslateLanguagesModel());
    }
    connect(mLanguageComboBox, &QComboBox::activated, this, &AutoTranslateConfigureWidget::slotLanguageChanged);

    horizontalLayout->addWidget(mLanguageComboBox);

    mainLayout->addStretch(1);
}

AutoTranslateConfigureWidget::~AutoTranslateConfigureWidget() = default;

Room *AutoTranslateConfigureWidget::room() const
{
    return mRoom;
}

void AutoTranslateConfigureWidget::slotLanguageChanged(int index)
{
    mRocketChatAccount->autoTranslateSaveLanguageSettings(mRoom->roomId(), mRocketChatAccount->autoTranslateLanguagesModel()->selectedLanguage(index));
}

void AutoTranslateConfigureWidget::slotChangeAutoTranslate(bool status)
{
    mRocketChatAccount->autoTranslateSaveAutoTranslateSettings(mRoom->roomId(), status);
}

void AutoTranslateConfigureWidget::slotAutoTranslateChanged()
{
    mAutoTranslate->setChecked(mRoom->autoTranslate());
}

void AutoTranslateConfigureWidget::slotAutoTranslateLanguageChanged()
{
    mLanguageComboBox->setCurrentIndex(mRocketChatAccount->autoTranslateLanguagesModel()->currentLanguage(mRoom->autoTranslateLanguage()));
}

void AutoTranslateConfigureWidget::setRoom(Room *room)
{
    mRoom = room;
    connect(mRoom, &Room::autoTranslateChanged, this, &AutoTranslateConfigureWidget::slotAutoTranslateChanged);
    connect(mRoom, &Room::autoTranslateLanguageChanged, this, &AutoTranslateConfigureWidget::slotAutoTranslateLanguageChanged);
    slotAutoTranslateChanged();
    slotAutoTranslateLanguageChanged();
}
