Name:           ruqola
Version:        1.2.2
Release:        0
Summary:        Rocket.Chat client for the KDE desktop
Source0:        %{name}-%{version}.tar.gz
Source1:        %{name}-%{version}.tar.gz.asc
Url:            https://invent.kde.org/network/ruqola
Group:          Network/InstantMessaging
License:        GPL-2.0-or-later
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Vendor:         KDE
Packager:       Klaralvdalens Datakonsult AB (KDAB) <info@kdab.com>

BuildRequires: cmake extra-cmake-modules qtkeychain-qt5-devel
%if %{defined suse_version}
BuildRequires:  python update-desktop-files libqt5-qtbase-devel libqt5-qtwebsockets-devel libqt5-qtnetworkauth-devel libqt5-qtmultimedia-devel libqt5-qtsvg-devel kcoreaddons-devel ki18n-devel kcrash-devel knotifications-devel kiconthemes-devel syntax-highlighting-devel knotifyconfig-devel kdoctools-devel kdbusaddons-devel kxmlgui-devel kio-devel sonnet-devel ktextwidgets-devel
%endif

%if %{defined fedora}
BuildRequires:  gcc-c++ shared-mime-info python desktop-file-utils qt5-qtbase-devel qt5-qtwebsockets-devel qt5-qtnetworkauth-devel qt5-qtmultimedia-devel qt5-qtsvg-devel  kf5-kcoreaddons-devel kf5-ki18n-devel kf5-kcrash-devel kf5-knotifications-devel kf5-kiconthemes-devel kf5-syntax-highlighting-devel kf5-knotifyconfig-devel kf5-kdoctools-devel kf5-kdbusaddons-devel kf5-kxmlgui-devel kf5-kio-devel kf5-sonnet-devel kf5-ktextwidgets-devel
%endif

%description
Ruqola is a Rocket.Chat client for the KDE desktop.

It supports multi-account, search in room, open close rooms, direct message, thread, discussions.
RC settings can be changed directly.

Maintainer:
----------
     Laurent Montel <montel@kde.org>

%define debug_package %{nil}
%global __debug_install_post %{nil}

%prep
%setup -T -c
%{__tar} -zxf %{SOURCE0} --strip-components=1

%build
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=False
%__make %{?_smp_mflags}

%install
%make_install

%clean
%__rm -rf "%{buildroot}"

%files
%defattr(-,root,root)
%{_prefix}/bin/ruqola
%{_libdir}/libruqolacore.*
%{_libdir}/libruqolawidgets.*
%{_libdir}/librocketchatrestapi-qt5.*
%dir %{_libdir}/qt5/plugins/ruqolaplugins/
%{_libdir}/qt5/plugins/ruqolaplugins/authentication/
%{_libdir}/qt5/plugins/ruqolaplugins/textplugins/
%{_prefix}/share/applications/org.kde.ruqola.desktop
%{_prefix}/share/icons/hicolor
%{_prefix}/share/doc/HTML/en/ruqola/
%{_prefix}/share/knotifications5/ruqola.notifyrc
%{_prefix}/share/metainfo/org.kde.ruqola.appdata.xml
%{_prefix}/share/qlogging-categories5/ruqola.categories

%changelog
* Sat Dec 12 2020 Allen Winter <allen.winter@kdab.com> 1.2.2
- 1.2.2 release
* Sun Dec 06 2020 Allen Winter <allen.winter@kdab.com> 1.2.1
- 1.2.1 release
* Tue Dec 01 2020 Allen Winter <allen.winter@kdab.com> 1.1.98
- 1.2.0 beta2
