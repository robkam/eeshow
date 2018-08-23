#!/usr/bin/perl
#
# libchk.pl - Check pins in Eeschema libraries
#

#
# The component editor has a check function, but this suffers the following
# limitations:
#
# - one has to run it on every component, making it easy to miss some errors,
# - the off-grid detection doesn't seem to detect pins off the current grid,
#   and even less off the standard 50 mil grid, and
# - duplicate pin detection seems to miss duplicate pins in different units.
#


sub check
{
	local ($part, $cvt, @a) = @_;

	my $name = $a[1];
	my $num = $a[2];
	my $x = $a[3];
	my $y = $a[4];
	my $unit = $a[9];

	warn "$part.$name ($x, $y) is off 50 mil grid\n"
	    if ($x % 50) || ($y % 50);

	if (defined $p[$cvt]{$num}) {
		warn "$part($cvt): $num used for $p[$cvt]{$num} and $name\n";
		return 0;
	}
	$p[$cvt]{$num} = $name;

	return 1;
}


while (<>) {
	if (/^DEF\s(\S+)/) {
		$part = $1;
		undef @p;
		next;
	}
	next unless /^X/;

	my @a = split(/\s+/);
	my $cvt = $a[10];

	if ($cvt) {
		foreach $cvt (1, 2) {
			last unless &check($part, $cvt, @a);
		}
	} else {
		&check($part, $cvt, @a);
	}
}
