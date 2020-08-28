#!/usr/bin/env perl
use strict;
use warnings;

use MaxMind::DB::Writer::Tree;
use Text::CSV;

my %types = (
    names     => 'map',
    continent => 'map',
    country   => 'map',
    iso_code  => 'utf8_string',
    en        => 'utf8_string'
);

my $tree = MaxMind::DB::Writer::Tree->new(
    ip_version            => 6,
    record_size           => 32,
    database_type         => 'Logswan',
    languages             => ['en'],
    description           => { en => 'Logswan IP geolocation test data' },
    map_key_type_callback => sub { $types{ $_[0] } }
);

my $au = {
    continent => { iso_code => "OC", names => { en => "Oceania" } },
    country   => { iso_code => "AU", names => { en => "Australia" } }
};

my $de = {
    continent => { iso_code => "EU", names => { en => "Europe" } },
    country   => { iso_code => "DE", names => { en => "Germany" } }
};

my $fr = {
    continent => { iso_code => "EU", names => { en => "Europe" } },
    country   => { iso_code => "FR", names => { en => "France" } }
};

my $us = {
    continent => { iso_code => "NA", names => { en => "North America" } },
    country   => { iso_code => "US", names => { en => "United States" } }
};

$tree->insert_network( '1.1.1.1/32', $au );
$tree->insert_network( '2.2.2.2/32', $fr );
$tree->insert_network( '3.3.3.3/32', $us );
$tree->insert_network( '4.4.4.4/32', $us );
$tree->insert_network( '5.5.5.5/32', $de );
$tree->insert_network( '6.6.6.6/32', $us );
$tree->insert_network( '7.7.7.7/32', $us );

open my $db, '>:raw', 'logswan.mmdb';
$tree->write_tree($db);
