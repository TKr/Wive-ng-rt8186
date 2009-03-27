#!/usr/bin/perl
#
# linux.bin & root.bin for DLink G700AP --> linux.bin for Edimax 6204WG 
#
# Usage: mk6204.pl linux.bin root.bin
# (c) jurabaz
#
# Some changes for rev.B by N.Leiten

$LINU= shift || "../images/linux.bin";
$ROOT= shift || "../images/root.bin";

!( -e $ROOT ) && die "root not exist";
!( -e $LINU ) && die "kernel not exist";

open FR, '<', $LINU || die 0;
binmode FR;
seek (FR, 4, 0);
read FR, $x, 4;
$y=unpack 'N', $x;
$START=sprintf("0x%08x", $y);
print "start=$START\n";
close FR;

$ls=uncvimg($LINU, 'linu');
$rs=uncvimg($ROOT, 'root');

$sum=sum16("root");

$ls=$ls+12+20;
$ns=$ls & (hex fff0000);
$ns=$ns+(hex 10000) if ($ns < $ls);

printf "ls=%x ns=%x\n", $ls, $ns;

for ($z="", $i=0; $i<($ns-$ls)+4; $i++) { $z.= chr(255); }
open TO, '>>linu' || die;
print TO $z;
print TO "ROOT";
print TO pack "N", $rs;
print TO pack 'N', $sum;
close TO;
`cat root >> linu`;
`./cvimg linux linu wive.bin $START 10000`;
`rm -f linu root`;

sub uncvimg {
  my ($fs, $x);
  open FR, '<', $_[0] || die 0;
  open TO, '>', $_[1] || die 0;
  binmode FR;
  binmode TO;
  seek(FR,0,2);
  $fs=tell(FR)-16-2;
  seek(FR,16,0);
  read FR, $x, $fs;
  print TO $x;
  close FR;
  close TO;
  return $fs
}

sub sum16 {
  my ($sum, $fs, $j, $i, $x);
  open FR, $_[0] || return 0;
  binmode FR;
  if (seek(FR, 0, 2)) {
    $fs=tell(FR)
  }
  seek(FR,0,0);

  $j=int($fs/2)*2;
  for($sum=0, $i=0; $i<$j; $i+=2) {
    read FR, $x, 2;	# , $i;
    $n=vec($x, 0, 16);
    $sum=($sum+$n) & 0xffff;
  }
  $sum = (~$sum +1) & 0xffff;
  printf "%04x\n", $sum;
  close FR;
  return $sum;
}
