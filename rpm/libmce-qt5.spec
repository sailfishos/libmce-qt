Name:       libmce-qt5

Summary:    A library of Qt bindings for mce
Version:    1.2.0
Release:    1
Group:      System/Libraries
License:    BSD
URL:        https://git.merproject.org/mer-core/libmce-qt5
Source0:    libmce-qt5-%{version}.tar.bz2
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(mce) >= 1.28.0
Requires:       mce >= 1.104.0

%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}

%description
This package contains Qt bindings for mce

%package declarative
Summary:    Declarative plugin for libmce-qt5
Group:      Development/Tools
Requires:   %{name} = %{version}-%{release}
Requires:   %{name} = %{version}

%description declarative
This package contains declarative plugin for libmce-qt5

%package devel
Summary:    Development files for libmce-qt5
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires:   %{name} = %{version}

%description devel
This package contains the development header files for libmce-qt5

%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5 "VERSION=$(sed 's/+.*//' <<<"%{version}")"
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/%{name}.so.*

%files declarative
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/Nemo/Mce

%files devel
%defattr(-,root,root,-)
%{_libdir}/%{name}.so
%{_libdir}/pkgconfig/mce-qt5.pc
%{_includedir}/mce-qt5/*.h
