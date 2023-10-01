#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[]) {

        /*
	String to hold name of the current directory, which shouldn't be longer than 256 characters 
	 
	 */

	char current_dir[256];


	
	if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
		perror("getcwd() error");
	}
	else {
		//current_dir = current_dir;
	}

	//Condition if we run ./myls with no arguments
	
	if (argc == 1) {
		DIR *directory;
		struct dirent *dir_element;
		if ((directory = opendir(current_dir)) == NULL) {
			perror("Could not open directory");
		}
		else {
			while ((dir_element = readdir(directory)) != NULL) {
				//Condition does not allow hidden directories to be included in the directories printed
				if (dir_element -> d_name[0] != '.') {
					printf("%s	", dir_element -> d_name);
				}
			}
			printf("\n");
		}
	}
	//Condition if ./myls has other arugments
	else {
		int opt;
		char *Directories[argc];
		int Dir_Length = 0;
	//For loop to get all of the directories listed in the arguments 
		for (int i = 1; i < argc; i++) {

			if (argv[i][0] != '-') {
				Directories[Dir_Length] = argv[i];
				//printf("%s \n", Directories[Dir_Length]);
				Dir_Length++;
			}
		}

		
		//Condition if ./myls is ran with no option arguments
		if (Dir_Length == (argc - 1)) {
		//Does the myls command Dir_length times for each directories in the array Directories[]
			for (int i = 0; i < Dir_Length; i++) {
				DIR *directory;
				struct dirent *dir_element;

				strncpy(current_dir, Directories[i], sizeof(current_dir));

				//printf("%s \n", current_dir);

				if ((directory = opendir(current_dir)) == NULL) {
					perror("Could not open directory");
				}
				else {
					//Condition if there is more than one directory to specify the each directories name
					if (Dir_Length > 1) {
						printf("%s: \n", current_dir);
					}

					while ((dir_element = readdir(directory)) != NULL) {

						if (dir_element -> d_name[0] != '.') {
							printf("%s	", dir_element -> d_name);
						}
					}
					printf("\n");
				}
			}
		}

		//Condition if myls is ran with no file arguments but has option arugments, puts currentdir inside Directories[]
		if ((Dir_Length == 0)) {
			Directories[0] = current_dir;
			Dir_Length += 1;
		}

		//for (int i = 0; i < Dir_Length; i++) {
		//	printf("Directories: %s \n", Directories[i]);
		//}

		//Lists which option arugments are available 'l' and 'a'
		while ((opt = getopt(argc, argv, "la")) != -1) {
			DIR *directory;
			struct dirent *dir_element;
		//Seperates the procedure for each option arugment 
			switch (opt) {

				case 'a':
					for (int i = 0; i < Dir_Length; i++) {
						//DIR *directory;
						//struct dirent *dir_element;
						
						strncpy(current_dir, Directories[i], sizeof(current_dir));

						if ((directory = opendir(current_dir)) == NULL) {
							perror("Could not open directory");
						}
						else {
							if (Dir_Length > 1) {
								printf("%s: \n", current_dir);
							}

							while ((dir_element = readdir(directory)) != NULL) {
								printf("%s	", dir_element -> d_name);
							}
							printf("\n");
						}
					}	


				case 'l':
					for (int i = 0; i < Dir_Length; i++) {
						//DIR *directory;
						//struct dirent *dir_element;
						struct passwd *username_access;
						struct passwd *group_access;
						struct tm *stattime;
						char timebuf[80];

						strncpy(current_dir, Directories[i], sizeof(current_dir));
						

						printf("\n");
						struct stat *buffer = malloc(sizeof(struct stat));
						if ((directory = opendir(current_dir)) == NULL) {
							perror("Could not open directory");
						}
						else {
							if (Dir_Length > 1) {
								printf("%s: \n", current_dir);
							}


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
							//Lists the directory permissions
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
						free(buffer);			
					}	

			}

	
		}
	}


	return 0;
}	
