```
                                _____
                            .xiX*****Xix.
                          .X7'        '4Xk,
                         dXl            'XX.        .
                        xXXl             XXl        .
                        4XXX             XX'
                       .  ,x            iX'   _,,xxii
                       |   ²|        ,iX7,xiiXXXXXXXl
                       |          .xi,xiXXXXXXXXXXXX:
                       .      ..iXXiXXXXXXXXXXXXXXX7.
                       .    .xXXXXXXXXXXXXXXX'XXXX7 .
                       |   ,XXXXXXXXXXXXXXXX'XXX7'  |
                       :  .XXXXX7*'"' 2XXX7'XX7'    |
  __/ \     _____    ____  \XX' _____  47'  ___  ___      _____     __
.\\_   \___/  _  \__/  _/_______\  _/______/  /  \  \____/  _  \___/  \  _____
. /     __    Y _ __   \__  _________  _____  \/\/   ____ _ _   ______ \/ __///
:/       /    |    \    |'   \/   \/    \/            \/    Y    \/   \    \  :
|\______/\_________/____|    /\____     /\_____/\_____/\____|____/\____\___/  |
+--------------------- \____/ --- \____/ ----:----------------------h7/dS!----+
                       .                     |      :
                       : .                   :      |
                       | .     Logswan       .      |
                       | :                       .  |
                       |_|_______________________|__|
                         |                       :
                                                 .
```
# Logswan

[![Build Status][1]][2]

Logswan is a fast Web log analyzer using probabilistic data structures. It is
targeted at very large log files, typically APIs logs. It has constant memory
usage regardless of the log file size, and takes approximatively 4MB of RAM.

Unique visitors counting is performed using two HyperLogLog counters (one for
IPv4, and another one for IPv6), providing a relative accuracy of 0.10%.
String representations of IP addresses are used and preferred as they offer
better precision.

Project design goals include: speed, memory-usage efficiency, and keeping the
code as simple as possible.

Logswan is **opinionated software**:

- It only supports the Common Log Format, in order to keep the parsing code
  simple. It can of course process the Combined Log Format as well (referer
  and user agent fields will be discarded)
- It does not split results per day, but log files can be split prior to
  being processed
- Input file size and bandwidth usage are reported in bytes, there are no
  plans to format or round them

Logswan is written with security in mind and is running sandboxed on OpenBSD
(using pledge). It has also been extensively fuzzed using AFL and Honggfuzz.

## Features

Currently implemented features:

- Counting used bandwidth
- Counting number of processed lines / invalid lines
- Counting number of hits (IPv4 and IPv6 hits)
- Counting visits (unique IP addresses for both IPv4 and IPv6)
- GeoIP lookups (for both IPv4 and IPv6)
- Hourly hits distribution
- HTTP method distribution
- HTTP protocol (HTTP/1.0, HTTP/1.1, or HTTP/2.0) distribution
- HTTP status codes distribution

## Dependencies

Logswan uses the `CMake` build system and requires `Jansson` and `libmaxminddb`
libraries and header files.

## Installing dependencies

- OpenBSD: `pkg_add -r cmake jansson libmaxminddb`
- NetBSD: `pkgin in cmake jansson libmaxminddb`
- FreeBSD: `pkg install cmake jansson libmaxminddb`
- Mac OS X: `brew install cmake jansson libmaxminddb`
- Alpine Linux: `apk add cmake gcc make musl-dev jansson-dev libmaxminddb-dev`
- Debian / Ubuntu: `apt-get install build-essential cmake libjansson-dev libmaxminddb-dev`

## Building

	mkdir build
	cd build
	cmake ..
	make

Logswan has been successfully built and tested on OpenBSD, NetBSD, FreeBSD,
Mac OS X, and Linux with both Clang and GCC.

## Packages

Packages are available for the following operating systems:

- [OpenBSD][3]
- [NetBSD][4]
- [Debian][5]
- [Ubuntu][6]
- [Void Linux][7]

### GeoIP2 databases

Logswan looks for GeoIP2 databases in `${CMAKE_INSTALL_PREFIX}/share/GeoIP2` by
default, which points to `/usr/local/share/GeoIP2`.

A custom directory can be set using the `GEOIP2DIR` variable when invoking
CMake:

	cmake -DGEOIP2DIR=/var/db/GeoIP .

The free GeoLite2 databases from MaxMind can be downloaded [here][8].

## Usage

	logswan [-ghv] [-d db] file

If file is a single dash (`-'), logswan reads from the standard input.

The options are as follows:

	-d db	Specify path to a GeoIP database.
	-g	Enable GeoIP lookups.
	-h	Display usage.
	-v	Display version.

Logswan outputs JSON data to **stdout**.

## Measuring Logswan memory usage

Heap profiling can be done using valgrind, as follows:

	valgrind --tool=massif logswan access.log
	ms_print massif.out.*

## License

Logswan is released under the BSD 2-Clause license. See `LICENSE` file for
details.

## Author

Logswan is developed by Frederic Cambus.

- Site: https://www.cambus.net

## Resources

Project homepage: https://www.logswan.org

GitHub: https://github.com/fcambus/logswan

[1]: https://api.travis-ci.org/fcambus/logswan.png?branch=master
[2]: https://travis-ci.org/fcambus/logswan
[3]: https://cvsweb.openbsd.org/cgi-bin/cvsweb/ports/www/logswan
[4]: http://pkgsrc.se/www/logswan
[5]: https://packages.debian.org/search?keywords=logswan
[6]: https://packages.ubuntu.com/search?keywords=logswan
[7]: https://github.com/void-linux/void-packages/tree/master/srcpkgs/logswan
[8]: https://dev.maxmind.com/geoip/geoip2/geolite2/
