#!/usr/bin/perl
#
# anl.pl - Generate "abstract" netlist
#
# This generates one line per net, containing all pins connecting to this
# net. Each pin is identified as <value>.<pin> so that this list is stable
# when re-annotating schematics. (However, it is ambiguous.)
#
# Each net line is sorted alphabetically and nets are also sorted.
#
# The purpose of this script is to make it possible to compare netlists before
# and after major refactoring of the schematics.
#

sub flush
{
	push(@nets, join(" ", sort({ $a cmp $b } @net)));
	undef @net;
}


while (<>) {
	if (/\(comp\s+\(ref\s+(\S+?)\)/) {
		$comp = $1;
		next;
	}
	if (/\(value\s+"([^"]+)"/) {
		$c{$comp} = $1 if defined $comp;
		next;
	}
	if (/\(value\s+(\S+?)\)/) {
		$c{$comp} = $1 if defined $comp;
		next;
	}
	if (/\(net\s/) {
		&flush unless $#net == -1;
		next;
	}
	if (/\(node\s+\(ref\s+(\S+?)\)\s+\(pin\s+(\S+?)\)/) {
		die "unknown component \"$1\"" unless defined $c{$1};
		push(@net, "$c{$1}.$2");
	}
}
&flush;

print join("\n", sort({ $a cmp $b } @nets)), "\n";
