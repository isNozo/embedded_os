#include "defines.h"
#include "lib.h"
#include "elf.h"

struct elf_header {
  struct {
    unsigned char magic[4];
    unsigned char class;
    unsigned char format;
    unsigned char version;
    unsigned char abi;
    unsigned char abi_version;
    unsigned char reserve[7];
  } id;
  short type;
  short arch;
  long version;
  long entry_point;
  long program_header_offset;
  long section_header_offset;
  long flags;
  short header_size;
  short program_header_size;
  short program_header_num;
  short section_header_size;
  short section_header_num;
  short section_name_index;
};

struct elf_program_header {
  long type;
  long offset;
  long virtual_addr;
  long physical_addr;
  long file_size;
  long memory_size;
  long flags;
  long align;
};

static int elf_check(struct elf_header *header)
{
  puts_d("elf_check is called.\n");

  if (memcmp(header->id.magic, "\x7f" "ELF", 4))
    return -1;
  
  if (header->id.class   != 1) return -1; /* 32-bit objects */
  if (header->id.format  != 2) return -1; /* 2's complement, big endian */
  if (header->id.version != 1) return -1; /* version 1 */
  if (header->type       != 2) return -1; /* Executable file */
  if (header->version    != 1) return -1; /* version 1 */

  /* Hitachi H8/300 or H8/300H */
  if ((header->arch != 46) && (header->arch != 47)) return -1;
  
  return 0;
}

static int elf_load_program(struct elf_header *header)
{
  puts_d("elf_load_program is called.\n");

  int i;
  struct elf_program_header *phdr;

  puts("Offset VirtAddr PhysAddr Fsize Msize Fl Align\n");
  for (i = 0; i < header->program_header_num; i++) {
    phdr = (struct elf_program_header *)
      ((char *)header + header->program_header_offset + header->program_header_size * i);

    if (phdr->type != 1)
      continue;

    putxval(phdr->offset       , 6); puts(" ");
    putxval(phdr->virtual_addr , 8); puts(" ");
    putxval(phdr->physical_addr, 8); puts(" ");
    putxval(phdr->file_size    , 5); puts(" ");
    putxval(phdr->memory_size  , 5); puts(" ");
    putxval(phdr->flags        , 2); puts(" ");
    putxval(phdr->align        , 2); puts("\n");
  }

  return 0;
}

int elf_load(char *buf)
{
  puts_d("elf_load is called.\n");

  struct elf_header *header = (struct elf_header *)buf;

  puts_d("Elf Header :\n");
  puts_d("  Magic                 : 0x"); putxval_d(header->id.magic              , 8); puts_d("\n");
  puts_d("  Class                 : 0x"); putxval_d(header->id.class              , 2); puts_d("\n");
  puts_d("  Format                : 0x"); putxval_d(header->id.format             , 2); puts_d("\n");
  puts_d("  Version               : 0x"); putxval_d(header->id.version            , 2); puts_d("\n");
  puts_d("  ABI                   : 0x"); putxval_d(header->id.abi                , 2); puts_d("\n");
  puts_d("  ABI version           : 0x"); putxval_d(header->id.abi_version        , 2); puts_d("\n");
  puts_d("  Type                  : 0x"); putxval_d(header->type                  , 4); puts_d("\n");
  puts_d("  Arch                  : 0x"); putxval_d(header->arch                  , 4); puts_d("\n");
  puts_d("  Version               : 0x"); putxval_d(header->version               , 8); puts_d("\n");
  puts_d("  Entry point           : 0x"); putxval_d(header->entry_point           , 8); puts_d("\n");
  puts_d("  Program Header Offset : 0x"); putxval_d(header->program_header_offset , 8); puts_d("\n");
  puts_d("  Section Header Offset : 0x"); putxval_d(header->section_header_offset , 8); puts_d("\n");
  puts_d("  Flags                 : 0x"); putxval_d(header->flags                 , 8); puts_d("\n");
  puts_d("  Header size           : 0x"); putxval_d(header->header_size           , 4); puts_d("\n");
  puts_d("  Program Header Size   : 0x"); putxval_d(header->program_header_size   , 4); puts_d("\n");
  puts_d("  Program Header Num    : 0x"); putxval_d(header->program_header_num    , 4); puts_d("\n");
  puts_d("  Section Header Size   : 0x"); putxval_d(header->section_header_size   , 4); puts_d("\n");
  puts_d("  Section Header Num    : 0x"); putxval_d(header->section_header_num    , 4); puts_d("\n");
  puts_d("  Section Name Index    : 0x"); putxval_d(header->section_name_index    , 4); puts_d("\n");

  if (elf_check(header) < 0)
    return -1;

  if (elf_load_program(header) < 0)
    return -1;

  return 0;
}
