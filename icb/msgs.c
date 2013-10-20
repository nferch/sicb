/* Copyright (c) 1991 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* handle various messages from the server */

#include <config.h>
#include <stdio.h>
#include "icb.h"
#include "externs.h"
#include "../version.h"
#include <time.h>
#include <openssl/evp.h>
#include "icbcrypt.h"
#include <string.h>

/* later, these routines should buffer up text to be sent to the user and */
/* only send them when the user isn't typing */
/* perhaps we should stat the tty and only send when it's been idle for */
/* a second and there is no FIONREAD */
/* DONT stat if there is nothing to send to the tty */

/* open message */

copenmsg(char *pkt) {
	if (split(pkt) != 2) {
		sprintf(cgenbuff,"%s[=%sError%s=]%s got bad open message packet%s", cequalbracket, cerror, cequalbracket, cgenmessage, csane);
		putl(cgenbuff, PL_ALL);
		return;
	}
	if (ishushed(fields[0]))
		return;

	if (!strcmp(gv.alert,"all"))
	{
		putchar('\007');
		fflush(stdout);
	}

	mbreakprint(0, fields[0], fields[1]);
}

/* personal message */

cpersonalmsg(pkt)
char *pkt;
{
	char string[256];
	char host[256];
	char * decryptedmessage;
	int decryptedmessagelen;

	if (split(pkt) != 2) {
		sprintf(cgenbuff,"%s[=%sError%s=]%s got bad personal message packet%s", cequalbracket, cerror, cequalbracket, cgenmessage, csane);
		putl(cgenbuff, PL_ALL);
		return;
	}

	if (ishushed(fields[0]))
		return;

	if (gv.tabreply)
		histput(fields[0]);

	if (strcmp(gv.alert,"none"))
	{
		putchar('\007');
		fflush(stdout);
	}

	if (!strncmp(fields[1],"{!BF:",5)) {
		fields[1][strlen(fields[1]) - 1] = '\0';
		decryptedmessage = blowfishDecrypt(fields[0],fields[1]+5,&decryptedmessagelen);
		if (decryptedmessage == NULL) {
	  		sprintf(mbuf, "%s[%sBF%s]%s Could not decrypt from %s%s%s. Have you completed a key agreement?%s",cequalbracket,cbeep,cequalbracket,cgenmessage,cusername, fields[0], cgenmessage,csane);
			putl(mbuf, PL_BUF | PL_SCR);
	  		sprintf(mbuf, "[BF] Could not decrypt from %s. Have you completed a key agreement?",fields[0]);
			putl(mbuf, PL_LOG);
		} else {
			strncpy(fields[1],decryptedmessage,decryptedmessagelen);
			fields[1][decryptedmessagelen] = '\0';
			mbreakprint(2,fields[0],fields[1]);
			free(decryptedmessage);
		}
	} else if (!strncmp(fields[1],"{!DH:PV:",8)) {
	  	sprintf(mbuf, "%s[%sDH%s]%s Received P Value from %s%s%s.%s",cequalbracket,cbeep,cequalbracket,cgenmessage,cusername, fields[0], cgenmessage,csane);
		putl(mbuf, PL_BUF | PL_SCR);
	  	sprintf(mbuf, "[DH] Received P Value from %s.",fields[0]);
		putl(mbuf, PL_LOG);
		if (strlen(fields[1]) > 9) {
			fields[1][strlen(fields[1]) - 1] = '\0';
			if (!dhSetPValue(fields[0],fields[1]+8)) {
				decryptedmessage = malloc(1024);
				sprintf(decryptedmessage,"{!DH:PK:%s}",dhMakeKeyPair(fields[0]),0);
				sendpersonal(fields[0],decryptedmessage,0);
				free(decryptedmessage);
			}
		} else {
	  		sprintf(mbuf, "%s[%sDH%s]%s Unsuccessful key agreement with %s%s%s.%s",cequalbracket,cbeep,cequalbracket,cgenmessage,cusername, fields[0], cgenmessage,csane);
			putl(mbuf, PL_BUF | PL_SCR);
	  		sprintf(mbuf, "[DH] Unsuccessful key agreement with %s.",fields[0]);
			putl(mbuf, PL_LOG);
		}
	} else if (!strncmp(fields[1],"{!DH:PK:",8)) {
	  	sprintf(mbuf, "%s[%sDH%s]%s Received Public Key from %s%s%s.%s",cequalbracket,cbeep,cequalbracket,cgenmessage,cusername, fields[0], cgenmessage,csane);
		putl(mbuf, PL_BUF | PL_SCR);
	  	sprintf(mbuf, "[DH] Received Public Key from %s.",fields[0]);
		putl(mbuf, PL_LOG);
		if (strlen(fields[1]) > 9) {
			fields[1][strlen(fields[1]) - 1] = '\0';
		
			if (!dhGenerateBFKey(fields[0],fields[1]+8)) {
	  			sprintf(mbuf, "%s[%sDH%s]%s Completed key agreement with %s%s%s.%s",cequalbracket,cbeep,cequalbracket,cgenmessage,cusername, fields[0], cgenmessage,csane);
				putl(mbuf, PL_BUF | PL_SCR);
	  			sprintf(mbuf, "[DH] Completed key agreement with %s.",fields[0]);
				putl(mbuf, PL_LOG);
			} else {
	  			sprintf(mbuf, "%s[%sDH%s]%s Unsuccessful key agreement with %s%s%s.%s",cequalbracket,cbeep,cequalbracket,cgenmessage,cusername, fields[0], cgenmessage,csane);
				putl(mbuf, PL_BUF | PL_SCR);
	  			sprintf(mbuf, "[DH] Unsuccessful key agreement with %s.",fields[0]);
				putl(mbuf, PL_LOG);
			}
		}
	} else {
		mbreakprint(1, fields[0], fields[1]);
	}
}

