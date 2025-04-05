/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "sdkconfig.h"
#include "cJSON.h"

static const char *TAG = "MAIN";

// Mount path for the partition
//const char *base_path = "/root";
char *base_path = "/root";

#define STR_SIZE 64
typedef struct {
	uint8_t level;
	char self[STR_SIZE+1];
	char parent[STR_SIZE+1];
	cJSON *json;
} DIRS_t;

typedef struct {
	char file[STR_SIZE+1];
	struct stat info;
} FILES_t;

#define MAX_DIRS 10
#define MAX_FILES 10

#if 0
static void printDirectory(char * path) {
	DIR* dir = opendir(path);
	assert(dir != NULL);
	while (true) {
		struct dirent *pe = readdir(dir);
		if (!pe) break;
		ESP_LOGI(__FUNCTION__,"%s d_name=%s d_ino=%d d_type=%x", path, pe->d_name, pe->d_ino, pe->d_type);
		if (pe->d_type == 1) {
			char fullPath[STR_SIZE+1];
			strcpy(fullPath, path);
			strcat(fullPath, "/");
			strcat(fullPath, pe->d_name);
			struct stat st;
			if (stat(fullPath, &st) == 0) {
				ESP_LOGI(__FUNCTION__,"fileSize=%d", (int)st.st_size);
			}
		}

		if (pe->d_type == 2) {
			char subDir[STR_SIZE+1];
			strcpy(subDir, path);
			strcat(subDir, "/");
			strcat(subDir, pe->d_name);
			ESP_LOGI(__FUNCTION__, "subDir=[%s]", subDir);
			printDirectory(subDir);
		}
	}
	closedir(dir);
}
#endif

static void saveFiles(char * path, FILES_t *files, int filesSize, int * count) {
	ESP_LOGI(__FUNCTION__,"path=[%s]", path);
	DIR* dir = opendir(path);
	assert(dir != NULL);
	while (true) {
		struct dirent *pe = readdir(dir);
		if (!pe) break;
		ESP_LOGD(__FUNCTION__,"%s d_name=%s d_ino=%d d_type=%x", path, pe->d_name, pe->d_ino, pe->d_type);
		if (pe->d_type == 1) {
			int _count = *count;
			strcpy(files[_count].file, pe->d_name);
			char fullPath[STR_SIZE+1];
			strcpy(fullPath, path);
			strcat(fullPath, "/");
			strcat(fullPath, pe->d_name);
			stat(fullPath, &files[_count].info);
			if ((*count) == (filesSize-1)) return;
			(*count)++;
		}
#if 0
		if (pe->d_type == 2) {
			char subDir[STR_SIZE+1];
			sprintf(subDir,"%s/%.64s", path, pe->d_name);
			ESP_LOGI(__FUNCTION__, "subDir=[%s]", subDir);
			printDirectory(subDir);
		}
#endif
	}
	closedir(dir);
}

static void saveDirectorys(char * path, DIRS_t *dirs, int dirsSize, int * count) {
	int level = 0;
	for (int i=0;i<strlen(path);i++) {
		if (path[i] == '/') level++;
	}
	ESP_LOGI(__FUNCTION__,"path=[%s] level=%d", path, level);
	if (level == 1) {
		int _count = *count;
		dirs[_count].level = level;
		strcpy(dirs[_count].self, path);
		strcpy(dirs[_count].parent, "");
		(*count)++;
	}

	DIR* dir = opendir(path);
	assert(dir != NULL);
	while (true) {
		struct dirent *pe = readdir(dir);
		if (!pe) break;
		ESP_LOGD(__FUNCTION__,"%s d_name=%s d_ino=%d d_type=%x", path, pe->d_name, pe->d_ino, pe->d_type);
#if 0
		if (pe->d_type == 1) {
			char fullPath[STR_SIZE+1];
			strcpy(fullPath, path);
			strcat(fullPath, "/");
			strcat(fullPath, pe->d_name);
			int fileSize = getFileSize(fullPath);
			ESP_LOGI(__FUNCTION__,"fileSize=%d", fileSize);
		}
#endif
		if (pe->d_type == 2) {
			char subDir[STR_SIZE+1];
			strcpy(subDir, path);
			strcat(subDir, "/");
			strcat(subDir, pe->d_name);
			ESP_LOGI(__FUNCTION__, "level=%d subDir=[%s]", level, subDir);
			int _count = *count;
			dirs[_count].level = (level+1);
			strcpy(dirs[_count].self, subDir);
			strcpy(dirs[_count].parent, path);
			if ((*count) == (dirsSize-1)) return;
			(*count)++;
			saveDirectorys(subDir, dirs, dirsSize, count);

		}
	}

	closedir(dir);
}

