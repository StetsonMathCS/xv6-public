#!/usr/bin/perl -w

use strict;
use Expect;

my $exp = Expect->spawn("make qemu-nox") or die "\n\nCannot start qemu with 'make qemu-nox'";

use sigtrap qw(handler stop_script INT QUIT);

sub stop_script {
    $exp->hard_close();
    die;
}

sleep(10);

$exp->send("benchmark\n");

sleep(5);

$exp->expect(100000, '-re', '(?s:\$\s*benchmark\s*(.*)\$)');

open(CSV, ">sched-benchmarks.csv");
print CSV ($exp->matchlist)[0];
close(CSV);

print "Wrote sched-benchmarks.csv\n";

$exp->send("\ca x");

$exp->hard_close();
