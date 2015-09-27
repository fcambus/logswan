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

Logswan is a Web log analyzer using probabilistic data structures. It is targeted at very large log files, typically APIs logs. It has constant memory usage regardless of the log file size.

Project design goals include : speed, memory-usage efficiency, and keeping the code as simple as possible.

Logswan is **opinionated software** :

- It only supports the Common Log Format, in order to keep the parsing code simple
- It does not split results per day, but log files can be split prior to being processed

## Features

Currently implemented features :

- Counting number of processed lines / invalid lines
- Counting number of hits (IPv4 and IPv6 hits)
- Counting bandwidth used
- Counting log file size
- GeoIP lookups (for both IPv4 and IPv6)
- HTTP status codes distribution
- Hourly hits distribution
- Counting hits per protocol (HTTP/1.0 or HTTP/1.1)
- Counting hits per HTTP Method
- Counting unique IP addresses (for both IPv4 and IPv6)

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

### GeoIP databases

Logswan requires the free GeoLite databases to be installed in `/usr/local/share/GeoIP`.
Files can be downloaded here : http://dev.maxmind.com/geoip/legacy/geolite/

#### For IPv4 support only :

  mkdir -p /usr/local/share/GeoIP
  cd /usr/local/share/GeoIP
  wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz
  gunzip *gz

#### For IPv4 and IPv6 support :

  mkdir -p /usr/local/share/GeoIP
  cd /usr/local/share/GeoIP
  wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz
  wget http://geolite.maxmind.com/download/geoip/database/GeoIPv6.dat.gz
  gunzip *gz

## Usage 

	USAGE : logswan [options] inputfile

Options are :

	-h Display usage
	-v Display version

## License

Logswan is released under the BSD 3-Clause license. See `LICENSE` file for details.

## Author

Logswan is developed by Frederic Cambus

- Site : http://www.cambus.net
- Twitter: http://twitter.com/fcambus

## Resources

Project Homepage : http://www.logswan.org

GitHub : https://github.com/fcambus/logswan
