Name:           logswan
Version:        2.1.14
Release:        1%{?dist}
Summary:        Fast Web log analyzer using probabilistic data structures

License:        BSD-2-Clause
URL:            https://github.com/fcambus/logswan
Source0:        %{url}/releases/download/%{version}/%{name}-%{version}.tar.gz

BuildRequires:  jansson-devel
BuildRequires:  libmaxminddb-devel
BuildRequires:  cmake
BuildRequires:  gcc

%description
Logswan is a fast Web log analyzer using probabilistic data structures. It is
targeted at very large log files, typically APIs logs. It has constant memory
usage regardless of the log file size, and takes approximatively 4MB of RAM.

Unique visitors counting is performed using two HyperLogLog counters (one for
IPv4, and another one for IPv6), providing a relative accuracy of 0.10%.

Project design goals include : speed, memory-usage efficiency, and keeping the
code as simple as possible.

%prep
%autosetup

%build
%cmake .
%cmake_build

%install
%cmake_install

%check

%files
%{_bindir}/%{name}
%{_mandir}/man1/%{name}.1*
%license LICENSE
%doc README.md
