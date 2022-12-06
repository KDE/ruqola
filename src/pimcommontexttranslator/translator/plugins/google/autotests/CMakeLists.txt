ecm_add_test(googleengineclienttest.cpp googleengineclienttest.h ../googleengineclient.cpp ../googleengineplugin.cpp
    TEST_NAME googleengineclienttest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n
)
