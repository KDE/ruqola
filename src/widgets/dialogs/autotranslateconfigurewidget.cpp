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

#include "autotranslateconfigurewidget.h"
#include "model/autotranslatelanguagesmodel.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "roomwrapper.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

AutoTranslateConfigureWidget::AutoTranslateConfigureWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mAutoTranslate = new QCheckBox(i18n("Auto-Translate"), this);
    mAutoTranslate->setObjectName(QStringLiteral("mAutoTranslate"));
    mainLayout->addWidget(mAutoTranslate);
    connect(mAutoTranslate, &QCheckBox::clicked, this, &AutoTranslateConfigureWidget::slotChangeAutoTranslate);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(horizontalLayout);

    QLabel *label = new QLabel(i18n("Language:"), this);
    label->setObjectName(QStringLiteral("label"));
    horizontalLayout->addWidget(label);

    mLanguage = new QComboBox(this);
    mLanguage->setObjectName(QStringLiteral("mLanguage"));
    mLanguage->setModel(Ruqola::self()->rocketChatAccount()->autoTranslateLanguagesModel());
    connect(mLanguage, &QComboBox::activated, this, &AutoTranslateConfigureWidget::slotLanguageChanged);

    horizontalLayout->addWidget(mLanguage);

    mainLayout->addStretch(1);
}

AutoTranslateConfigureWidget::~AutoTranslateConfigureWidget()
{
}

RoomWrapper *AutoTranslateConfigureWidget::roomWrapper() const
{
    return mRoomWrapper;
}

void AutoTranslateConfigureWidget::slotLanguageChanged(int index)
{
    Ruqola::self()->rocketChatAccount()->autoTranslateSaveLanguageSettings(mRoomWrapper->roomId(), Ruqola::self()->rocketChatAccount()->autoTranslateLanguagesModel()->selectedLanguage(index));
}

void AutoTranslateConfigureWidget::slotChangeAutoTranslate(bool status)
{
    Ruqola::self()->rocketChatAccount()->autoTranslateSaveAutoTranslateSettings(mRoomWrapper->roomId(), status);
}

void AutoTranslateConfigureWidget::slotAutoTranslateChanged()
{
    mAutoTranslate->setChecked(mRoomWrapper->autoTranslate());
}

void AutoTranslateConfigureWidget::slotAutoTranslateLanguageChanged()
{
    mLanguage->setCurrentIndex(Ruqola::self()->rocketChatAccount()->autoTranslateLanguagesModel()->currentLanguage(mRoomWrapper->autoTranslateLanguage()));
}

void AutoTranslateConfigureWidget::setRoomWrapper(RoomWrapper *roomWrapper)
{
    mRoomWrapper = roomWrapper;
    connect(mRoomWrapper, &RoomWrapper::autoTranslateChanged, this, &AutoTranslateConfigureWidget::slotAutoTranslateChanged);
    connect(mRoomWrapper, &RoomWrapper::autoTranslateLanguageChanged, this, &AutoTranslateConfigureWidget::slotAutoTranslateLanguageChanged);
    slotAutoTranslateChanged();
    slotAutoTranslateLanguageChanged();
}
