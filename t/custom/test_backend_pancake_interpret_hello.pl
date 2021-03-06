#!/usr/bin/env perl
use strict;
use warnings;
use v5.10;

my $path = "./icarus";

die "Could not find '$path'\n" unless -e $path;

my $input = <<EOF;
fn foo(s::String)
  println(s)
end

fn main()
  foo("Hello world")
end
EOF

my $expected = <<EOF;
Hello world
EOF

my $in_tmp_file = `mktemp TESTING_BACKEND_PANCAKE_interpret_HELLO_XXX.ic`;
chomp($in_tmp_file);
open( my $fh, ">", "$in_tmp_file" ) or die "failed to open tmp file '$in_tmp_file'";
print $fh $input;
close $fh;

my $output = `$path pancake $in_tmp_file`;
my $exit_status = $?;

`rm $in_tmp_file`;

if( $exit_status != 0 ){
  say "Icarus 2c exit status was not as expected";
  say "=======\nGot:\n$output";
  die "exit_status was '$exit_status', expected 0";
}

if( $exit_status != 0 || $output ne $expected ){
    say "Output was not as expected";
    say "=======\nExpected:\n$expected";
    say "=======\nGot:\n$output";
    say "=======\n";
    if( $exit_status != 0 ){
        die "exit_status was '$exit_status', expected 0";
    } else {
        die "Output not as expected";
    }
}

say "test_backend_pancake_interpret_hello successs";
say "=======\nGot correct output:\n$output";

