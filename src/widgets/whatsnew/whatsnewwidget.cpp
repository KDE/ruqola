/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "whatsnew/whatsnewtranslations.h"

#include <QTextBrowser>
#include <QVBoxLayout>

namespace
{
constexpr int allVersion = -1;
}

WhatsNewWidget::WhatsNewWidget(QWidget *parent)
    : QWidget{parent}
    , mLabelInfo(new QTextBrowser(this))
    , mWhatsNewComboBoxWidget(new WhatsNewComboBoxWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mWhatsNewComboBoxWidget->setObjectName(u"mWhatsNewComboBoxWidget"_s);
    mainLayout->addWidget(mWhatsNewComboBoxWidget);

    mLabelInfo->setObjectName(u"mLabelInfo"_s);
    mLabelInfo->setReadOnly(true);
    mLabelInfo->setOpenExternalLinks(true);
    mLabelInfo->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    connect(mWhatsNewComboBoxWidget, &WhatsNewComboBoxWidget::versionChanged, this, &WhatsNewWidget::slotVersionChanged);
    mainLayout->addWidget(mLabelInfo);
    fillTranslations();
    fillComboBox();
    mWhatsNewComboBoxWidget->initializeVersion(currentVersion());
}

WhatsNewWidget::~WhatsNewWidget() = default;

void WhatsNewWidget::fillComboBox()
{
    mWhatsNewComboBoxWidget->addVersion(i18n("All Version"), allVersion);
    for (int i = mWhatsNewInfo.count() - 1; i >= 0; i--) {
        const WhatsNewInfo &info = mWhatsNewInfo.at(i);
        mWhatsNewComboBoxWidget->addVersion(i18n("Version %1", info.version()), i);
    }
}

void WhatsNewWidget::fillTranslations()
{
    const WhatsNewTranslations translations;
    mWhatsNewInfo = translations.createWhatsNewInfo();
}

int WhatsNewWidget::currentVersion() const
{
    return mWhatsNewInfo.count() - 1;
}

void WhatsNewWidget::updateInformations()
{
    slotVersionChanged(currentVersion());
}

QString WhatsNewWidget::generateStartEndHtml(const QString &str) const
{
    const QString message = u"<qt>"_s + str + u"</qt>"_s;
    return message;
}

void WhatsNewWidget::slotVersionChanged(int type)
{
    if (type == allVersion) { // All
        QString message;
        for (int i = mWhatsNewInfo.count() - 1; i >= 0; i--) {
            const WhatsNewInfo &info = mWhatsNewInfo.at(i);
            message += generateVersionHeader(i);
            message += createVersionInformation(info);
        }
        mLabelInfo->setHtml(generateStartEndHtml(message));
    } else {
        const QString message = generateStartEndHtml(createVersionInformation(mWhatsNewInfo.at(type)));
        mLabelInfo->setHtml(message);
    }
}

QString WhatsNewWidget::importantChangeStr() const
{
    return u"<b>"_s + i18n("Important changes since last version:") + u"</b>"_s;
}

QString WhatsNewWidget::featuresChangeStr() const
{
    return u"<b>"_s + i18n("Some of the new features in this release of Ruqola include:") + u"</b>"_s;
}

QString WhatsNewWidget::bugFixingChangeStr() const
{
    return u"<b>"_s + i18n("Some bug fixing:") + u"</b>"_s;
}

QString WhatsNewWidget::createVersionInformation(const WhatsNewInfo &info)
{
    QString message;
    if (!info.changes().isEmpty()) {
        message += importantChangeStr();
        message += u"<ul>"_s;
        for (int i = 0, total = info.changes().count(); i < total; ++i) {
            message += u"<li>%1</li>"_s.arg(info.changes().at(i));
        }
        message += u"</ul>"_s;
    }

    if (!info.newFeatures().isEmpty()) {
        message += featuresChangeStr();
        message += u"<ul>"_s;
        for (int i = 0, total = info.newFeatures().count(); i < total; ++i) {
            message += u"<li>%1</li>"_s.arg(info.newFeatures().at(i));
        }
        message += u"</ul>"_s;
    }

    if (!info.bugFixings().isEmpty()) {
        message += bugFixingChangeStr();
        message += u"<ul>"_s;
        for (int i = 0, total = info.bugFixings().count(); i < total; ++i) {
            message += u"<li>%1</li>"_s.arg(info.bugFixings().at(i));
        }
        message += u"</ul>"_s;
    }
    return message;
}

QString WhatsNewWidget::generateVersionHeader(int type) const
{
    if (type != allVersion) {
        return u"<h1><i> Version %1 </i></h1><hr/><br>"_s.arg(mWhatsNewInfo.at(type).version());
    }
    return {};
}

#include "moc_whatsnewwidget.cpp"
