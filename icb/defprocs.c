/*
	THIS IS AN AUTOMATICALLY GENERATED FILE

	If you edit this file, your changes may be lost later.
*/

char *defprocs[] = 
{
	"c_usage add ! g \"\\[command\\]\" \"shell out or run a unix command\"",
	"c_usage add m g \"\\[-permanent\\] \\[-clear\\] nick message\" \"send a personal message\"",
	"proc _quote {args} {\
		set length [ length $args chars ]\n\
		set string \"\"\n\
		for {set i 0 } { $i < $length } {set i [ expr $i+1 ]} {\n\
			set ch [ index $args $i chars ]\n\
			case $ch in \\\n\
			\\; {set string ${string}\\\\\\;} \\\n\
			\\\" {set string ${string}\\\\\\\"} \\\n\
			\\\\ {set string ${string}\\\\\\\\} \\\n\
			*   {set string ${string}${ch}}\n\
		}\n\
		return $string\n\
	}",
	"proc m {args} {\
		set args [ _quote $args ]\n\
		set first [ range $args 0 0 chars ]\n\
		if [ string compare $first \\{ ]==0 {\n\
			set length [ length $args chars ]\n\
			set args [ range $args 1 [ expr $length-2 ] chars ]\n\
		}\n\
		eval s_personal $args\n\
	}",
	"proc n {args} {\
		set args [ _quote $args ]\n\
		set first [ range $args 0 0 chars ]\n\
		if [ string compare $first \\{ ]==0 {\n\
			set length [ length $args chars ]\n\
			set args [range $args 1 [ expr $length-2 ] chars ]\n\
		}\n\
		eval s_spersonal $args\n\
	}",
	"oset pagesize -1",
	"c_usage add tcl g \"tcl command\" \"pass command to tcl without quoting\"",
	"c_usage add oset g \"\\[variable\\] \\[value\\]\" \"view or set an icb variable\"",
	"c_usage add ounset g \"\\[variable\\]\" \"unset an icb variable\"",
	"proc fset {args} {\
		if { [ length $args ] == 0 } then {\n\
			oset\n\
		} else { if { [ length $args ] == 1 } then {\n\
			oset $args\n\
		} else {\n\
			set var [ range $args 0 0 ]\n\
			set value [ range $args 1 1 ]\n\
			oset $var $value\n\
		}\n\
		}\n\
	}\n\
	proc funset {var} {ounset $var}",
	"c_usage add beep g \"nick\" \"send someone a beep\"",
	"proc beep nick {s_beep $nick}",
	"c_usage add boot m \"nick\" \"kick someone out of your group\"",
	"proc boot nick {s_group remove $nick}",
	"c_usage add bug g \"\" \"send mail to the author\"",
	"proc bug {} {c_shell \"mail mark@cad.gatech.edu\"}",
	"c_usage add cancel m \"nick\" \"cancel an invitation\"",
	"proc cancel nick {s_group cancel $nick}",
	"c_usage add commands i \"\" \"list commands\"",
	"proc commands {} {c_usage list}",
	"c_usage add bump nickname \"\\[password\\]\" \"bump someone who is using your registered nickname\"",
	"proc bump {args} {s_bump $args}",
	"c_usage add cp \"\\[old\\]\" \"\\[new\\]\" \"change your registration password\"",
	"proc cp {args} {s_chpw $args}",
	"proc dh {args} {s_dh $args}",
	"c_usage add echoback g \"on|off\" \"turn on/off echoback\"",
	"proc echoback onoff {s_user echoback $onoff}",
	"c_usage add group g \"group\" \"change groups\"",
	"proc group group {s_group change $group}",
	"proc g group {s_group change $group}",
	"c_usage add hclear g \"\" \"clear the \\\"tab\\\" history buffer\"",
	"proc hclear {} {c_hclear}",
	"c_usage add hdel g \"\" \"remove a nickname from the \\\"tab\\\" history buffer\"",
	"proc hdel {nick} {c_hdel $nick}",
	"c_usage add help i \"\\[subject\\]\" \"get help \\[on subject\\]\"",
	"proc help {args} {c_help $args}",
	"c_usage add hush g \"\\[nick\\]\" \"toggle hushing of a user\"",
	"proc hush {args} {c_hush $args}",
	"c_usage add invite m \"nick\" \"invite people to your group\"",
	"proc invite nick {s_group invite $nick}",
	"c_usage add log g \"\\[filename\\]\" \"toggle session logging\"",
	"proc log {args} {eval c_log $args}",
	"proc message {args} {eval s_personal $args}",
	"c_usage add motd i \"\" \"print the ICB message of the day\"",
	"proc motd {} {s_motd}",
	"c_usage add new m \"group\" \"create your own group\"",
	"proc new group {s_group create $group}",
	"c_usage add nick g \"newnick\" \"change your nickname\"",
	"proc nick newnick {s_user nick $newnick}",
	"c_usage add pass m \"\\[nick\\]\" \"pass moderation of your group\"",
	"proc pass {args} {s_group pass $args}",
	"c_usage add display g \"\\[-personal\\] \\[-nickname nick\\] \\[-(no)log\\] \\[-(no)screen\\] \\[lines\\]\" \"display past conversation\"",
	"proc display {args} {eval c_replay $args}",
	"proc replay {args} {eval c_replay $args}",
	"c_usage add quit g \"\" \"exit icb\"",
	"proc quit {} {c_quit}",
	"c_usage add register g \"\\[password\\]\" \"register your nickname\"",
	"proc register args {s_register $args}",
	"c_usage add send g \"message\" \"send text to your group\"",
	"proc send args {s_send $args}",
	"c_usage add status m \"\" \"change or see the status of a group\"",
	"proc status {args} {s_group status $args}",
	"c_usage add time i \"\" \"show the current time\"",
	"proc time {} {c_time}",
	"c_usage add topic m \"topicname\" \"set your group's topic\"",
	"proc topic args {s_group topic $args}",
	"c_usage add version i \"\\[-s|-c\\]\" \"display server and/or client version info\"",
	"proc version {args} {\
		if { [ string compare $args \"-c\" ] !=0 } {\n\
			s_version\n\
		}\n\
		if { [ string compare $args \"-s\" ] != 0 } {\n\
			c_version\n\
		}\n\
	}",
	"c_usage add who i \"\\[.\\] \\[group\\]\" \"list users\"",
	"proc who {args} {s_who long $args}",
	"proc a_su pass {s_user nick admin $pass}",
	"proc a_drop args {s_admin drop $args}",
	"proc a_shutdown {when args} {s_admin shutdown $when $args}",
	"proc a_wall args {s_admin wall $args}",
	(char *) 0
};

