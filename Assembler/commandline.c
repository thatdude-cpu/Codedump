#include "poweroff.h"

void Usage(char *errstr,...)
{
  char str[10000],tmpstr[100];
  va_list param;

  Log("Usage start");
  va_start(param,errstr);
  vsprintf(tmpstr,errstr,param);
  va_end(param);

  sprintf(str,"Error: %s\n",tmpstr);
  strcat(str,
		"Usage: poweroff action [options]\n\
Actions: logoff, reboot, shutdown, poweroff, standby, hibernate, lock, wol, monitor_off, monitor_on or no_action\n\
Options:\n\
  -warn\t\t: Show a warning message before doing the action\n\
  -warntime s\t: Show the warning message for s seconds\n\
  -playsound\t: Play a sound when the warning is displayed\n\
  -soundfile \"f\"\t: Specifies the .WAV file that must be played\n\
  -msg \"m\"\t: Specifies the warning message\n\
  -force\t\t: Force applications to close\n\
  -wait s\t\t: Wait s seconds before checking the time\n\
  -remote m\t: Do the action on the remote machine m (name or ip)\n\
  -remschedule\t: Schedule on the remote machine\n\
  -use_nt\t\t: Use the NT protocol to shutdown/reboot a remote machine\n\
  -port p\t\t: Use port p to connect to the remote machine\n\
  -username u\t: User NT username u to connect to the remote machine\n\
  -password p\t: Use password p to connect to the remote machine\n\
  -run\t\t: Run a program before doing the action\n\
  -program \"p\"\t: Runs program p before doing the action\n\
  -directory \"d\"\t: Start the program in directory d\n\
  -notray\t\t: Do not show poweroff icon in the system tray\n\
  -nocancel\t: Do not allow the user to cancel the action from the tray or in the warning dialog\n\
  -immediate\t: Perform the action immediately (default)\n\
  -scheduled\t: Perform the action in a scheduled manner\n\
  -time hh:mm\t: Perform the action at the specified time\n\
  -date dd/mm/yyyy\t: Perform the action on the specified date\n\
  -days [d1,d2,...]\t: Perform the action on the spcified days of the week (m,t,w,th,s,su)\n\
  -weeks [1,..,5]\t: Perform the action on the spcified weeks (default=all weeks)\n\
  -day d\t\t: Perform the action on the specified day of the month (1-31)\n\
  -seconds n\t: Perform the action after n seconds\n\
  -process p\t: Perform the action when a process named p does not exist anymore\n\
  -ip ip\t\t: Specifies the ip adress for WOL\n\
  -subnet s\t: Specifies the subnet mask for WOL\n\
  -mac m\t\t: Specifies the mac address for WOL\n\
  -allow_remote\t: Allow remote control\n\
");
  strcat(str,
"  -remote_port p\t: Listen on port p for commands\n\
  -remote_pswd p\t: Set a password that clients must give for remote control\n\
  -simulate\t: Simulate all actions, do not perform them\n\
  -minimize\t: Start minimized in the system tray\n\
  -quiet\t\t: Don't display error messages\n\
  -save_settings\t: Saves current settings to the registry\n\
  -remove_settings\t: Remove the settings from the registry\n\
  -save_svc_set\t: Save current settings to the service\n\
  -remove_svc_set\t: Removes the service settings\n\
  -create_service\t: Creates the poweroff service, including the settings\n\
  -remove_service\t: Removes the poweroff service, including the settings\n\
  -debug\t\t: Start poweroff in debug mode\n\
");
  Error(NULL,NULL,str);
  Log("Usage end");
  exit(0);
}

void CreateNewArgv(char ***argv,int *argc,char *commandline,int start_pos,int end_pos)
{
  *argv=(char**)realloc(*argv,(*argc+1)*sizeof(char*));
  (*argv)[*argc]=malloc(end_pos-start_pos+1);
  memcpy((*argv)[*argc],&commandline[start_pos],end_pos-start_pos);
  (*argv)[*argc][end_pos-start_pos]=0;
  (*argc)++;
}

void ConvertCommandLine(char *commandline,int *argc,char ***argv)
{
  int lastpos=0,x;
  char quote=0;

  *argv=NULL;
  *argc=0;
  for (x=0;x<=(int)strlen(commandline);x++)
  {
    if (!quote && (commandline[x]==' ' || commandline[x]=='\t' || commandline[x]==0))
    {
      if (lastpos!=x)
        CreateNewArgv(argv,argc,commandline,lastpos,x);
      lastpos=x+1;
    }
    else if ((commandline[x]=='"' || commandline[x]=='\'') && (!quote || commandline[x]==quote))
    {
      if (!quote)
      {
        quote=commandline[x];
      }
      else
      {
        CreateNewArgv(argv,argc,commandline,lastpos,x);
        quote=0;
      }
      lastpos=x+1;
    }
  }
  *argv=(char**)realloc(*argv,(*argc+1)*sizeof(char*));
  (*argv)[*argc]=NULL;
}

