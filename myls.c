#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>

int main(int argc, char *argv[]) {

        /*
	*First step: identify arguments. If none, simply open file contents 
	*/

	char current_dir[256];
	//ibm.com
	if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
		perror("getcwd() error");
	}
	else {
		//current_dir = current_dir;
	}

	//printf("FILE: %s\n", current_dir);



	if (argc == 1) {
		DIR *directory;
		struct dirent *dir_element;
       		if ((directory = opendir(current_dir)) == NULL) {
			perror("Could not open directory");
		}
		else {
			while ((dir_element = readdir(directory)) != NULL) {
				if (dir_element -> d_name[0] != '.') {
					printf("%s     /n", dir_element -> d_name);
				}
				
			}
		}
	}

	int opt;

	while ((opt = getopt(argc, argv, "la:")) != -1) {
		DIR *directory;
		struct dirent *dir_element;

		switch (opt) {
			case 'a':
				//DIR *directory;
				//struct dirent *dir_element;
       				if ((directory = opendir(current_dir)) == NULL) {
					perror("Could not open directory");
				}
				else {
					while ((dir_element = readdir(directory)) != NULL) {
						printf("%s	\n", dir_element -> d_name);
				
					}
				}

			case 'l': ;
				  
				//DIR *directory;
				//struct dirent *dir_element;
				struct passwd *username_access;
				struct passwd *group_access;
				struct tm *stattime;
				char timebuf[80];

				

				printf("\n");
				struct stat *buffer = malloc(sizeof(struct stat));
       				if ((directory = opendir(current_dir)) == NULL) {
					perror("Could not open directory");
				}
				else {
					

					while ((dir_element = readdir(directory)) != NULL) {
						char *filename = dir_element -> d_name;
						stat(filename, buffer);

						username_access = getpwuid(buffer->st_uid);
						if (username_access == NULL) {
							perror("getpwuid");
						}
						else {}

						group_access = getpwuid(buffer->st_gid);
						if (group_access == NULL) {
							perror("getpwuid");
						}
						else {}

						stattime = localtime(&buffer->st_ctime);

						strftime(timebuf, 80, "%c", stattime);
						if (filename[0] != '.') {
							printf((S_ISDIR(buffer->st_mode)) ? "d" : "-");
							printf((S_IRUSR & (buffer->st_mode)) ? "r" : "-");
							printf((S_IWUSR & (buffer->st_mode)) ? "w" : "-");
							printf((S_IXUSR & (buffer->st_mode)) ? "x" : "-");
							printf((S_IRGRP & (buffer->st_mode)) ? "r" : "-");
							printf((S_IWGRP & (buffer->st_mode)) ? "w" : "-");
							printf((S_IXGRP & (buffer->st_mode)) ? "x" : "-");
							printf((S_IROTH & (buffer->st_mode)) ? "r" : "-");
							printf((S_IWOTH & (buffer->st_mode)) ? "w" : "-");
							printf((S_IXOTH & (buffer->st_mode)) ? "x" : "-");


							printf("	%lu	%s	%s	%jd	%s	%s\n", 
							buffer->st_nlink,
							username_access->pw_name, group_access->pw_name, 
							buffer->st_size, timebuf, filename);
						}
											}
				}
			/*		
			case 'a': ;
				//DIR *directory;
				//struct dirent *dir_element;
       				if ((directory = opendir(current_dir)) == NULL) {
					perror("Could not open directory");
				}
				else {
					while ((dir_element = readdir(directory)) != NULL) {
						printf("%s	\n", dir_element -> d_name);
				
					}
				}

			case '?': ;
				  printf("unknown option: %c", optopt);
			*/

		}
	}

	/*
	for (int i = 0; i < argc - 1; i++) { //loop through the arguments

	}
	*/

        /*
	*if there are some, go to a new function and parse through the options -l or -a
	*/

	/*
	*set some booleans to true for -l or -a.
	*return to main function. loop through the non-option arguments
	*/
	return 0;
}	
