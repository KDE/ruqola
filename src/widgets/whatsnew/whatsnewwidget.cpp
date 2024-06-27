/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewwidget.h"
#include "whatsnewutils.h"
#include "whatsnewwidgettranslation.h"

#include <QCryptographicHash>
#include <QTextBrowser>
#include <QVBoxLayout>

WhatsNewWidget::WhatsNewWidget(QWidget *parent)
    : QWidget{parent}
    , mLabelInfo(new QTextBrowser(this))
    , mWhatsNewComboBoxWidget(new WhatsNewComboBoxWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mWhatsNewComboBoxWidget->setObjectName(QStringLiteral("mWhatsNewComboBoxWidget"));
    mainLayout->addWidget(mWhatsNewComboBoxWidget);

    mLabelInfo->setObjectName(QStringLiteral("mLabelInfo"));
    mLabelInfo->setReadOnly(true);
    mLabelInfo->setOpenExternalLinks(true);
    mLabelInfo->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    connect(mWhatsNewComboBoxWidget, &WhatsNewComboBoxWidget::versionChanged, this, &WhatsNewWidget::slotVersionChanged);
    mWhatsNewComboBoxWidget->initializeVersion(currentVersion());
    mainLayout->addWidget(mLabelInfo);
}

WhatsNewWidget::~WhatsNewWidget() = default;

WhatsNewUtils::VersionType WhatsNewWidget::currentVersion() const
{
    return WhatsNewUtils::LastVersion;
}

// static
QString WhatsNewWidget::newFeaturesMD5()
{
    QByteArray str;
    for (int i = 0; i < numRuqolaNewFeatures2_3; ++i) {
        str += ruqolaNewFeatures2_3[i].untranslatedText();
    }
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(str);
    return QLatin1StringView(md5.result().toBase64());
}

void WhatsNewWidget::updateInformations()
{
    slotVersionChanged(currentVersion());
}

QString WhatsNewWidget::generateStartEndHtml(const QString &str) const
{
    QString message = QStringLiteral("<qt>");
    message += str;
    message += QStringLiteral("</qt>");
    return message;
}

void WhatsNewWidget::slotVersionChanged(WhatsNewUtils::VersionType type)
{
    if (type == WhatsNewUtils::Version2_0) {
        const QString message = generateStartEndHtml(createVersionInformationsV2_0());
        mLabelInfo->setHtml(message);
    } else if (type == WhatsNewUtils::Version2_1) {
        const QString message = generateStartEndHtml(createVersionInformationsV2_1());
        mLabelInfo->setHtml(message);
    } else if (type == WhatsNewUtils::Version2_2) {
        const QString message = generateStartEndHtml(createVersionInformationsV2_2());
        mLabelInfo->setHtml(message);
    } else if (type == WhatsNewUtils::Version2_3) {
        const QString message = generateStartEndHtml(createVersionInformationsV2_3());
        mLabelInfo->setHtml(message);
    } else if (type == WhatsNewUtils::AllVersion) {
        QString message = generateVersionHeader(WhatsNewUtils::Version2_3);
        message += createVersionInformationsV2_3();
        message += generateVersionHeader(WhatsNewUtils::Version2_2);
        message += createVersionInformationsV2_2();
        message += generateVersionHeader(WhatsNewUtils::Version2_1);
        message += createVersionInformationsV2_1();
        message += generateVersionHeader(WhatsNewUtils::Version2_0);
        message += createVersionInformationsV2_0();
        mLabelInfo->setHtml(generateStartEndHtml(message));
    }
}

QString WhatsNewWidget::importantChangeStr() const
{
    return QStringLiteral("<b>") + i18n("Important changes since last version:") + QStringLiteral("</b>");
}

QString WhatsNewWidget::featuresChangeStr() const
{
    return QStringLiteral("<b>") + i18n("Some of the new features in this release of Ruqola include:") + QStringLiteral("</b>");
}

QString WhatsNewWidget::bugFixingChangeStr() const
{
    return QStringLiteral("<b>") + i18n("Some bug fixing:") + QStringLiteral("</b>");
}

QString WhatsNewWidget::createVersionInformation(const WhatsNewInfo &info)
{
    QString message;
    if (!info.changes().isEmpty()) {
        message += importantChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0, total = info.changes().count(); i < total; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(info.changes().at(i));
        }
        message += QStringLiteral("</ul>");
    }

    if (!info.newFeatures().isEmpty()) {
        message += featuresChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0, total = info.newFeatures().count(); i < total; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(info.newFeatures().at(i));
        }
        message += QStringLiteral("</ul>");
    }

    if (!info.bugFixings().isEmpty()) {
        message += bugFixingChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0, total = info.bugFixings().count(); i < total; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(info.bugFixings().at(i));
        }
        message += QStringLiteral("</ul>");
    }
    return message;
}

QString WhatsNewWidget::createVersionInformationsV2_0() const
{
    QString message;
    if (numRuqolaChanges2_0 > 0) {
        message += importantChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaChanges2_0; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaChangesV2_0[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    if (numRuqolaNewFeatures2_0 > 0) {
        message += featuresChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaNewFeatures2_0; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaNewFeatures2_0[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    if (numRuqolaBugfixing2_0 > 0) {
        message += bugFixingChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaBugfixing2_0; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaBugfixing2_0[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    return message;
}

QString WhatsNewWidget::createVersionInformationsV2_1() const
{
    QString message;
    if (numRuqolaNewFeatures2_1 > 0) {
        message += featuresChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaNewFeatures2_1; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaNewFeatures2_1[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    if (numRuqolaBugfixing2_1 > 0) {
        message += bugFixingChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaBugfixing2_1; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaBugfixing2_1[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    return message;
}

QString WhatsNewWidget::createVersionInformationsV2_2() const
{
    QString message;
    if (numRuqolaNewFeatures2_2 > 0) {
        message += featuresChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaNewFeatures2_2; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaNewFeatures2_2[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    if (numRuqolaBugfixing2_2 > 0) {
        message += bugFixingChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaBugfixing2_2; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaBugfixing2_2[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    return message;
}

QString WhatsNewWidget::createVersionInformationsV2_3() const
{
    QString message;
    if (numRuqolaNewFeatures2_3 > 0) {
        message += featuresChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaNewFeatures2_3; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaNewFeatures2_3[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
#if 0
    if (numRuqolaBugfixing2_3 > 0) {
        message += bugFixingChangeStr();
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaBugfixing2_3; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaBugfixing2_3[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
#endif
    return message;
}

QString WhatsNewWidget::generateVersionHeader(WhatsNewUtils::VersionType type) const
{
    switch (type) {
    case WhatsNewUtils::VersionType::AllVersion:
        return {};
    case WhatsNewUtils::VersionType::Version2_0:
    case WhatsNewUtils::VersionType::Version2_1:
    case WhatsNewUtils::VersionType::Version2_2:
    case WhatsNewUtils::VersionType::Version2_3:
        return QStringLiteral("<h1><i> %1 </i></h1><hr/><br>").arg(WhatsNewUtils::convertVersionEnumToString(type));
    }
    return {};
}

#include "moc_whatsnewwidget.cpp"
