#!/usr/bin/perl

sub usage
{
	print STDERR "usage: $0 [-c] [-i] map.in file.sch ...\n";
	exit(1);
}


while ($ARGV[0] =~ /^-/) {
	if ($ARGV[0] eq "-c") {
		$cache = 1;
	} elsif ($ARGV[0] eq "-i") {
		$ignore_errors = 1;
	} else {
		&usage;
	}
	shift @ARGV;
}

open(MAP, $ARGV[0]) || die "$ARGV[0]: $!";
while (<MAP>) {
	chop;
	if (/^\s*$/) {
		undef $key;
		next;
	}
	next if /^\s*#/;
	if (!defined $key) {
		$key = $_;
		$key =~ s/([.{}()+[\]])/\\\1/g;
		$key =~ s/\*/.*/g;
		$key =~ s/\?/./g;
		next;
	}
	die "bad document line" unless /^("[^"]*"|\S+)\s+/;
	$xtag = "$1";
	$url = "$'";
#print STDERR "$url \n\n";
	if ($url =~ "^\`") {
	    $url = substr($url, 1);
#print STDERR "# $url \n";
	    $url = `$url`;
#print STDERR "# $url \n";
	}
	push(@keys, $key);
	push(@tags, $xtag);
	push(@s, $url);
	next unless $cache;
	next if $xtag eq "FP" || $xtag eq "PKG";
	system("eeshow-viewer", "-c", $url) && push(@failed, $url);
}
close MAP;

shift @ARGV;


sub ith
{
	local ($i) = @_;

	$tag = $tags[$i];
	if ($tag eq "DS") {
		print "Data sheet\n";
	} elsif ($tag eq "TRM") {
		print "TRM\n";
	} elsif ($tag =~ /^"([^"]*)"$/) {
		print "$1\n";
	} elsif ($tag eq "FP" || $tag eq "PKG") {
		if ($tag eq "FP") {
			print "Footprint\n";
		} else {
			print "Package\n";
		}
		if ($s[$i] =~ /^(\d+)\s*$/) {
			print "$s[$i - 1]#$1\n\n";
			return;
		}
	} else {
		die "unknown tag \"$tag\"";
	}
	print "$s[$i]\n\n";
}


while (<>) {
	if (/^\$Comp/) {
		undef @f;
		undef $unit;
		next;
	}
	if (/^U\s+(\d+)/) {
		$unit = $1;
	}
	if (/^F\s+(\d+)\s+"([^"]*)"/) {
		$f[$1] = $2;
		next;
	}
	if (/^\$EndComp/) {
#print STDERR "|$f[0]_$f[1]| $unit\n";
		for ($i = 0; $i <= $#keys; $i++) {
#print STDERR "    |$keys[$i]|\n";
			next unless "$f[0]_$f[1]" =~ /^$keys[$i]$/;
			$sfx = 'A';
			for ($u = $unit; $u > 1; $u--) {
				$sfx++;
			}
			# @@@ hack: we should check in the library if the part
			# has multiple units, and generate the component
			# reference accordingly.
			if ($unit < 2)  {
				print "$f[0]\n";
				&ith($i);
			}
			print "$f[0]$sfx\n";
			&ith($i);
		}
	}
}

exit unless $#failed > -1;

printf STDERR "The following download%s failed:\n", $#failed ? "s" : "";
for (@failed) {
	print STDERR "$_\n";
}
exit if $ignore_errors;
exit(1);
