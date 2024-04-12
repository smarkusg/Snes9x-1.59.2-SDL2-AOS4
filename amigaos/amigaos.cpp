//markus

#define __USE_INLINE__
#include <proto/dos.h>
#include <proto/icon.h>
#include <workbench/startup.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char* SDL_FULL = NULL;
char* FULLSCREEN_START = NULL;

static int AmigaOS_argc = 0;
static char **AmigaOS_argv = NULL;

static void Free_Arg(void)
{
	if (AmigaOS_argv)
	{
		ULONG i;
		for (i=0; i < AmigaOS_argc; i++)
			if(AmigaOS_argv[i]) free(AmigaOS_argv[i]);

		free(AmigaOS_argv);
	}
}


void AmigaOS_ParseArg(int argc, char *argv[], int *new_argc, char ***new_argv)
{
	struct WBStartup *WBStartup = NULL;
	struct DiskObject *icon = NULL;


	// Ok is launch from cmd line - we dont need command line , clear
	if (argc)
	{
	*new_argc = 0;
	*new_argv = NULL;
		return;
	}

	*new_argc = argc;
	*new_argv = argv;

	WBStartup = (struct WBStartup *) argv;
	if (!WBStartup)
	{
		return ; // We never know !
	}

      struct WBArg *WBArg=&WBStartup->sm_ArgList[0];
      BPTR oldcd;

      if (*WBArg->wa_Name) 
      {
	oldcd=SetCurrentDir(WBArg->wa_Lock);
	if ((icon=GetDiskObjectNew(WBArg->wa_Name)))
         {

	    STRPTR str;
            if ((str=FindToolType(icon->do_ToolTypes, "SDL_FULL")))
		{		
		 free(SDL_FULL);
		 SDL_FULL = strdup(str);
		 
		}

            if ((str=FindToolType(icon->do_ToolTypes, "FULLSCREEN_START")))
		{		
		 free(FULLSCREEN_START);
		 FULLSCREEN_START = strdup(str);
		 
		}

	 }
      SetCurrentDir(oldcd);
      }

//

	if (WBStartup->sm_NumArgs > 1)
	{
		// The first arg is always the tool name (aka us)
		// Then if more than one arg, with have some file name
		ULONG i;

		// We will replace the original argc/argv by us
		AmigaOS_argc = WBStartup->sm_NumArgs;
		AmigaOS_argv = (char **)   malloc(AmigaOS_argc * sizeof(char *) );
		if (!AmigaOS_argv) goto fail;

		memset(AmigaOS_argv, 0x00, AmigaOS_argc * sizeof(char *) );

		for(i=0; i<AmigaOS_argc; i++)
		{
			AmigaOS_argv[i] = (char *) malloc(MAX_DOS_NAME + MAX_DOS_PATH + 1);
			if (!AmigaOS_argv[i])
			{
				goto fail;
			}
			NameFromLock(WBStartup->sm_ArgList[i].wa_Lock, AmigaOS_argv[i], MAX_DOS_PATH);
			AddPart(AmigaOS_argv[i], WBStartup->sm_ArgList[i].wa_Name, MAX_DOS_PATH+MAX_DOS_NAME);
		}

	*new_argc = AmigaOS_argc;
	*new_argv = AmigaOS_argv;

	return;
	    }

	    
fail:
	Free_Arg();
	*new_argc = AmigaOS_argc;
	*new_argv = AmigaOS_argv;


	
}
#undef __USE_INLINE__

