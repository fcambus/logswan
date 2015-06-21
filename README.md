# Logswan

Logswan is a Web log analyzer in early stages of development. It is targeted at very large log files, typically APIs logs.

Project design goals include : speed, memory-usage efficiency, and keeping the code as simple as possible.

## Features

Currently implemented features :

- Counting number of processed lines / invalid lines
- Counting number of hits (IPv4 and IPv6 hits)
- Counting bandwidth used
- Counting log file size
- GeoIP lookups (IPv4 only for now)

## Dependencies

Logswan requires the GeoIP library and header files.

## Compiling

Logswan uses the CMake build system :

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
