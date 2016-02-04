#!/usr/bin/perl -w

use strict;
use Expect;


my $exp = Expect->spawn("make qemu-nox") or die "\n\nCannot start qemu with 'make qemu-nox'";

sleep(5);

$exp->send("benchmark\n");

sleep(5);

$exp->expect(100000, '-re', '(?s:\$\s*benchmark\s*(.*)\$)');

open(CSV, ">sched-benchmarks.csv");
print CSV ($exp->matchlist)[0];
close(CSV);

print "Wrote sched-benchmarks.csv\n";

$exp->send("\ca x");

$exp->hard_close();