void app_main(void)
{
	ESP_LOGI(TAG, "Mounting FAT filesystem");
	// To mount device we need name of device partition, define base_path
	// and allow format partition in case if it is new one and was not formatted before
	const esp_vfs_fat_mount_config_t mount_config = {
		.max_files = 4,
		.format_if_mount_failed = false,
		.allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
		.use_one_fat = false,
	};

	//esp_err_t err;
	esp_err_t err = esp_vfs_fat_spiflash_mount_ro(base_path, "storage", &mount_config);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
		return;
	}

	//printDirectory(base_path);

	//DIRS_t dirs[MAX_DIRS];
	DIRS_t *dirs;
	dirs = malloc (sizeof(DIRS_t) * MAX_DIRS);
	if (dirs == NULL) {
		ESP_LOGE(TAG, "dirs malloc fail");
		return;
	}

	//FILES_t files[MAX_FILES];
	FILES_t *files;
	files = malloc (sizeof(FILES_t) * MAX_FILES);
	if (files == NULL) {
		ESP_LOGE(TAG, "files malloc fail");
		return;
	}

	// Collect directory
	int dirCount = 0;
	saveDirectorys(base_path, dirs, MAX_DIRS, &dirCount);
	ESP_LOGI(TAG, "dirCount=%d", dirCount);
	for (int i=0;i<dirCount;i++) {
		ESP_LOGI(TAG, "dirs[%d] level=%d self=[%s] parent=[%s]", i, dirs[i].level, dirs[i].self, dirs[i].parent);
	}

	// Build tree format json
	ESP_LOGI(TAG, "Serialize.....");
	for (int target_level=10;target_level>=0;target_level--) {
		for (int i=0;i<dirCount;i++) {
			if (dirs[i].level != target_level) continue;

			// Collect files in directory
			int fileCount = 0;
			saveFiles(dirs[i].self, files, MAX_FILES, &fileCount);
			ESP_LOGD(TAG, "fileCount=%d", fileCount);

			dirs[i].json = cJSON_CreateObject();
			ESP_LOGD(TAG, "target_level=%d dirs[%d] self=[%s] parent=[%s]",
				target_level, i, dirs[i].self, dirs[i].parent);
			cJSON_AddNumberToObject(dirs[i].json, "level", dirs[i].level);
			cJSON_AddStringToObject(dirs[i].json, "self", dirs[i].self);

			struct stat info;
			struct tm timeinfo;
			char atime[32];
			char mtime[32];
			char ctime[32];
			for (int j=0;j<fileCount;j++) {
				info = files[j].info;
				localtime_r(&info.st_atime, &timeinfo);
				strftime(atime, sizeof(atime), "%Y-%m-%d %T", &timeinfo);
				localtime_r(&info.st_mtime, &timeinfo);
				strftime(mtime, sizeof(mtime), "%Y-%m-%d %T", &timeinfo);
				localtime_r(&info.st_ctime, &timeinfo);
				strftime(ctime, sizeof(ctime), "%Y-%m-%d %T", &timeinfo);
				ESP_LOGD(TAG, "files[%d] file=[%s] size=%d mtime=%s", j, files[j].file, (int)info.st_size, mtime);
				cJSON *fileInfo;
				fileInfo = cJSON_CreateObject();
				cJSON_AddNumberToObject(fileInfo, "size", (int)info.st_size);
				//cJSON_AddStringToObject(fileInfo, "atime", atime);
				cJSON_AddStringToObject(fileInfo, "mtime", mtime);
				//cJSON_AddStringToObject(fileInfo, "ctime", ctime);
				cJSON_AddItemToObject(dirs[i].json, files[j].file, fileInfo);
			}

			// Search child directory
			for (int j=0;j<dirCount;j++) {
				if (strcmp(dirs[i].self, dirs[j].parent) != 0) continue;
				ESP_LOGD(TAG, "dirs[%d] has child node. self=[%s]", j, dirs[j].self);
				cJSON *array;
				array = cJSON_CreateArray();
				for (int k=0;k<dirCount;k++) {
					if (strcmp(dirs[i].self, dirs[k].parent) != 0) continue;
					cJSON_AddItemToArray(array, dirs[k].json);
				}
				cJSON_AddItemToObject(dirs[i].json, "child", array);
				break;
			}
		}
	}

	// print json string
	char *my_json_string = cJSON_Print(dirs[0].json);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(dirs[0].json);

	// Buffers returned by cJSON_Print must be freed by the caller.
	// Please use the proper API (cJSON_free) rather than directly calling stdlib free.
	cJSON_free(my_json_string);

	free(dirs);
	free(files);

	// Unmount FATFS
	ESP_LOGI(TAG, "Unmounting FAT filesystem");
	ESP_ERROR_CHECK(esp_vfs_fat_spiflash_unmount_ro(base_path, "storage"));
	ESP_LOGI(TAG, "Done");
}
