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

Logswan is a Web log analyzer in early stages of development. It is targeted at very large log files, typically APIs logs. Project design goals include : speed, memory-usage efficiency, and keeping the code as simple as possible.

Logswan is **opinionated software** :

- It only supports the Common Log Format, in order to keep the parsing code simple
- It does not split results per day, but log files can be split prior to being processed

## Features

Currently implemented features :

- Counting number of processed lines / invalid lines
- Counting number of hits (IPv4 and IPv6 hits)
- Counting bandwidth used
- Counting log file size
- GeoIP lookups (IPv4 only for now)
- HTTP status codes distribution
- Hourly hits distribution
- Counting hits per protocol (HTTP/1.0 or HTTP/1.1)
- Counting hits per HTTP Method

## Dependencies

Logswan uses the `CMake` build system and requires `GeoIP` and `Jansson` libraries and header files.

## Installing dependencies

- OpenBSD : `pkg_add -r cmake GeoIP jansson`
- NetBSD : `pkgin install cmake GeoIP jansson`
- Mac OS X : `brew install cmake geoip jansson`
- Linux (Debian / Ubuntu): `apt-get install cmake libgeoip-dev libjansson-dev`

## Compiling

	cmake .
	make

## Usage 

Logswan takes the input log file as parameter :

	logswan inputfile

## License

Logswan is released under the BSD 3-Clause license. See `LICENSE` file for details.

## Author

Logswan is developed by Frederic Cambus

- Site : http://www.cambus.net
- Twitter: http://twitter.com/fcambus

## Resources

GitHub : https://github.com/fcambus/logswan
