/* This should load and parse ~/.cicbrc */
#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "color.h"
extern FILE *logfp;


typedef struct	{
			char *	name;
			char *	field1;
			char *	field2;
			char *	field3;
			}	colorspec;



char	cicbrc[16]=".cicbrc";
char	cicbrcpath[1024];
char		cgenbuff[1024];
unsigned char	*cicbrcbuff,*workc;
char *line;

char * colornames[] = { "black", "red", "green", "yellow", "blue", "magenta", "cyan", "white" };

char * hicode = "hi";

char * fieldnames[] = { "sane", "warn", "error", "mod", "name", "addr", 
			"idle", "login", "unreg", "whead", "wsub", "pers", "persfrom",
			"normal", "gtlt", "bracket", "advise", "packet",
			"beep", "pass", "more", "status", "START", "END" };

long	filesize;

char	csane[COLORSIZE] = "\0";
char	cwarning[COLORSIZE] = "\0";
char	cerror[COLORSIZE] = "\0";
char	cmodstar[COLORSIZE] = "\0";
char	cusername[COLORSIZE] = "\0";
char	caddress[COLORSIZE] = "\0";
char	cidletime[COLORSIZE] = "\0";
char	clogintime[COLORSIZE] = "\0";
char	cunreg[COLORSIZE] = "\0";
char	cwheader[COLORSIZE] = "\0";
char	cwsubheader[COLORSIZE] = "\0";
char	cwhisper[COLORSIZE] = "\0";
char cpersfrom[COLORSIZE] = "\0";
char	cnormal[COLORSIZE] = "\0";
char	cgtlt[COLORSIZE] = "\0";
char	cequalbracket[COLORSIZE] = "\0";
char	cgenmessage[COLORSIZE] = "\0";
char	cpackettype[COLORSIZE] = "\0";
char	cbeep[COLORSIZE] = "\0";
char	cpass[COLORSIZE] = "\0";
char	cmore[COLORSIZE] = "\0";
char	cstatus[COLORSIZE] = "\0";

char colormap[COLORSIZE][23];


void clearcolors()
{
        int     i;

        for (i=0; i<22; i++)
                sprintf( colormap[COLORSIZE*i], "%c[0m" );

        return;
}


void setcolors()
{
	int	i = 0;

	sprintf( csane, "%s", colormap[COLORSIZE * i++] );
	sprintf( cwarning, "%s", colormap[COLORSIZE * i++] );
	sprintf( cerror, "%s", colormap[COLORSIZE * i++] );
	sprintf( cmodstar, "%s", colormap[COLORSIZE * i++] );
	sprintf( cusername, "%s", colormap[COLORSIZE * i++] );
	sprintf( caddress, "%s", colormap[COLORSIZE * i++] );
	sprintf( cidletime, "%s", colormap[COLORSIZE * i++] );
	sprintf( clogintime, "%s", colormap[COLORSIZE * i++] );
	sprintf( cunreg, "%s", colormap[COLORSIZE * i++] );
	sprintf( cwheader, "%s", colormap[COLORSIZE * i++] );
	sprintf( cwsubheader, "%s", colormap[COLORSIZE * i++] );
	sprintf( cwhisper, "%s", colormap[COLORSIZE * i++] );
	sprintf( cpersfrom, "%s", colormap[COLORSIZE * i++] );
	sprintf( cnormal, "%s", colormap[COLORSIZE * i++] );
	sprintf( cgtlt, "%s", colormap[COLORSIZE * i++] );
	sprintf( cequalbracket, "%s", colormap[COLORSIZE * i++] );
	sprintf( cgenmessage, "%s", colormap[COLORSIZE * i++] );
	sprintf( cpackettype, "%s", colormap[COLORSIZE * i++] );
	sprintf( cbeep, "%s", colormap[COLORSIZE * i++] );
	sprintf( cpass, "%s", colormap[COLORSIZE * i++] );
	sprintf( cmore, "%s", colormap[COLORSIZE * i++] );
	sprintf( cstatus, "%s", colormap[COLORSIZE * i++] );
	
	return;
}


