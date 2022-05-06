#!/usr/bin/env perl
use strict;
use warnings;

use MaxMind::DB::Writer::Tree;
use Text::CSV;

my %types = (
    names     => 'map',
    continent => 'map',
    country   => 'map',
    code      => 'utf8_string',
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
    continent => { code     => "OC" },
    country   => { iso_code => "AU", names => { en => "Australia" } }
};

my $de = {
    continent => { code     => "EU" },
    country   => { iso_code => "DE", names => { en => "Germany" } }
};

my $fr = {
    continent => { code     => "EU" },
    country   => { iso_code => "FR", names => { en => "France" } }
};

my $us = {
    continent => { code     => "NA" },
    country   => { iso_code => "US", names => { en => "United States" } }
};

$tree->insert_network( '1.1.1.1/32', $au );
$tree->insert_network( '2.2.2.2/32', $fr );
$tree->insert_network( '3.3.3.3/32', $us );
$tree->insert_network( '4.4.4.4/32', $us );
$tree->insert_network( '5.5.5.5/32', $de );
$tree->insert_network( '6.6.6.6/32', $us );
$tree->insert_network( '7.7.7.7/32', $us );

$tree->insert_network( '::ffff:101:101/128', $au );
$tree->insert_network( '::ffff:202:202/128', $fr );
$tree->insert_network( '::ffff:303:303/128', $us );
$tree->insert_network( '::ffff:404:404/128', $us );
$tree->insert_network( '::ffff:505:505/128', $de );
$tree->insert_network( '::ffff:606:606/128', $us );
$tree->insert_network( '::ffff:707:707/128', $us );

open my $db, '>:raw', 'logswan.mmdb';
$tree->write_tree($db);
