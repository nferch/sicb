use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    'NAME'	=> 'IcbCrypt',
    'VERSION_FROM' => 'IcbCrypt.pm', # finds $VERSION
    'LIBS'	=> ['-lcrypto'],   # e.g., '-lm' 
    'DEFINE'	=> '',     # e.g., '-DHAVE_SOMETHING' 
    'INC'	=> '-I.. -I../icb',     # e.g., '-I/usr/include/other' 
    'OBJECT'	=> 'IcbCrypt.o ../icb/icbcrypt.o'
);