/* beep message */

int beep(char *pkt) {
	char nick[MAX_NICKLEN+1];

	if (split(pkt) != 1) {
		sprintf(cgenbuff,"%s[=%sError%s=]%s got bad beep message packet%s", cequalbracket, cerror, cequalbracket, cgenmessage, csane);
		putl(cgenbuff, PL_ALL);
		return;
	}

	if (sscanf(fields[0],"%s (%*s)",nick)!=1)
	{
		strncpy(nick,fields[0],MAX_NICKLEN);
		nick[MAX_NICKLEN]='\0';
	}

	if (ishushed(nick))
		return;
	if (gv.beeps)
/*		sprintf(mbuf,"%c%s*%sMoo. %s%s%s sent you an annoying beep.%s",
			'\007',cequalbracket,cbeep, cusername, fields[0], cgenmessage,csane);*/
	  sprintf(mbuf, "%c%s[%sMoo.%s]%s %s%s sent you an annoying beep.%s", '\007',cequalbracket,cbeep,cequalbracket,cusername, fields[0], cgenmessage,csane);
	else
		sprintf(mbuf,"%s*%sMoo. %s%s%s sent you an annoying beep.%s",
			cequalbracket,cbeep, cusername, fields[0], cgenmessage,csane);
	  /*sprintf(mbuf, "%s[=%sMoo.%s=]%s %s%s sent you an annoying beep.%s",cequalbracket, cbeep,cequalbracket,cusername,fields[0],cgenmessage,csane);*/
	putl(mbuf, PL_SCR | PL_BUF);

	/* logs and review buffers don't get audible beeps */
		sprintf(mbuf, "[=Beep.=] %s sent you an annoying beep.",fields[0]);
	putl(mbuf, (PL_LOG));
	if (gv.tabreply)
		histput(nick);

}

/* "please drop the connection" message */

exitmsg()
{
/*	icbexit(); */
}


/* protocol version message */