int ParseCommandLine(char *commandline,PowerSettings *ps)
{
  int argc,x,args_found=0;
  char **argv;
  CommandLineAction action=NOTHING;

  Log("ParseCommandLine start");
  ConvertCommandLine(commandline,&argc,&argv);
  strcpy(ps->executable,argv[0]);
  for (x=1;x<argc;x++)
  {
    args_found++;
    if (argv[x][0]=='-')
		{
			if (!strcmp(argv[x],"-force"))
			{
				ps->options.force=1;
			}
			else if (!strcmp(argv[x],"-warn"))
			{
				ps->options.warning=1;
			}
			else if (!strcmp(argv[x],"-quiet"))
			{
				ps->quiet=1;
			}
			else if (!strcmp(argv[x],"-playsound"))
			{
				ps->warning.play_sound=1;
			}
      else if (!strcmp(argv[x],"-soundfile"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->warning.sound_file,argv[x]);
        }
        else
          Usage("Missing argument for -soundfile");
      }
			else if (!strcmp(argv[x],"-service"))
			{
				ps->run_as_service=1;
        args_found--;
			}
      else if (!strcmp(argv[x],"-warntime"))
      {
        if (x<argc-1)
        {
          x++;
          ps->warning.seconds=atoi(argv[x]);
        }
        else
          Usage("Missing argument for -warntime");
      }
      else if (!strcmp(argv[x],"-wait"))
      {
        if (x<argc-1)
        {
          x++;
          ps->schedule.wait=atoi(argv[x]);
        }
        else
          Usage("Missing argument for -wait");
      }
      else if (!strcmp(argv[x],"-msg"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->warning.message,argv[x]);
          ReplaceString(ps->warning.message,"\\n","\r\n");
          ReplaceString(ps->warning.message,"\\\r\n","\\n");
        }
        else
          Usage("Missing argument for -msg");
      }
      else if (!strcmp(argv[x],"-remote"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->remote.computer_name,argv[x]);
          ps->who=REMOTE_COMPUTER;
        }
        else
          Usage("Missing argument for -remote");
      }
			else if (!strcmp(argv[x],"-remschedule"))
			{
				ps->remote.schedule_remote=1;
			}
			else if (!strcmp(argv[x],"-use_nt"))
			{
				ps->remote.use_nt=1;
			}
      else if (!strcmp(argv[x],"-port"))
      {
        if (x<argc-1)
        {
          x++;
          ps->remote.port=atoi(argv[x]);
          if (ps->remote.port<=0)
            ps->remote.port=LISTEN_PORT;
        }
        else
          Usage("Missing argument for -port");
      }
      else if (!strcmp(argv[x],"-password"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->remote.password,argv[x]);
        }
        else
          Usage("Missing argument for -password");
      }
      else if (!strcmp(argv[x],"-username"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->remote.username,argv[x]);
          ps->remote.current_credentials=0;
        }
        else
          Usage("Missing argument for -username");
      }
			else if (!strcmp(argv[x],"-run"))
			{
				ps->options.run_program=1;
			}
      else if (!strcmp(argv[x],"-program"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->program.program,argv[x]);
        }
        else
          Usage("Missing argument for -program");
      }
      else if (!strcmp(argv[x],"-directory"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->program.directory,argv[x]);
        }
        else
          Usage("Missing argument for -directory");
      }
			else if (!strcmp(argv[x],"-notray"))
			{
				ps->options.in_tray=0;
			}
			else if (!strcmp(argv[x],"-nocancel"))
			{
				ps->options.allow_cancel=0;
			}
			else if (!strcmp(argv[x],"-immediate"))
			{
				ps->when=IMMEDIATE;
			}
			else if (!strcmp(argv[x],"-scheduled"))
			{
				ps->when=SCHEDULED;
			}
			else if (!strcmp(argv[x],"-allow_remote"))
			{
				ps->options.allow_remote_control=1;
			}
      else if (!strcmp(argv[x],"-remote_port"))
      {
        if (x<argc-1)
        {
          x++;
          ps->remote_control.port=atoi(argv[x]);
          if (ps->remote_control.port<=0)
            ps->remote_control.port=LISTEN_PORT;
        }
        else
          Usage("Missing argument for -remote_port");
      }
      else if (!strcmp(argv[x],"-remote_pswd"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->remote_control.password,EncodePassword(argv[x]));
        }
        else
          Usage("Missing argument for -remote_pswd");
      }
			else if (!strcmp(argv[x],"-process"))
			{
				ps->when=PROCESS;
        if (x<argc-1)
        {
          x++;
          strcpy(ps->process.process,argv[x]);
        }
        else
          Usage("Missing argument for -process");
			}
			else if (!strcmp(argv[x],"-simulate"))
			{
				ps->simulate=1;
        args_found--;
			}
			else if (!strcmp(argv[x],"-minimize"))
			{
				ps->start_minimized=1;
        args_found--;
			}
      else if (!strcmp(argv[x],"-time"))
      {
        if (x<argc-1)
        {
          int hours,minutes;

          x++;
          if (sscanf(argv[x],"%d:%d",&hours,&minutes)!=2)
            Usage("Invalid time format for -time (%s)",argv[x]);
          else
          {
            GetLocalTime(&ps->schedule.time);
            ps->schedule.time.wHour=hours;
            ps->schedule.time.wMinute=minutes;
            if (ps->schedule.date.wYear==0)
              GetLocalTime(&ps->schedule.date);
          }
        }
        else
          Usage("Missing argument for -time");
      }
      else if (!strcmp(argv[x],"-date"))
      {
        if (x<argc-1)
        {
          int days,months,years;

          x++;
          if (sscanf(argv[x],"%d/%d/%d",&days,&months,&years)!=3)
            Usage("Invalid format for -date (%s)",argv[x]);
          else
          {
            GetLocalTime(&ps->schedule.date);
            ps->schedule.date.wDay=days;
            ps->schedule.date.wMonth=months;
            ps->schedule.date.wYear=years;
            ps->schedule.schedule=FIXED_DAY;
          }
        }
        else
          Usage("Missing argument for -date");
      }
      else if (!strcmp(argv[x],"-days"))
      {
        if (x<argc-1)
        {
          char days[10][10];
          int n,y;

          x++;
          ps->schedule.monday=0;
          ps->schedule.tuesday=0;
          ps->schedule.wednesday=0;
          ps->schedule.thursday=0;
          ps->schedule.friday=0;
          ps->schedule.saturday=0;
          ps->schedule.sunday=0;
          n=sscanf(argv[x],"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",days[0],days[1],days[2],days[3],days[4],days[5],days[6]);
          for (y=0;y<n;y++)
          {
            if (!strcmp(days[y],"m"))
              ps->schedule.monday=1;
            else if (!strcmp(days[y],"t"))
              ps->schedule.tuesday=1;
            else if (!strcmp(days[y],"w"))
              ps->schedule.wednesday=1;
            else if (!strcmp(days[y],"th"))
              ps->schedule.thursday=1;
            else if (!strcmp(days[y],"f"))
              ps->schedule.friday=1;
            else if (!strcmp(days[y],"s"))
              ps->schedule.saturday=1;
            else if (!strcmp(days[y],"su"))
              ps->schedule.sunday=1;
            else
              Usage("Invalid day specified for -day (%s)",argv[x]);
            ps->schedule.schedule=DAILY;
          }
        }
        else
          Usage("Missing argument for -days");
      }
      else if (!strcmp(argv[x],"-weeks"))
      {
        if (x<argc-1)
        {
          char weeks[10][10];
          int n,y;

          x++;
          ps->schedule.week1=0;
          ps->schedule.week2=0;
          ps->schedule.week3=0;
          ps->schedule.week4=0;
          ps->schedule.week5=0;
          n=sscanf(argv[x],"%[^,],%[^,],%[^,],%[^,],%s",weeks[0],weeks[1],weeks[2],weeks[3],weeks[4]);
          for (y=0;y<n;y++)
          {
            if (!strcmp(weeks[y],"1"))
              ps->schedule.week1=1;
            else if (!strcmp(weeks[y],"2"))
              ps->schedule.week2=1;
            else if (!strcmp(weeks[y],"3"))
              ps->schedule.week3=1;
            else if (!strcmp(weeks[y],"4"))
              ps->schedule.week4=1;
            else if (!strcmp(weeks[y],"5"))
              ps->schedule.week5=1;
            else
              Usage("Invalid week number specified for -week (%s)",argv[x]);
            ps->schedule.schedule=DAILY;
          }
        }
        else
          Usage("Missing argument for -weeks");
      }
      else if (!strcmp(argv[x],"-day"))
      {
        if (x<argc-1)
        {
          x++;
          ps->schedule.day=atoi(argv[x]);
          ps->schedule.schedule=DAY_OF_MONTH;
        }
        else
          Usage("Missing argument for -day");
      }
      else if (!strcmp(argv[x],"-seconds"))
      {
        if (x<argc-1)
        {
          x++;
          ps->schedule.seconds=atoi(argv[x]);
          ps->schedule.schedule=AFTER_X_SECONDS;
        }
        else
          Usage("Missing argument for -seconds");
      }
      else if (!strcmp(argv[x],"-ip"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->remote.ip_address,argv[x]);
        }
        else
          Usage("Missing argument for -ip");
      }
      else if (!strcmp(argv[x],"-subnet"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->remote.subnet_mask,argv[x]);
        }
        else
          Usage("Missing argument for -subnet");
      }
      else if (!strcmp(argv[x],"-mac"))
      {
        if (x<argc-1)
        {
          x++;
          strcpy(ps->remote.mac_address,argv[x]);
        }
        else
          Usage("Missing argument for -mac");
      }
      else if (!strcmp(argv[x],"-save_settings"))
      {
        action=SAVE_SETTINGS;
      }
      else if (!strcmp(argv[x],"-remove_settings"))
      {
        action=REMOVE_SETTINGS;
      }
      else if (!strcmp(argv[x],"-save_svc_set"))
      {
        action=SAVE_SERVICE_SETTINGS;
      }
      else if (!strcmp(argv[x],"-remove_svc_set"))
      {
        action=REMOVE_SERVICE_SETTINGS;
      }
      else if (!strcmp(argv[x],"-create_service"))
      {
        action=CREATE_SERVICE;
      }
      else if (!strcmp(argv[x],"-remove_service"))
      {
        action=REMOVE_SERVICE;
      }
      else if (!strcmp(argv[x],"-debug"))
      {
        debug=1;
        args_found--;
      }
      else
        Usage("Invalid option specified (%s)",argv[x]);
    }
    else
    {
      if (!_stricmp(argv[x],"logoff"))
        ps->action=LOGOFF;
      else if (!_stricmp(argv[x],"reboot"))
        ps->action=REBOOT;
      else if (!_stricmp(argv[x],"shutdown"))
        ps->action=SHUTDOWN;
      else if (!_stricmp(argv[x],"poweroff"))
        ps->action=POWEROFF;
      else if (!_stricmp(argv[x],"standby"))
        ps->action=STANDBY;
      else if (!_stricmp(argv[x],"hibernate"))
        ps->action=HIBERNATE;
      else if (!_stricmp(argv[x],"lock"))
        ps->action=LOCK;
      else if (!_stricmp(argv[x],"monitor_off"))
        ps->action=MONITOR_OFF;
      else if (!_stricmp(argv[x],"monitor_on"))
        ps->action=MONITOR_ON;
      else if (!_stricmp(argv[x],"no_action"))
        ps->action=NO_ACTION;
      else if (!_stricmp(argv[x],"wol"))
      {
        ps->action=WAKE_ON_LAN;
        ps->who=REMOTE_COMPUTER;
      }
      else
        Usage("Invalid action specified (%s)",argv[x]);
      ps->interactive=0;
    }
  }
  switch (action)
  {
    case SAVE_SETTINGS:
      SaveSettings(HKEY_CURRENT_USER,"SOFTWARE\\Poweroff",ps);
      exit(0);
      break;
    case REMOVE_SETTINGS:
      if (!DeleteRegistryKey(HKEY_CURRENT_USER,"SOFTWARE\\Poweroff"))
      {
        DisplayLastError(ps,NULL);
      }
      exit(0);
      break;
    case SAVE_SERVICE_SETTINGS:
      SaveSettings(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\Poweroff\\Parameters",ps);
      if (ps->windowsversion.dwPlatformId!=VER_PLATFORM_WIN32_WINDOWS)
      {
        if (IsPoweroffServiceRunning(NULL,ps))
        {
          StopPoweroffService(NULL,ps);
          StartPoweroffService(NULL,ps);
        }
      }
      exit(0);
      break;
    case REMOVE_SERVICE_SETTINGS:
      if (!DeleteRegistryKey(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\Poweroff\\Parameters"))
      {
        DisplayLastError(ps,NULL);
      }
      exit(0);
      break;
    case CREATE_SERVICE:
      CreatePoweroffService(NULL,ps);
      exit(0);
      break;
    case REMOVE_SERVICE:
      if (IsPoweroffServiceRunning(NULL,ps))
        StopPoweroffService(NULL,ps);
      RemovePoweroffService(NULL,ps);
      exit(0);
      break;
  }
  if (args_found>0)
    return 1;
  Log("ParseCommandLine end");
  return 0;
}