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
# Logswan [![Build Status][1]][2] [![Coverity Scan Build Status][3]][4]

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

## Features

Currently implemented features:

- Counting bandwidth used
- Counting number of processed lines / invalid lines
- Counting number of hits (IPv4 and IPv6 hits)
- Counting visits (unique IP addresses for both IPv4 and IPv6)
- GeoIP lookups (for both IPv4 and IPv6)
- Hourly hits distribution
- HTTP method distribution
- HTTP protocol (HTTP/1.0 or HTTP/1.1) distribution
- HTTP status codes distribution

## Dependencies

Logswan uses the `CMake` build system and requires `Jansson` and `libmaxminddb`
libraries and header files.

## Installing dependencies

- OpenBSD: `pkg_add -r cmake jansson libmaxminddb`
- Mac OS X: `brew install cmake jansson libmaxminddb`
- Alpine Linux: `apk add cmake gcc make musl-dev jansson-dev libmaxminddb-dev`
- Debian / Ubuntu: `apt-get install build-essential cmake libjansson-dev libmaxminddb-dev`

## Compiling

	cmake .
	make

Logswan has been sucessfully compiled and tested on Mac OS X, OpenBSD, NetBSD,
and Linux with both Clang and GCC.

## Installation

Logswan packages are available for:

### OpenBSD

	pkg_add logswan

### Pkgsrc (NetBSD, SmartOS, Mac OS X, etc.)

	pkgin install logswan

### GeoIP2 databases

Logswan looks for GeoIP2 databases in `${CMAKE_INSTALL_PREFIX}/share/GeoIP2` by
default, which points to `/usr/local/share/GeoIP2`.

A custom directory can be set using the `GEOIP2DIR` variable when invoking
CMake:

	cmake -DGEOIP2DIR=/var/db/GeoIP .

The free GeoLite2 databases from MaxMind can be downloaded [here][5]:

	https://geolite.maxmind.com/download/geoip/database/GeoLite2-Country.tar.gz

## Usage

	logswan [-ghv] file

If file is a single dash (`-'), logswan reads from the standard input.

Options are:

	-g Enable GeoIP lookups
	-h Display usage
	-v Display version

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
[3]: https://scan.coverity.com/projects/10944/badge.svg
[4]: https://scan.coverity.com/projects/10944
[5]: https://dev.maxmind.com/geoip/geoip2/geolite2/