protomsg(pkt)
char *pkt;
{
	char *p;
	int proto_level;

	if (split(pkt) != 3) {
		sprintf(cgenbuff,"%s[=%sError%s=]%s  got bad proto message packet%s",cequalbracket,cerror,cequalbracket,cgenmessage,csane);
		putl(cgenbuff, PL_SCR | PL_BUF);
		sprintf(cgenbuff,"[=Error=]  got bad proto message packet");
		putl(cgenbuff, PL_LOG);

		return;
	}

	proto_level = atoi(fields[0]);
	if (PROTO_LEVEL != proto_level) {
	  fprintf(stderr,
	    "\r\nThis forum client does not know the same protocol as\r\n");
	  fprintf(stderr,
	    "the server. I know protocol level %d and the server",PROTO_LEVEL);
	  fprintf(stderr,
	    " is at\r\nlevel %d.\r\n", proto_level);
	  fprintf(stderr,
	    "I am proceeding, but you may get weird results.\r\n\n");
	}

	if (!whoflg) {
		sprintf(mbuf, "Connected to the %s ICB server (%s).",
		  fields[1], fields[2]);
		putl(mbuf, PL_ALL);
	}

	if (whoflg)
		/* we want a who */
		sendlogin(myloginid, mynick, mygroup, "w", mypass);
	else
	{
		/* we want to log in */
		sendlogin(myloginid, mynick, mygroup, "login", mypass);

	tclreadicbrc();
	/*colorinit();*/
	sprintf(mbuf,"Type %chelp for help.\n", gv.cmdchar);
	putl(mbuf,PL_SCR);
	}

	/* clobber the password */
	for (p = mypass; *p != '\0'; p++)
		*p = '\0';
}


/* server is telling us we are logged in */

loginokmsg(char *pkt) {
	connected = 1;
}

/* system status update message */

statusmsg(char *pkt) {
	if (split(pkt) != 2) {
		sprintf(cgenbuff,"%s[=%sError%s=]%s got bad status message packet%s",cequalbracket,cerror,cequalbracket,cgenmessage,csane);
		putl(cgenbuff, PL_SCR | PL_BUF);
		sprintf(cgenbuff,"[=Error=] got bad status message packet");
		putl(cgenbuff, PL_LOG);
		return;
	}

	if (!strcmp(fields[0], "Register") && gv.autoregister) {
		regnick((char *)0);
		return;
	}
		
	if (!strcmp(fields[0], "Pass")) {
		if (!strncmp(fields[1], "Password changed to", 19)) {
			sprintf(cgenbuff,"%s[=%sPass%s=]%s Password successfully changed%s",cequalbracket,cpass,cequalbracket,cgenmessage,csane);
			putl(cgenbuff, PL_SCR | PL_BUF);
			sprintf(cgenbuff,"[=Pass=] Password successfully changed");
			putl(cgenbuff, PL_LOG);
			return;
		} else if (!strncmp(fields[1], "Password set to", 15)) {	
			sprintf(cgenbuff,"%s[=%sPass%s=]%s Password set%s",cequalbracket,cpass,cequalbracket,cgenmessage,csane);
			putl(cgenbuff, PL_ALL);
			sprintf(cgenbuff,"[=Pass=] Password set");
			putl(cgenbuff, PL_LOG);
		return;
		}
	}
		
	if (!strcmp(fields[0], "RSVP")) {
		char *user;
		if (strncmp(fields[1],"You are",7)==0){ 	/* Unix server */ 	
			int i;
			user = fields[1];
			for (i=0; i<7; ++i) {
				while (*user++ != ' ');
			}
		} else	/* VMS server */
			user = getword(fields[1]);

		if (ishushed(user))
			return;

		if (gv.tabreply)
			histput(user);


	}

	if (!strcmp(gv.alert,"all")) {
		putchar('\007');
		fflush(stdout);
	}
	if (!strcmp(fields[0], "Notify-On")) {
		sprintf(mbuf, "%c%c%c%c%s**%sWake up dipwick%s**%s %s%s%c%c%c",'\007','\007','\007','\007',cequalbracket,cstatus,cequalbracket, cgenmessage, fields[1],csane,'\007','\007','\007');
		putl(mbuf, PL_SCR | PL_BUF);
		sprintf(mbuf, "**Wake up ** %s",fields[1]);
		putl(mbuf, PL_LOG);
		histput(getword(fields[1]));
	}
	if ((!strcmp(fields[0], "Drop"))|| (!strcmp(fields[0], "Beep"))) {
		sprintf(mbuf, "%s[=%sAnti-Drop Mechanism%s=]%s %s%s", cequalbracket,cstatus,cequalbracket,cwarning,fields[0],csane);
		putl(mbuf, PL_SCR | PL_BUF);
		sprintf(mbuf, "[=Anti-Drop Mechanism =] %s",fields[0]);
		putl(mbuf, PL_LOG);
		sing(gv.idle);
	}

	if (!strcmp(fields[0], "Status")) {
		if (!strncmp(fields[1],"You are now in group",20)) {
			char *groupname=(char*) malloc(strlen(fields[1]));
			char *wspace;
			strcpy(groupname,fields[1]+21);
			wspace=strchr(groupname,' ');
			if (wspace !=NULL)
				*wspace='\0';
			if (!strcasecmp(groupname,"~IDLE~")) {
				sendcmd("g",mygroup);
			} else {
				if (!mygroup) 
					free(mygroup);
				mygroup=(char*) malloc(strlen(groupname) + 1);
				strcpy(mygroup,groupname);
			}
			free(groupname);
		}
	}
	sprintf(mbuf, "%s[=%s%s%s=]%s %s%s",cequalbracket,cstatus,fields[0],cequalbracket,cgenmessage,fields[1],csane);
	putl(mbuf, PL_SCR | PL_BUF);
	sprintf(mbuf, "[=%s=] %s",fields[0],fields[1]);
	putl(mbuf, PL_LOG);
}

