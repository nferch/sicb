#!/usr/bin/perl -IIcbCrypt/blib/lib -IIcbCrypt/blib/arch

use IcbCrypt;

while (<STDIN>) {
	chomp;
	if (/^dh/) {
		($cmd,$nick) = split(' ',$_,2);
		$pvalue = IcbCrypt::dhGetPValue($nick,1);
		print "\nDH P Value for $nick is:\n$pvalue\n";
		$pubkey = IcbCrypt::dhMakeKeyPair($nick);
		print "Public Key for $nick is:\n$pubkey\n\n";
	} elsif (/^recvp /) {
		($cmd,$nick,$pvalue) = split(' ',$_,3);
		$rc = IcbCrypt::dhSetPValue($nick,$pvalue);
		print "P Value for $nick set. Return code was $rc.\n";
		$pubkey = IcbCrypt::dhMakeKeyPair($nick);
		print "Public Key for $nick is:\n$pubkey\n\n";
	} elsif (/^recvpk /) {
		($cmd,$nick,$hexpk) = split(' ',$_,3);
		$rc = IcbCrypt::dhGenerateBFKey($nick,$hexpk);
		print "Generated BF key for $nick. Return code was $rc.\n";
	} elsif (/^n /) {
		($cmd,$nick,$message) = split(' ',$_,3);
		print IcbCrypt::blowfishEncrypt($nick,$message),"\n\n";
	} elsif (/^recvn /) {
		($cmd,$nick,$message) = split(' ',$_,3);
		print IcbCrypt::blowfishDecrypt($nick,$message),"\n\n";
	} else {
		print "Nope, I don't get it.\n";
	}
}
