#include "fs.h"
#include <atfs.h>
#include <types.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <status.h>

static char buf[512];

#define TEST_PATH_VALID(str) printf("path_valid(\"%s\") = %s\n", str, path_valid(str) ? "true" : "false")
#define TEST_PATH_PARENT(str) printf("path_parent(\"%s\") = \"%s\"\n", str, path_parent(strdup(str)))
#define TEST_PATH_JOIN(str, append) printf("path_join(\"%s\", \"%s\") = \"%s\"\n", str, append, path_join(strcpy(buf, str), append))

Status cmd_ls(const char *path)
{
	u32 i;
	Dir dir;
	DirEntry entry;

	RETURN_IF(fs_dopen(path, &dir));
	printf("Number of Entries: %d\n", dir.NumEntries);

	exit(1);

	for(i = 0; i < dir.NumEntries; ++i)
	{
		RETURN_IF(fs_dread(&dir, i, &entry));
		printf("%s\n", entry.Name);
	}

	return STATUS_OK;
}

int main(void)
{
	TEST_PATH_VALID("home.files.studium.gs.its_board.a2_drehgeber.main");
	TEST_PATH_VALID("sys");
	TEST_PATH_VALID("sys.init");
	TEST_PATH_VALID(".abc");
	TEST_PATH_VALID("abc.");
	TEST_PATH_VALID("a.bc..");
	TEST_PATH_VALID("a.b.c.d.e.f.g");
	TEST_PATH_VALID("home..files");
	TEST_PATH_VALID("home.files");
	TEST_PATH_VALID("home.4chan");
	TEST_PATH_VALID("home.chan2");
	TEST_PATH_VALID("home.Uppercase");
	TEST_PATH_VALID("home.#hashtag");
	TEST_PATH_VALID("");

	printf("\n\n");

	TEST_PATH_PARENT("home.files.images");
	TEST_PATH_PARENT("home");

	printf("\n\n");

	TEST_PATH_JOIN("home.files.images", "vacation");
	TEST_PATH_JOIN("sys", "init");

	printf("\n\n");

	printf("fs_format() = %s\n", status_str(fs_format()));

	printf("\n\n");

	printf("cmd_ls(\"\") = %s\n", status_str(cmd_ls("")));

	printf("\n\n");

	return 0;
}
