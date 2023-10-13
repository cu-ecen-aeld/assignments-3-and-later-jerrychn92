#include <stdio.h>
#include <syslog.h>

int uc_file_exists(const char* pc_file_name)
{
   FILE *file;
   //Case where file exists
   if ((file = fopen(pc_file_name, "w+")))
   {
	fclose(file);
	return 1;
   }
   else
   {
	return 0;
   }
}

void write_to_file(const char* pc_file_path_, const char* pc_string_)
{
   // write string arg to the text file
   syslog(LOG_DEBUG, "writing string: %s to file: %s.\n", pc_string_, pc_file_path_);

   FILE *file;
   //Due to previous check, we assume the file exists
   file = fopen(pc_file_path_, "w");
   fprintf(file, "%s\n", pc_string_);
   fclose(file);

   return;
}

void setup_syslog()
{
   setlogmask(LOG_UPTO (LOG_DEBUG));
   openlog("assignment2", LOG_CONS|LOG_PID|LOG_NDELAY, LOG_USER);
}

void close_syslog()
{
   closelog();
}

int main(int argc, char *argv[]) {
   setup_syslog();
   //Check number of arguments are exactly 3. 1: c file, 2: file path, 3: string to write
   if (argc != 3)
   {
	syslog(LOG_ERR, "Number of arguments does not match expectation.");
	return 1;
   }

   // Check whether the file exists
   if (!uc_file_exists(argv[1]))
   {
	syslog(LOG_ERR, "File path provided does not exist.");
	return 1;
   }

   char *pc_file_name = argv[1];
   const char* pc_str = argv[2];

   write_to_file(pc_file_name, pc_str); 

   closelog();

   return 0;
}
