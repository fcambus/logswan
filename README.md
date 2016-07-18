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

Logswan is a fast Web log analyzer using probabilistic data structures. It is targeted at very large log files, typically APIs logs. It has constant memory usage regardless of the log file size, and takes approximatively 4MB of RAM.

Unique visitors counting is performed using two HyperLogLog counters (one for IPv4, and another one for IPv6), providing a relative accuracy of 0.10%. String representations of IP addresses are used and preferred as they offer better precision.

Project design goals include : speed, memory-usage efficiency, and keeping the code as simple as possible.

Logswan is **opinionated software** :

- It only supports the Common Log Format, in order to keep the parsing code simple. It can of course process the Combined Log Format as well (referer and user agent fields will be discarded)
- It does not split results per day, but log files can be split prior to being processed
- Input file size and bandwidth usage are reported in bytes, there are no plans to format or round them

## Features

Currently implemented features :

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

Logswan uses the `CMake` build system and requires `GeoIP` and `Jansson` libraries and header files.

## Installing dependencies

- OpenBSD : `pkg_add -r cmake GeoIP jansson`
- NetBSD : `pkgin install cmake GeoIP jansson`
- Mac OS X : `brew install cmake geoip jansson`
- Alpine Linux : `apk add cmake gcc make musl-dev geoip-dev jansson-dev`
- Debian / Ubuntu : `apt-get install build-essential cmake libgeoip-dev libjansson-dev`

## Compiling

	cmake .
	make

Logswan has been sucessfully compiled and tested on Mac OS X, OpenBSD, NetBSD, and Linux with both Clang and GCC.

## Installation

Logswan packages are available for :

### OpenBSD

	pkg_add logswan

### Pkgsrc (NetBSD, SmartOS, Mac OS X, etc.)

	pkgin install logswan

### GeoIP databases

By default, Logswan looks for GeoIP databases in `${CMAKE_INSTALL_PREFIX}/share/GeoIP`, which points to `/usr/local/share/GeoIP` by default.

A custom directory can be set using the `GEOIPDIR` variable when invoking CMake :

	cmake -DGEOIPDIR=/var/db/GeoIP .

The free GeoLite databases can be downloaded here : http://dev.maxmind.com/geoip/legacy/geolite/

#### For IPv4 support only :

	https://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz

#### For IPv4 and IPv6 support :

	https://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz
	https://geolite.maxmind.com/download/geoip/database/GeoIPv6.dat.gz

## Usage 

	logswan [-hv] file

If file is a single dash (`-'), logswan reads from the standard input.

Options are :

	-h Display usage
	-v Display version

Logswan outputs JSON data to **stdout**. 

## Measuring Logswan memory usage

Heap profiling can be done using valgrind, as follow :

	valgrind --tool=massif logswan access.log
	ms_print massif.out.*

## License

Logswan is released under the BSD 2-Clause license. See `LICENSE` file for details.

## Author

Logswan is developed by Frederic Cambus

- Site : http://www.cambus.net
- Twitter: http://twitter.com/fcambus

## Resources

Project Homepage : http://www.logswan.org

GitHub : https://github.com/fcambus/logswan
