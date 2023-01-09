#!/usr/bin/perl
#------------------------------------------------------------------------------#
# THIS IS A TEST SCRIPT (DO NOT EXECUTE IT)                                    #
#------------------------------------------------------------------------------#

use strict;
use warnings;

my $crs = "";
my $files_changed = 0;
my $lines_added = 0;
my $lines_removed = 0;
my $total = 0;
my @messages = "";

print "<ul>";

sub print_crs {
  if ($crs ne "") {
    print "<li>$crs (files $files_changed, lines +$lines_added -$lines_removed)<ul>\n";

    for my $message (@messages) {
        # COMMENT BLA BLA BLA
        # COMMENT BLE BLE BLE
        print "  <li>$message</li>\n";
    }

    print "</ul></li>\n";
  }

  return 0;
}

while (my $line = <>) {
  (my $line_crs = $line) =~ s/^(([Rr]evert\s*)?([A-Z]+-[0-9]+\s*,?\s*)+).*/$1/;
  $line_crs =~ s/\s*(,\s*)?$//;

  if ($line_crs ne $crs) {
    print_crs();

    $crs = $line_crs;
    $files_changed = 0;
    $lines_added = 0;
    $lines_removed = 0;
    @messages = ();
  }

  (my $message = $line) =~ s/^(^[Rr]evert\s*)?([A-Z]+-[0-9]+\s*,?\s*)+\s*(:|-)?(.*)/$4/;
  (my $files_changed_aux = $message) =~ s/.*\(files\s([0-9]+),\slines\s\+([0-9]+)\s-([0-9]+).*\)/$1/;
  (my $lines_added_aux = $message) =~ s/.*\(files\s([0-9]+),\slines\s\+([0-9]+)\s-([0-9]+)\).*/$2/;
  (my $lines_removed_aux = $message) =~ s/.*\(files\s([0-9]+),\slines\s\+([0-9]+)\s-([0-9]+).*\)/$3/;
  $files_changed += $files_changed_aux;
  $lines_added += $lines_added_aux;
  $lines_removed += $lines_removed_aux;

  $message =~ s/&/&amp;/g;
  $message =~ s/</&lt;/g;
  $message =~ s/>/&gt;/g;
  push(@messages, $message);
}

print_crs();

print "</ul>";
