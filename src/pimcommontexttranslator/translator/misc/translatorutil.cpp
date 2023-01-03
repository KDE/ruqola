/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorutil.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QComboBox>
using namespace PimCommonTextTranslator;

TranslatorUtil::TranslatorUtil() = default;

QMap<TranslatorUtil::Language, QString> TranslatorUtil::translatedLanguages()
{
    QMap<TranslatorUtil::Language, QString> map;
    for (int i = TranslatorUtil::Language::automatic; i < TranslatorUtil::Language::lastLanguage; ++i) {
        map.insert(static_cast<TranslatorUtil::Language>(i), translatedLanguage(static_cast<TranslatorUtil::Language>(i)));
    }
    return map;
}

QString TranslatorUtil::translatedLanguage(TranslatorUtil::Language lang)
{
    QString ret;
    switch (lang) {
    case automatic:
        ret = i18n("Detect language");
        break;
    case hmong:
        ret = i18n("Hmong");
        break;
    case bs:
        ret = i18n("Bosnian");
        break;
    case en:
        ret = i18n("English");
        break;
    case zh:
        ret = i18n("Chinese (Simplified)");
        break;
    case zt:
        ret = i18n("Chinese (Traditional)");
        break;
    case nl:
        ret = i18n("Dutch");
        break;
    case fr:
        ret = i18n("French");
        break;
    case de:
        ret = i18n("German");
        break;
    case el:
        ret = i18n("Greek");
        break;
    case it:
        ret = i18n("Italian");
        break;
    case ja:
        ret = i18n("Japanese");
        break;
    case ko:
        ret = i18n("Korean");
        break;
    case pt:
        ret = i18n("Portuguese");
        break;
    case ru:
        ret = i18n("Russian");
        break;
    case es:
        ret = i18n("Spanish");
        break;
    case af:
        ret = i18n("Afrikaans");
        break;
    case sq:
        ret = i18n("Albanian");
        break;
    case ar:
        ret = i18n("Arabic");
        break;
    case hy:
        ret = i18n("Armenian");
        break;
    case az:
        ret = i18n("Azerbaijani");
        break;
    case eu:
        ret = i18n("Basque");
        break;
    case be:
        ret = i18n("Belarusian");
        break;
    case bg:
        ret = i18n("Bulgarian");
        break;
    case ca:
        ret = i18n("Catalan");
        break;
    case hr:
        ret = i18n("Croatian");
        break;
    case cs:
        ret = i18n("Czech");
        break;
    case da:
        ret = i18n("Danish");
        break;
    case et:
        ret = i18n("Estonian");
        break;
    case tl:
        ret = i18n("Filipino");
        break;
    case fi:
        ret = i18n("Finnish");
        break;
    case gl:
        ret = i18n("Galician");
        break;
    case ka:
        ret = i18n("Georgian");
        break;
    case ht:
        ret = i18n("Haitian Creole");
        break;
    case iw:
        ret = i18n("Hebrew");
        break;
    case hi:
        ret = i18n("Hindi");
        break;
    case hu:
        ret = i18n("Hungarian");
        break;
    case is:
        ret = i18n("Icelandic");
        break;
    case id:
        ret = i18n("Indonesian");
        break;
    case ga:
        ret = i18n("Irish");
        break;
    case lv:
        ret = i18n("Latvian");
        break;
    case lt:
        ret = i18n("Lithuanian");
        break;
    case mk:
        ret = i18n("Macedonian");
        break;
    case ms:
        ret = i18n("Malay");
        break;
    case mt:
        ret = i18n("Maltese");
        break;
    case no:
        ret = i18n("Norwegian");
        break;
    case fa:
        ret = i18n("Persian");
        break;
    case pl:
        ret = i18n("Polish");
        break;
    case ro:
        ret = i18n("Romanian");
        break;
    case sr:
        ret = i18n("Serbian");
        break;
    case sk:
        ret = i18n("Slovak");
        break;
    case sl:
        ret = i18n("Slovenian");
        break;
    case sw:
        ret = i18n("Swahili");
        break;
    case sv:
        ret = i18n("Swedish");
        break;
    case th:
        ret = i18n("Thai");
        break;
    case tr:
        ret = i18n("Turkish");
        break;
    case uk:
        ret = i18n("Ukrainian");
        break;
    case ur:
        ret = i18n("Urdu");
        break;
    case vi:
        ret = i18n("Vietnamese");
        break;
    case cy:
        ret = i18n("Welsh");
        break;
    case yi:
        ret = i18n("Yiddish");
        break;
    }
    return ret;
}

QString TranslatorUtil::searchI18nFromLanguage(const QString &langCode)
{
    for (int i = TranslatorUtil::Language::automatic; i < TranslatorUtil::Language::lastLanguage; ++i) {
        if (langCode == languageCode(static_cast<TranslatorUtil::Language>(i))) {
            return translatedLanguage(static_cast<TranslatorUtil::Language>(i));
        }
    }
    return {};
}

