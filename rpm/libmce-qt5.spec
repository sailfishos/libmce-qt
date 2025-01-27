Name:       libmce-qt5

Summary:    A library of Qt bindings for mce
Version:    1.5.1
Release:    1
License:    BSD
URL:        https://github.com/sailfishos/libmce-qt
Source0:    libmce-qt5-%{version}.tar.bz2
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(mce) >= 1.30.0
Requires:       mce >= 1.110.0

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
%qmake5 "VERSION=$(sed 's/+.*//' <<<"%{version}")"
%make_build

%install
%qmake5_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%{_libdir}/%{name}.so.*

%files declarative
%{_libdir}/qt5/qml/Nemo/Mce

%files devel
%{_libdir}/%{name}.so
%{_libdir}/pkgconfig/mce-qt5.pc
%{_includedir}/mce-qt5/*.h
