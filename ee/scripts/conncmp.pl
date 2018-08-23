#!/usr/bin/perl
#
# conncmp.pl - Compare signal names on connectors
#

sub usage
{
	print STDERR <<"EOF";
usage: $0 list-a.net comp-a list-b.net comp-b pat-a=pat-b ...

  patterns can be constant, e.g., GND=GND3
  or variable, e.g., %=%_U
  Nets connecting to no other component are renamed to NC.
EOF
	exit(1);
}


sub get_pins
{
	local ($file, $comp) = @_;

	my %pins = ();
	my $net = undef;
	my $single = undef;
	my $conns = 0;

	open(FILE, $file) || die "$file: $!";
	while (<FILE>) {
		if (/\(net\s+.*\(name\s+"([^"]+)"\)/ ||
		    /\(net\s+.*\(name\s+(\S+?)\)/) {
			$pins{$single} = "NC" if $conns == 1;
			$net = $1;
			$net =~ s#^/.*/##;
			$single = undef;
			$conns = 0;
			next;
		}
		next unless /\(node\s+\(ref\s+(\S+?)\)\s+\(pin\s+(\S+?)\)/;

		$conns++;
		next unless $1 eq $comp;
		die "duplicate pin $1.$2" if defined $pins{$2};
		die "undefined net" unless defined $net;
		$pins{$2} = $net;
		$single = $2;
	}
	close(FILE);
	return \%pins;
}


&usage unless $#ARGV >= 3;
%a = %{ &get_pins($ARGV[0], $ARGV[1]) };
%b = %{ &get_pins($ARGV[2], $ARGV[3]) };
@eq = @ARGV[4 .. $#ARGV];

for (@eq) {
	&usage unless $_ =~ /=/;
}

PIN: for $pin (keys %a) {
	if (!defined $b{$pin}) {
		warn "A.$pin has no matching B.$pin\n";
		next;
	}
	my $a = $a{$pin};
	my $b = $b{$pin};
	delete $b{$pin};
	next if $a eq $b;
	for (@eq) {
		die unless /=/;
		my ($pa, $pb) = ($`, $');
		if ($pa =~ /%/) {
			my $pat = "^$`(.*)$'\$";
			next unless $a =~ $pat;
			my $var = $1;
			$pb =~ s/%/$var/g;
			next PIN if $b eq $pb;
		} else {
			next PIN if $a eq $pa && $b eq $pb;
		}
	}
	die "A.$pin($a) does not seem to match B.$pin($b)\n";
}

for $pin (keys %b) {
	warn "B.$pin has no matching A.$pin\n";
}