/* command output message */
/* this should eventually parse things according to command requested */

cmdoutmsg(char *pkt) {
	char *idlestr(), *response(), *ampm();

	/* THIS IS REALLY BAD CODING - it should be cleaned up */
	split(pkt);

	if (strcmp(fields[0], "wl") == 0) {

		/* body of a who listing */
		sprintf(mbuf, "  %s%c%s%-12s %s%s   %s%s  %s%s@%s%s  %s%s",
			cmodstar,*fields[1] == 'm' ? '*' : ' ',
			cusername,fields[2],
			cidletime,idlestr(fields[3]),
			clogintime,ampm((time_t)atol(fields[5]), 0),
			caddress,fields[6], fields[7],
			cunreg,fields[8],
			csane);
		putl(mbuf, PL_SCR);
		sprintf(mbuf, "  %c%-12s %s   %s  %s@%s  %s",
			*fields[1] == 'm' ? '*' : ' ',
			fields[2],
			idlestr(fields[3]),
			ampm((time_t)atol(fields[5]), 0),
			fields[6], fields[7],
			fields[8]);
		putl(mbuf, PL_LOG);
	} else if (strcmp(fields[0], "gh") == 0) {
		if (m_groupheader)
			/* header for a grouplisting */
			sprintf(cgenbuff,"%sGroup     ## S  Moderator    Topic%s",cwheader,csane);
			putl(cgenbuff, PL_SCR);
			sprintf(cgenbuff,"Group     ## S  Moderator    Topic");
			putl(cgenbuff, PL_LOG);

	} else if (strcmp(fields[0], "wh") == 0) {
		if (m_whoheader)
	     /* header for a who listing */
		sprintf(cgenbuff,"%s   Nickname        Idle  Sign-On  Account%s",cwsubheader,csane);
	     putl(cgenbuff, PL_SCR);
		sprintf(cgenbuff,"   Nickname        Idle  Sign-On  Account");
	     putl(cgenbuff, PL_LOG);

	} else if (strcmp(fields[0], "wg") == 0) {

		/* group name from a who command */
/*		if (m_whoheader) { */
			if (gv.groupblanks)
			{
				putl("", PL_SL);
			}
			if (*fields[2] == '\0') { 
				sprintf(mbuf, "%sGroup:%s%s",cwheader, fields[1],csane);
				putl(mbuf, PL_SCR);
				sprintf(mbuf, "Group:%s",fields[1]);
				putl(mbuf, PL_LOG);
			} else {
				sprintf(mbuf2, "Group:", fields[1]);
				sprintf(mbuf, "%s%-11s  %s%s",cwheader,mbuf2,fields[2],csane);
				putl(mbuf, PL_SCR);
				sprintf(mbuf, "%-11s  %s",mbuf2,fields[2]);
				putl(mbuf, PL_LOG);
			}
/*		} else {
			if (*fields[2] == '\0')
				putl(fields[1]);
			else {
				sprintf(mbuf, "%s  \"%s\"", fields[1], fields[2]);
				putl(mbuf, PL_SCR | PL_LOG);
			}
		}
*/
	} else if (strcmp(fields[0], "ch") == 0) {
/*		listcommands();*/
	} else if (strcmp(fields[0], "c") == 0) {
		sprintf(mbuf, "%c%s", gv.cmdchar, fields[1]);
		putl(mbuf, PL_SCR | PL_LOG);
	} else
		/* just some generic command output */
		putl(fields[1], PL_SCR | PL_LOG);
}