void nocolors()
{
	int	i = 0;

	sprintf( csane, "%c", '\0' );
	sprintf( cwarning, "%c", '\0' );
	sprintf( cerror, "%c", '\0' );
	sprintf( cmodstar, "%c", '\0' );
	sprintf( cusername, "%c", '\0' );
	sprintf( caddress, "%c", '\0' );
	sprintf( cidletime, "%c", '\0' );
	sprintf( clogintime, "%c", '\0' );
	sprintf( cunreg, "%c", '\0' );
	sprintf( cwheader, "%c", '\0' );
	sprintf( cwsubheader, "%c", '\0' );
	sprintf( cwhisper, "%c", '\0' );
	sprintf( cpersfrom, "%c", '\0' );
	sprintf( cnormal, "%c", '\0' );
	sprintf( cgtlt, "%c", '\0' );
	sprintf( cequalbracket, "%c", '\0' );
	sprintf( cgenmessage, "%c", '\0' );
	sprintf( cpackettype, "%c", '\0' );
	sprintf( cbeep, "%c", '\0' );
	sprintf( cpass, "%c", '\0' );
	sprintf( cmore, "%c", '\0' );
	sprintf( cstatus, "%c", '\0' );

	return;
}


char * getcolorline( char **buffer )
{
	char *line;


	if ( filesize > 1)
	{
		line = *buffer;
		line++;
		while((**buffer != '@') && (filesize > 1))
		{
			if (**buffer == 0x09)
				**buffer = 0x20;
			(*buffer)++;
			filesize--;
		}
		**buffer = 0;
	}
	else
		line = NULL;

	return( line );
}


int parsecolorline( char * line, colorspec * cspec )
{
	char c;
	int fields = 0;

	cspec->name = line;
	cspec->field1 = cspec->field2 = cspec->field3 = NULL;


	c = *line;
	while((c != 0) && (c != '#'))
	{
		while((c != '#') && (c != 0x20) && (c != 0))
		{
			line++;
			c = *line;
		}
		switch (c)
		{
			case 0:
				break;
			case '#':
				fields++;
				break;

			case 0x20:
			{
				while(c == 0x20)
				{
					*line = 0;
					line++;
					c = *line;
				}
				if ((c == '#') || ( c == 0))
					fields++;
				else
				{
					switch (fields)
					{
						case 0:
							fields++;
							cspec->field1 = line;
							break;
						case 1:
							fields++;
							cspec->field2 = line;
							break;
						case 2:
							fields++;
							cspec->field3 = line;
							break;
						case 3:
							fields++;
							break;
						default:
							break;
					}
				}
				break;
			}

			default:
				line++;
				c = *line;
				break;
		}
	}

	return( fields );
}


void colorinit()
{
	FILE			*fp,*fopen();
	colorspec		TheColor;
	int			i, j, fields, fgcolor, bgcolor, hiflag;
	char			colorbuf[COLORSIZE];
	struct stat statbuf;


	sprintf(cicbrcpath,"%s/%s",getenv("HOME"),cicbrc);
	if (!( fp=fopen(cicbrcpath,"r"))) {
		sprintf(cicbrcpath,"%s",ICBSYSTEMCICBRC);
		if (!( fp=fopen(cicbrcpath,"r"))) {
			printf("Could not find .cicbrc.  No color used.\n");
			return;
		} 
	}
		fstat(fileno(fp),&statbuf);
		filesize=statbuf.st_size;
		printf("file size is %i\n",filesize);
		cicbrcbuff=(char *) malloc(filesize*2);
		workc=cicbrcbuff;
		fread(cicbrcbuff,filesize,1,fp);
		fclose(fp);

		clearcolors();
		printf( "Loading colors: \n"); 

		line = getcolorline( (char**)&workc);

/* discard first string, it's the header line */

		while( line != NULL ) {
			line = getcolorline( (char**)&workc );
			if (line != NULL) {
				fields = parsecolorline( line, &TheColor );

				for ( i=0; i<23; i++)
					if (!strcmp( TheColor.name, fieldnames[i] )) {
						break;
					}


					/* start line */
				if( i == 22)
					break;


				if( i != 22) {
					fgcolor = bgcolor = hiflag = 0;

					for (j=0; j<8; j++) {
						if (fields > 1)
							if (!strcmp( TheColor.field1, colornames[j] ))
								fgcolor = j+30;

						if (fields > 2)
							if (!strcmp( TheColor.field2, colornames[j] ))
								bgcolor = j+40;
					}

					if (fields > 2)
						if (!strcmp( TheColor.field2, hicode ))
							hiflag = 1;

					if ( fields > 3)
						if (!strcmp( TheColor.field3, hicode ))
							hiflag = 1;

						sprintf(colormap[COLORSIZE * i], "%c[%d;%d;%dm", 27, bgcolor, hiflag, fgcolor ); 
	
						printf( "%8s%s%c[0m", colormap[COLORSIZE * i],"G!",27 ); 
				}
			}
		}
		printf("\n");
		setcolors ();
		fflush(stdout);
		free(cicbrcbuff);
	}
