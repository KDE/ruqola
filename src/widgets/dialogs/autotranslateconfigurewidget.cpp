/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslateconfigurewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "autotranslate/translatesavesettingsjob.h"
#include "model/autotranslatelanguagesmodel.h"
#include "rocketchataccount.h"

#include "connection.h"
#include "room.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

AutoTranslateConfigureWidget::AutoTranslateConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mAutoTranslate(new QCheckBox(i18nc("@option:check", "Auto-Translate"), this))
    , mLanguageComboBox(new QComboBox(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mAutoTranslate->setObjectName(u"mAutoTranslate"_s);
    mainLayout->addWidget(mAutoTranslate);
    connect(mAutoTranslate, &QCheckBox::clicked, this, &AutoTranslateConfigureWidget::slotChangeAutoTranslate);

    auto horizontalLayout = new QHBoxLayout;
    horizontalLayout->setObjectName(u"horizontalLayout"_s);
    horizontalLayout->setContentsMargins({});
    mainLayout->addLayout(horizontalLayout);

    auto label = new QLabel(i18nc("@label:textbox", "Language:"), this);
    label->setObjectName(u"label"_s);
    label->setTextFormat(Qt::PlainText);
    horizontalLayout->addWidget(label);

    mLanguageComboBox->setObjectName(u"mLanguage"_s);
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
    auto job = new RocketChatRestApi::TranslateSaveSettingsJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    job->setRoomId(QString::fromLatin1(mRoom->roomId()));
    job->setType(RocketChatRestApi::TranslateSaveSettingsJob::SettingType::LanguageSetting);
    job->setLanguage(mRocketChatAccount->autoTranslateLanguagesModel()->selectedLanguage(index));
    if (!job->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start autoTranslateSaveLanguageSettings";
    }
}

void AutoTranslateConfigureWidget::slotChangeAutoTranslate(bool status)
{
    auto job = new RocketChatRestApi::TranslateSaveSettingsJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    job->setRoomId(QString::fromLatin1(mRoom->roomId()));
    job->setType(RocketChatRestApi::TranslateSaveSettingsJob::SettingType::AutoTranslateSetting);
    job->setAutoTranslate(status);
    if (!job->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start autoTranslateSaveAutoTranslateSettings";
    }
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

#include "moc_autotranslateconfigurewidget.cpp"
