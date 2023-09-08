/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewwidget.h"
#include "whatsnewcomboboxwidget.h"

#include <KLazyLocalizedString>

#include <QCryptographicHash>
#include <QScrollArea>
#include <QTextEdit>
#include <QVBoxLayout>

// enter items for the "Important changes" list here:
static const KLazyLocalizedString ruqolaChangesV2_0[] = {
    kli18n("Store Message in Local Database (experimental)"),
};
static const int numRuqolaChanges2_0 = sizeof ruqolaChangesV2_0 / sizeof *ruqolaChangesV2_0;

// enter items for the "new features" list here, so the main body of
// the welcome page can be left untouched (probably much easier for
// the translators). Note that the <li>...</li> tags are added
// automatically below:
static const KLazyLocalizedString ruqolaNewFeatures2_0[] = {
    kli18n("Download Server Icon from Server"),
    kli18n("Show Server Error Info in Specific DialogBox"),
    kli18n("Allow to Copy Image in Clipboard"),
    kli18n("Improve debug support (Allow to show current account permissions)"),
    kli18n("Add support for quoted text (text which starts by \'>\')"),
    kli18n("Show leader/moderator/owner in channel info."),
    kli18n("Video/Sound Message support (kf6 only)."),
    kli18n("Import/Export Accounts."),
};
static const int numRuqolaNewFeatures2_0 = sizeof ruqolaNewFeatures2_0 / sizeof *ruqolaNewFeatures2_0;

// enter items for the "Important changes" list here:
static const KLazyLocalizedString ruqolaBugfixing2_0[] = {
    kli18n("Fix avatar support"),
    kli18n("Fix emoji support (use new ktextaddons/textemoticons)"),
    kli18n("Fix animated gif in reactions"),
    kli18n("Fix kf6 support"),
};
static const int numRuqolaBugfixing2_0 = sizeof ruqolaBugfixing2_0 / sizeof *ruqolaBugfixing2_0;

WhatsNewWidget::WhatsNewWidget(QWidget *parent)
    : QWidget{parent}
    , mLabelInfo(new QTextEdit(this))
    , mWhatsNewComboBoxWidget(new WhatsNewComboBoxWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mWhatsNewComboBoxWidget->setObjectName(QStringLiteral("mWhatsNewComboBoxWidget"));
    mainLayout->addWidget(mWhatsNewComboBoxWidget);

    mLabelInfo->setObjectName(QStringLiteral("mLabelInfo"));
    mLabelInfo->setReadOnly(true);
    mLabelInfo->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    connect(mWhatsNewComboBoxWidget, &WhatsNewComboBoxWidget::versionChanged, this, &WhatsNewWidget::slotVersionChanged);
    mWhatsNewComboBoxWidget->initializeVersion(currentVersion());
    mainLayout->addWidget(mLabelInfo);
    // mWhatsNewComboBoxWidget->setVisible(false);
}

WhatsNewWidget::~WhatsNewWidget() = default;

WhatsNewComboBoxWidget::VersionType WhatsNewWidget::currentVersion() const
{
    return WhatsNewComboBoxWidget::Version2_0;
}

// static
QString WhatsNewWidget::newFeaturesMD5()
{
    // TODO use last version
    // => update each time that we change version
    QByteArray str;
    for (int i = 0; i < numRuqolaChanges2_0; ++i) {
        str += ruqolaChangesV2_0[i].untranslatedText();
    }
    for (int i = 0; i < numRuqolaNewFeatures2_0; ++i) {
        str += ruqolaNewFeatures2_0[i].untranslatedText();
    }
    for (int i = 0; i < numRuqolaBugfixing2_0; ++i) {
        str += ruqolaBugfixing2_0[i].untranslatedText();
    }
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(str);
    return QLatin1String(md5.result().toBase64());
}

void WhatsNewWidget::updateInformations()
{
    mLabelInfo->setHtml(createVersionInformations());
}

void WhatsNewWidget::slotVersionChanged(WhatsNewComboBoxWidget::VersionType type)
{
    if (type == WhatsNewComboBoxWidget::Version2_0 || type == WhatsNewComboBoxWidget::AllVersion) {
        mLabelInfo->setHtml(createVersionInformations());
    }
}

QString WhatsNewWidget::createVersionInformations() const
{
    QString message = QStringLiteral("<qt>");
    if (numRuqolaChanges2_0 > 0) {
        message += QStringLiteral("<b>") + i18n("Important changes since last version:") + QStringLiteral("</b>");
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaChanges2_0; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaChangesV2_0[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    if (numRuqolaNewFeatures2_0 > 0) {
        message += QStringLiteral("<b>") + i18n("Some of the new features in this release of Ruqola include:") + QStringLiteral("</b>");
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaNewFeatures2_0; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaNewFeatures2_0[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    if (numRuqolaBugfixing2_0 > 0) {
        message += QStringLiteral("<b>") + i18n("Some bug fixing:") + QStringLiteral("</b>");
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaBugfixing2_0; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaBugfixing2_0[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    message += QStringLiteral("</qt>");
    return message;
}

QString WhatsNewWidget::generateVersionHeader(WhatsNewComboBoxWidget::VersionType type) const
{
    switch (type) {
    case WhatsNewComboBoxWidget::VersionType::AllVersion:
        return {};
    case WhatsNewComboBoxWidget::VersionType::Version2_0:
        return QStringLiteral("--- %1 ---\n").arg(WhatsNewComboBoxWidget::convertVersionEnumToString(type));
    }
    return {};
}

#include "moc_whatsnewwidget.cpp"
