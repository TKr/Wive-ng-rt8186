#!/usr/bin/perl
# rtl8186 wive-ng patch from revB to revA
# qwerty 05.04.2009 v1.1

# setup
my $fread= "wive.bin";
my $fwrite="wive-rev-a.bin";

my ($fs, $x, $a);
open FR, '<', $fread or die "cannot open $fread : $!";
binmode FR;
# test for header 6 bytes
seek(FR, 0, 0);
read(FR, $x, 6);
$x=unpack('H*', "$x");
if ($x != "435359538030") { die "it seems $fread is not a valid rtl8186 image..." }
# test for header 16-20 bytes are valid
seek(FR, 16, 0);
read(FR, $x, 4);
$x=unpack('H*', "$x");
if ($x != "00008021") { $a="1"; }
# 
open FW, '>', $fwrite or die "cannot write $fwrite : $!";
binmode FW;
# - begin sadomazo
seek(FR, 0, 2);					# seek to end
$fs=tell(FR);					# get filesize to $fs
seek(FR, 0, 0);					# seek to begin
read(FR, $x, 12);				# read 12 bytes
print FW $x					# write to file
	or die "write to $fwrite failed : $!";
print FW "zzzz"					# write magic 4 bytes
	or die "write to $fwrite failed : $!";

seek(FR, 12, 0);				# seek to 12 byte
read(FR, $x, 4);				# read 4 bytes checksum
# if header 16-20 are null write without any changes 
if ($a == "1") {
	print "its seems $fread is black_fw...\n";
	print FW $x				# write checksum as is
	or die "write to $fwrite failed : $!";	
	seek(FR, 16, 0);			# seek to 16 byte
	read(FR, $x, $fs-16);			# read to end
	print FW $x				# write to file
	or die "write to $fwrite failed : $!";
	close FR or die "error closing $fread : $!";
	close FW or die "error closing $fwrite : $!";
	print "all done.\n";
	exit(0);				# nothing to do, exit
}
$x=unpack('N', "$x")-4;				# calculate checksum
print FW pack('N', "$x")			# write new checksum
	or die "write to $fwrite failed : $!";
seek(FR, 16, 0);				# seek to 16 byte
read(FR, $x, $fs);				# read to end
while($x =~ m/ROOT/g) {				# find start of rootfs
	$a=pos($x);				# get rootfs offset from 16 byte
}
seek(FR, 16, 0);				# seek again to 16 byte
read(FR, $x, $a-8);				# read to end of kernel without final 4 bytes (its only 0xff)
print FW $x					# write to file
	or die "write to $fwrite failed : $!";
seek(FR, $a+12, 0);				# seek from begin to rootfs (16-4)
read(FR, $x, $fs-($a+16-4)-1);			# read all rootfs ($a+16-4) bytes and -1 byte for checksum
print FW $x					# write to file
	or die "write to $fwrite failed : $!";
seek(FR, -1, 2);				# seek to last byte
read(FR, $x, 1);				# read it
$x=unpack('C', "$x")-2;				# calcualte checksum
print FW pack('c', "$x")			# write new checksum
	or die "write to $fwrite failed : $!";
# - end sadomazo
close FR or die "error closing $fread : $!";
close FW or die "error closing $fwrite : $!";
print "all done.\n";