/* error message */

int errormsg(char *pkt) {
	char nick[MAX_NICKLEN+1];
	int index;

	sprintf(mbuf, "%s[=%sError%s=]%s %s%s",cequalbracket,cerror,cequalbracket,cgenmessage, pkt,csane);
	putl(mbuf, PL_BUF | PL_SCR);
	sprintf(mbuf, "[=Error=] %s",pkt);
	putl(mbuf, PL_LOG);

	for 	(index = 0;
		pkt[index] && (!isspace(pkt[index])) && (index < MAX_NICKLEN);
	 	++index) {
		nick[index]=pkt[index];
	}
	nick[index]='\0';
		
	if (!strncmp(pkt+index, " not logged on", 14) ||
	    !strncmp(pkt+index, " is not logged on", 17) ||
	    !strncmp(pkt+index, " not signed on", 14))
	{
		if (gv.autodel)
			histdel(nick);
	}
}


/* important system message */

int importantmsg(char *pkt) {
	if (split(pkt) != 2) {
		sprintf(cgenbuff,"%s[=%sError%s=]%s  got bad \"important\" message packet%s",cequalbracket,cerror,cequalbracket,cgenmessage,csane);
		putl(cgenbuff, PL_BUF | PL_SCR);
		sprintf(cgenbuff,"[=Error=]  got bad \"important\" message packet");
		putl(cgenbuff, PL_LOG);
		return;
	}
	sprintf(mbuf, "\007%s[=%s%s%s=]%s %s%s",cequalbracket,cstatus,fields[0],cequalbracket,cgenmessage,fields[1],csane);
	putl(mbuf, PL_SCR | PL_BUF);
	sprintf(mbuf, "[=%s=] %s",fields[0],fields[1]);
	putl(mbuf, PL_LOG);
}

/* THIS STUFF NEEDS TO BE MOVED! */

/* return a string indicating idle time */

char *idlestr(char *num) {
	int seconds = atoi(num);
	static char idletime[8];
	int hours, minutes;

	if (seconds < 60)
		sprintf(idletime, "%6s", "-");
	else if (seconds < 3600)
		sprintf(idletime, "%5dm", seconds / 60,cidletime);
	else
	{
		hours = seconds/3600;
		minutes = (seconds - hours * 3600)/60;
		sprintf(idletime, "%2dh%2dm", hours, minutes);
	}
	return idletime;
}


/* return a string indicating response time */

char *response(char *num) {
	int secs = atoi(num);
	static char rtime[8];

	if (secs >= 2) {
		sprintf(rtime, " %2ds", secs);
		return(rtime);
	} else
		return("   -");
}

/* return a time, i.e "7:45pm" */


char *ampm(time_t secs, int style) {
	struct tm *t, *localtime();
	static char timestr[8];
	int hr, mn;

	t = localtime(&secs);
	hr = t->tm_hour;
	mn = t->tm_min;

	if (style) {
		sprintf(timestr, "%02d:%02d", hr, mn);
		return(timestr);
	}

	if (hr >= 12)
		sprintf(timestr, "%2d:%02dpm", (hr>12)?hr-12:hr,mn);
	else if (hr > 0)
		sprintf(timestr, "%2d:%02dam", hr, mn);
	else 
		sprintf(timestr, "12:%02dam", mn);

	return(timestr);
}

