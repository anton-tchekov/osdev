#include <std.h>
#include "keyword.h"

static const char *_keywords[] =
    {
        "auto",
        "await",
        "bool",
        "break",
        "case",
        "catch",
        "char",
        "class",
        "const",
        "continue",
        "debugger",
        "default",
        "delete",
        "do",
        "double",
        "else",
        "enum",
        "export",
        "extends",
        "extern",
        "f32",
        "f64",
        "false",
        "finally",
        "float",
        "for",
        "function",
        "goto",
        "i16",
        "i32",
        "i64",
        "i8",
        "if",
        "implements",
        "import",
        "in",
        "instanceof",
        "int",
        "interface",
        "let",
        "long",
        "new",
        "null",
        "package",
        "private",
        "protected",
        "public",
        "register",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "struct",
        "super",
        "switch",
        "this",
        "throw",
        "true",
        "try",
        "typedef",
        "typeof",
        "u16",
        "u32",
        "u64",
        "u8",
        "union",
        "unsigned",
        "var",
        "void",
        "volatile",
        "while",
        "with",
        "yield"};

#define HASHMAP_SIZE (ARRLEN(_keywords) * 3)
#define COLLISION_LIMIT 4

static i32 _hashmap[HASHMAP_SIZE];

static u32 _keyword_hash(const char *word, u32 len)
{
    u32 i, hash;
    i32 c;

    hash = 5381;
    for (i = 0; i < len && (c = word[i]); ++i)
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void keyword_init(void)
{
    u32 i, hash, steps;

    debug_print("hashmap size: %d bytes\n", sizeof(_hashmap));

    for (i = 0; i < HASHMAP_SIZE; ++i)
    {
        _hashmap[i] = -1;
    }

    for (i = 0; i < ARRLEN(_keywords); ++i)
    {
        steps = 0;
        hash = _keyword_hash(_keywords[i], UINT32_MAX);
        while (_hashmap[hash % HASHMAP_SIZE] != -1)
        {
            ++hash;
            if (++steps >= COLLISION_LIMIT)
            {
                PANIC("Keyword hashmap collision limit exceeded");
            }
        }

        debug_print("Collision with: %s %d\n", _keywords[i], steps);

        _hashmap[hash % HASHMAP_SIZE] = i;
    }

    for (i = 0; i < HASHMAP_SIZE; ++i)
    {
        debug_print("%3d : %3d => %s\n", i, _hashmap[i], _hashmap[i] < 0 ? "" : _keywords[_hashmap[i]]);
    }

    debug_print("keyword_detect(\"int\", 3) = %s\n", keyword_detect("int", 3) ? "true" : "false");
    debug_print("keyword_detect(\"bla\", 3) = %s\n", keyword_detect("bla", 3) ? "true" : "false");
}

bool keyword_detect(const char *str, u32 len)
{
    u32 i, hash;

    hash = _keyword_hash(str, len);
    for (i = 0; i < COLLISION_LIMIT; ++i)
    {
        i32 index = _hashmap[hash % HASHMAP_SIZE];
        if (index != -1 && len == strlen(_keywords[index]) &&
            !strncmp(str, _keywords[index], len))
        {
            return true;
        }

        ++hash;
    }

    return false;
}
