#!/usr/bin/perl
$MAGIC = "Xsheetnumber";

sub usage
{
	print STDERR "usage: $0 [-q] [-xy|-yx] [top-sheet.sch]\n";
	exit(1);
}


while ($ARGV[0] =~ /^-/) {
	if ($ARGV[0] eq "-q") {
		$query = 1;
	} elsif ($ARGV[0] eq "-xy") {
		$xy = 1;
	} elsif ($ARGV[0] eq "-yx") {
		$yx = 1;
	} else {
		&usage;
	}
	shift @ARGV;
}

$where = "head";
$sheet = 1;
while (<>) {
	if (/^\$Sheet/) {
		$where = "sheet";
		$tmp = $_;
		$sheet++;
		next;
	}
	if ($where eq "head") {
		$head .= $_;
		next;
	}
	if ($where ne "sheet") {
		$tail .= $_;
		next;
	}
	if (/^\$EndSheet/) {
		$where = "outside";
		$tmp .= $_;

		if ($xy || $yx) {
			push(@x, $x);
			push(@y, $y);
			push(@unsorted, $tmp);
			next;
		}

		die "file name not found" unless $tmp =~ /^F1\s+"([^"]+)"/ms;
		$file = $1;
		open(FILE, $file) || die "$file: $!";

		undef $explicit;
		while (<FILE>) {
			next unless /^Comment4\s+"$MAGIC=([1-9]\d*)"/;
			$explicit = $1;
			last;
		}
		close FILE;

		if (!defined $explicit) {
			warn "$file: sheet $sheet has no explicit tag\n";
			$explicit = $sheet;
		}
		die "$file: sheet number must be 2 or higher\n"
		    if $explicit < 2;
		die "$file: collision on number $explicit\n"
		    if defined $sorted[$explicit];
		$sorted[$explicit] = $tmp;
		$was[$explicit] = $sheet;
		next;
	}
	if (/^S\s+(\d+)\s+(\d+)/) {
		$x = $1;
		$y = $2;
	}
	$tmp .= $_;
}


sub map
{
	local ($a, $b) = @_;

	return $x[$a] == $x[$b] ? $y[$a] <=> $y[$b] : $x[$a] <=> $x[$b]
	    if $xy;
	return $y[$a] == $y[$b] ? $x[$a] <=> $x[$b] : $y[$a] <=> $y[$b];
}


if ($xy || $yx) {
	my @i = sort({ &map($a, $b) } ( 0 .. $#unsorted ));

	for (0..$#i) {
#print STDERR "$_ <- $i[$_]  $x[$i[$_]] $y[$i[$_]]\n";
		$sorted[$_ + 2] = $unsorted[$i[$_]];
		$was[$_ + 2] = $i[$_] + 2;
	}
}

print $head || die "write: $!" unless $query;
for ($i = 2; $i <= $#sorted; $i++) {
	if (!defined $sorted[$i]) {
		warn "sheet #$i is missing in sequence\n";
		next;
	}
	if ($was[$i] != $i) {
		exit(1) if $query;
		warn "sheet $was[$i] is now $i\n" unless $was[$i] == $i;
	}
	print $sorted[$i] || die "write: $!" unless $query;
}
exit(0) if $query;
print $tail || die "write: $!";
close(STDOUT) || die "write: $!";
