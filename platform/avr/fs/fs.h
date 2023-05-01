#ifndef __FS_H__
#define __FS_H__

typedef enum
{
	FILETYPE_DIR = "DIR",
	FILETYPE_TEXT = "TXT"

} FileType;

typedef struct
{
	char Device;
	u32 StartBlock, SizeBlocks;
} File;

typedef struct
{
	char Device;
	u32 NumFiles;
	File InternalFile;
} Dir;

typedef struct
{
	u32 Size, Type;
	char Name[64], Type;
} DirEntry;

/**
 * Format a drive.
 *
 * @param dev Drive Letter
 */
StatusCode fs_format(char dev);


/**
 * Create a file.
 *
 * @param path File path
 */
StatusCode fs_fcreate(const char *path, u32 size);

StatusCode fs_fdelete(const char *path);
StatusCode fs_fopen(const char *path, File *file);

StatusCode fs_dopen(const char *path, Dir *dir);
StatusCode fs_dread(Dir *dir, i32 index, DirEntry *entry);

StatusCode fs_fresize(File *file, u32 new_size);
StatusCode fs_fread(File *file, u32 offset, u32 size, void *buffer);
StatusCode fs_fwrite(File *file, u32 offset, u32 size, const void *buffer);

#endif /* __FS_H__ */
