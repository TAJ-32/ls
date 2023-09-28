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
					printf("%s	", dir_element -> d_name);
				}
			}
			printf("\n");
		}
	}
	else {
		int opt;
		char *Directories[argc];
		int Dir_Length = 0;
		
		for (int i = 1; i < argc; i++) {

			if (argv[i][0] != '-') {
				Directories[Dir_Length] = argv[i];
				//printf("%s \n", Directories[Dir_Length]);
				Dir_Length++;
			}
		}

		
		
		if (Dir_Length == (argc - 1)) {
			//printf("NO OPTIONS\n");
			for (int i = 0; i < Dir_Length; i++) {
				DIR *directory;
				struct dirent *dir_element;

				strncpy(current_dir, Directories[i], sizeof(current_dir));

				//printf("%s \n", current_dir);

				if ((directory = opendir(current_dir)) == NULL) {
					perror("Could not open directory");
				}
				else {

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


		if ((Dir_Length == 0)) {
			Directories[0] = current_dir;
			Dir_Length += 1;
		}

		//for (int i = 0; i < Dir_Length; i++) {
		//	printf("Directories: %s \n", Directories[i]);
		//}


		while ((opt = getopt(argc, argv, "la")) != -1) {
			DIR *directory;
			struct dirent *dir_element;

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
