if (Qt5QuickCompiler_FOUND AND NOT DISABLE_QTQUICK_COMPILER)
    qtquick_compiler_add_resources(RuqolaResources qml/qml.qrc)
else ()
    qt5_add_resources(RuqolaResources qml/qml.qrc)
endif()

# Add QML file as sources so they are available in QtCreator searches
file(GLOB ruqola_qml_files */*.qml)

add_executable(ruqolaqml main.cpp ${RuqolaResources} ${ruqola_qml_files})

target_link_libraries(ruqolaqml libruqolacore
    librocketchatrestapi-qt5
    KF5::Crash
)

if (ANDROID)
    target_link_libraries(ruqolaqml Qt5::Svg)
endif()

install(TARGETS ruqolaqml ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

if (ANDROID)
    kirigami_package_breeze_icons(ICONS
        go-down-symbolic
        go-next-symbolic
        go-up-symbolic
        view-refresh
        search
        list-add
        window-close-symbolic
        preferences-desktop-notification
        system-users
        edit-find
        favorite
        draw-arrow-back
        network-workgroup
        ruqola
        lock
        irc-channel-inactive
        irc-channel-active
        user-available
        user-online
        user-busy
        user-away
        user-offline
        im-user-online
        im-user-busy
        im-user-away
        im-user-offline
        list-remove
        document-edit
        tools-report-bug
        settings-configure
        system-help
        system-log-out
        application-exit
        hide_table_row
        dialog-close
        edit-symbolic
        folder
        document-send-symbolic
        face-smile
        mail-sent
        edit-delete-shred
        cloud-download
        media-playback-start
        hint
        visibility
        documentinfo
    )
endif()
