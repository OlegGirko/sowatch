# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
# 

Name:       sowatch

# >> macros
# << macros

%{!?_qt5_qmake_flags:%global _qt5_qmake_flags %{?nil: \\
  QMAKE_CFLAGS_DEBUG="${CFLAGS:-%{_qt5_optflags} %{?_qt5_cflags}}" \\
  QMAKE_CFLAGS_RELEASE="${CFLAGS:-%{_qt5_optflags} %{?_qt5_cflags}}" \\
  QMAKE_CXXFLAGS_DEBUG="${CXXFLAGS:-%{_qt5_optflags} %{?_qt5_cxxflags}}" \\
  QMAKE_CXXFLAGS_RELEASE="${CXXFLAGS:-%{_qt5_optflags} %{?_qt5_cxxflags}}" \\
  QMAKE_LFLAGS_DEBUG="${LDFLAGS:-%{_qt5_ldflags}}" \\
  QMAKE_LFLAGS_RELEASE="${LDFLAGS:-%{_qt5_ldflags}}" \\
  QMAKE_STRIP= \\
  %nil
}}
%{!?qmake5:%global qmake5 %{_qt5_bindir}/qmake %{?_qt5_qmake_flags}}
%{!?qmake5_install:%global qmake5_install make install INSTALL_ROOT=%{buildroot}}
%{!?qtc_qmake5:%global qtc_qmake5 %qmake5}
%{!?qtc_make:%global qtc_make make}
Summary:    A tool for using smart watches
Version:    0.1.0
Release:    1
Group:      Qt/Qt
License:    GPLv2
Source0:    %{name}-%{version}.tar.bz2
Source100:  sowatch.yaml
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5Declarative)
BuildRequires:  pkgconfig(Qt5Bluetooth)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(gconf-2.0)

%description
sowatch is a tool that allows you to use smart watches with your phone.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5 

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

%files
%defattr(-,root,root,-)
/opt/%{name}
%{_libdir}/lib%{name}*
%{_libdir}/%{name}
%{_includedir}/%{name}
%{_datadir}/%{name}
%{_bindir}/%{name}d
/opt/sowatch/bin
/usr/share/sowatch
/opt/sowatch/.
/usr/bin
/usr/lib
/usr/include/sowatch
/usr/share/sowatch/i18n
/usr/lib/sowatch/watchlets
/usr/share/sowatch/qml/notificationswatchlet
/usr/lib/sowatch/drivers
/usr/share/sowatch/metawatch
/usr/share/sowatch/qml
/usr/share/sowatch/qml/nekowatchlet
/usr/share/sowatch/qml/metawatchwatchlets
# >> files
# << files
