#! /usr/local/bin/perl

use Path::Tiny;

my $path = path("./hankaku.txt");
my ($xxx, @lines) = $path->lines;

my @dats;

foreach my $l (@lines){
    if($l =~ /^\s*$/){
        next;
    }elsif($l =~ /^char/){
        next;
    }

    my $d = 0x00;
    my @cs = split(//, $l);
    my $i = 7;
    foreach my $c (@cs){
        if($c eq "*"){
            $d |= 0x01 << $i;
        }
        $i--;
    }
    push(@dats, $d);
}

printf("char hankaku[4096] = {\n");

my $d = shift @dats;
printf(" 0x%02X ", $d);
my $cnt = 1;
foreach $d (@dats){
    if($cnt >= 15){
        printf(",0x%02X", $d);
        print "\n";
        $cnt = 0;
    }else{
        printf(",0x%02X ", $d);
        $cnt++;
    }
}
printf("};\n");