QString TranslatorUtil::languageCode(TranslatorUtil::Language lang)
{
    QString ret;
    switch (lang) {
    case automatic:
        ret = QStringLiteral("auto");
        break;
    case hmong:
        ret = QStringLiteral("hmn");
        break;
    case en:
        ret = QStringLiteral("en");
        break;
    case zh:
        ret = QStringLiteral("zh-CN");
        break;
    case zt:
        ret = QStringLiteral("zh-TW");
        break;
    case nl:
        ret = QStringLiteral("nl");
        break;
    case fr:
        ret = QStringLiteral("fr");
        break;
    case de:
        ret = QStringLiteral("de");
        break;
    case el:
        ret = QStringLiteral("el");
        break;
    case it:
        ret = QStringLiteral("it");
        break;
    case ja:
        ret = QStringLiteral("ja");
        break;
    case ko:
        ret = QStringLiteral("ko");
        break;
    case pt:
        ret = QStringLiteral("pt");
        break;
    case ru:
        ret = QStringLiteral("ru");
        break;
    case es:
        ret = QStringLiteral("es");
        break;
    case af:
        ret = QStringLiteral("af");
        break;
    case sq:
        ret = QStringLiteral("sq");
        break;
    case ar:
        ret = QStringLiteral("ar");
        break;
    case hy:
        ret = QStringLiteral("hy");
        break;
    case az:
        ret = QStringLiteral("az");
        break;
    case eu:
        ret = QStringLiteral("eu");
        break;
    case be:
        ret = QStringLiteral("be");
        break;
    case bg:
        ret = QStringLiteral("bg");
        break;
    case ca:
        ret = QStringLiteral("ca");
        break;
    case hr:
        ret = QStringLiteral("hr");
        break;
    case cs:
        ret = QStringLiteral("cs");
        break;
    case da:
        ret = QStringLiteral("da");
        break;
    case et:
        ret = QStringLiteral("et");
        break;
    case tl:
        ret = QStringLiteral("tl");
        break;
    case fi:
        ret = QStringLiteral("fi");
        break;
    case gl:
        ret = QStringLiteral("gl");
        break;
    case ka:
        ret = QStringLiteral("ka");
        break;
    case ht:
        ret = QStringLiteral("ht");
        break;
    case iw:
        ret = QStringLiteral("he");
        break;
    case hi:
        ret = QStringLiteral("hi");
        break;
    case hu:
        ret = QStringLiteral("hu");
        break;
    case is:
        ret = QStringLiteral("is");
        break;
    case id:
        ret = QStringLiteral("id");
        break;
    case ga:
        ret = QStringLiteral("ga");
        break;
    case lv:
        ret = QStringLiteral("lv");
        break;
    case lt:
        ret = QStringLiteral("lt");
        break;
    case mk:
        ret = QStringLiteral("mk");
        break;
    case ms:
        ret = QStringLiteral("ms");
        break;
    case mt:
        ret = QStringLiteral("mt");
        break;
    case no:
        ret = QStringLiteral("no");
        break;
    case fa:
        ret = QStringLiteral("fa");
        break;
    case pl:
        ret = QStringLiteral("pl");
        break;
    case ro:
        ret = QStringLiteral("ro");
        break;
    case sr:
        ret = QStringLiteral("sr");
        break;
    case sk:
        ret = QStringLiteral("sk");
        break;
    case sl:
        ret = QStringLiteral("sl");
        break;
    case sw:
        ret = QStringLiteral("sw");
        break;
    case sv:
        ret = QStringLiteral("sv");
        break;
    case th:
        ret = QStringLiteral("th");
        break;
    case tr:
        ret = QStringLiteral("tr");
        break;
    case uk:
        ret = QStringLiteral("uk");
        break;
    case ur:
        ret = QStringLiteral("ur");
        break;
    case vi:
        ret = QStringLiteral("vi");
        break;
    case cy:
        ret = QStringLiteral("cy");
        break;
    case yi:
        ret = QStringLiteral("yi");
        break;
    case bs:
        return QStringLiteral("bs");
    }
    return ret;
}

void TranslatorUtil::addItemToFromComboBox(QComboBox *combo, const QString &languageCode, const QString &translatedStr)
{
    combo->addItem(translatedStr, languageCode);
}

QString TranslatorUtil::groupTranslateName()
{
    return QStringLiteral("Translate");
}

QString TranslatorUtil::engineTranslateName()
{
    return QStringLiteral("engine");
}

QString TranslatorUtil::defaultEngineName()
{
    return QStringLiteral("google");
}

QString TranslatorUtil::loadEngine()
{
    KConfigGroup myGeneralGroup(KSharedConfig::openConfig(), groupTranslateName());
    const QString engineTypeStr = myGeneralGroup.readEntry(engineTranslateName(), defaultEngineName()); // Default google
    return engineTypeStr;
}

void TranslatorUtil::saveEngineSettings(const QString &engineName)
{
    KConfigGroup myGroup(KSharedConfig::openConfig(), groupTranslateName());
    myGroup.writeEntry(engineTranslateName(), engineName);
    myGroup.sync();
}
