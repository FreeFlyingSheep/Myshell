#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <getopt.h>
#include <dirent.h>

#define TRUE  1
#define FALSE 0

#define INDENT 8

#define MAX_PID  32768
#define MAX_NAME 100
#define MAX_LEN  1024

typedef struct ps {
  char name[MAX_NAME];
  pid_t pid;
  pid_t ppid;
  int num;
  int exist;
  struct ps **child;
  int pos;
} ps;

int is_process(const char *name);
void get_ps(const char *name, pid_t id);
void get_info(const char *name, pid_t id);
void create_tree();
void print_tree(ps *pos, int indent);
void delete_tree();
void quick_sort(ps *p[], int left, int right);
void show_version();
void show_usage();

int show_pids = FALSE;
int numeric_sort = FALSE;

ps process[MAX_PID];

int main(int argc, char *argv[]) {
  int ch, index;
  DIR *dir;
  struct dirent *file;
  char *path = "/proc";
  char name[MAX_LEN];
  pid_t id;
  struct option options[] = {
    { "show-pids",    no_argument, 0, 'p' },
    { "numeric-sort", no_argument, 0, 'n' },
    { "version",      no_argument, 0, 'V' },
    { 0,              0,           0,  0 }
  };
  
  while (TRUE) {
    ch = getopt_long(argc, argv, "pnV", options, &index);
    
    if (ch == -1)
      break;
    
    switch (ch) {
      case 'p':
        show_pids = TRUE;
        break;
      case 'n':
        numeric_sort = TRUE;
        break;
      case 'V':
        show_version();
        exit(EXIT_SUCCESS);
      default:
        show_usage();
        exit(EXIT_FAILURE);
    }
  }
  
  if ((dir = opendir(path)) == NULL) {
    perror(path);
    exit(EXIT_FAILURE);
  }
  
  while ((file = readdir(dir)) != NULL) {
    if (file->d_type == DT_DIR && is_process(file->d_name)) {
      sprintf(name, "%s/%s", path, file->d_name);
      id = atoi(file->d_name);
      get_ps(name, id);
    }
  }
  
  closedir(dir);
  
  create_tree();
  print_tree(&process[1], 0);
  delete_tree();
  
  return 0;
}

int is_process(const char *name) {
  for ( ; *name; ++name) {
    if (!isdigit(*name))
      return FALSE;
  }
  return TRUE;
}

void get_info(const char *name, pid_t id)
{
  FILE *fp;
  char path[MAX_LEN];
  char pname[MAX_NAME];
  pid_t pid;
  pid_t ppid = id;
  
  sprintf(path, "%s/stat", name);
  if ((fp = fopen(path, "r")) == NULL) {
    perror(path);
    exit(EXIT_FAILURE);
  }
  
  fscanf(fp, "%d", &pid);
  fscanf(fp, "%s", pname);
  if (id == 0)
    fscanf(fp, "%*s%d", &ppid);
  
  strncpy(process[pid].name, pname + 1, strlen(pname) - 2);
  process[pid].pid = pid;
  process[pid].ppid = ppid;
  ++process[ppid].num;
  process[pid].exist = TRUE;
  
  fclose(fp);
}

void get_ps(const char *name, pid_t id)
{
  DIR *dir;
  struct dirent *file;
  char path[MAX_LEN];
  
  get_info(name, 0);
  
  sprintf(path, "%s/task", name);
  if ((dir = opendir(path)) == NULL) {
    perror(path);
    exit(EXIT_FAILURE);
  }
  
  while ((file = readdir(dir)) != NULL) {
    if (file->d_type == DT_DIR && is_process(file->d_name)) {
      if (atoi(file->d_name) != id) {
        sprintf(path, "%s/task/", name);
        strcat(path, file->d_name);
        get_info(path, id);
      }
    }
  }
  
  closedir(dir);
}

void create_tree()
{
  int i, j;
  
  for (i = 0; i < MAX_PID; ++i) {
    if (process[i].exist && process[i].num > 0)
      process[i].child = (ps **)malloc(process[i].num * sizeof(ps *));
  }
  
  for (i = 0; i < MAX_PID; ++i) {
    if (process[i].exist) {
      j = process[i].ppid;
      if (j > 0)
        process[j].child[process[j].pos++] = &process[i];
    }
  }
}

void delete_tree()
{
  int i;
  
  for (i = 0; i < MAX_PID; ++i) {
    if (process[i].exist && process[i].num > 0)
      free(process[i].child);
  }
}

void print_tree(ps *pos, int indent)
{
  int i;
  char buf[MAX_LEN];
  
  if (pos == NULL)
    return;
  
  if (show_pids)
    sprintf(buf, "%s(%d)", pos->name, pos->pid);
  else
    sprintf(buf, "%s", pos->name);
  printf("%*s%s\n", indent, "", buf);
  
  if (!numeric_sort)
    quick_sort(pos->child, 0, pos->num - 1);

  for (i = 0; i < pos->num; ++i)
    print_tree(pos->child[i], indent + INDENT);
}

void quick_sort(ps *p[], int left, int right)
{
  int i = left;
  int j = right;
  ps *key = p[left];
  
  if(left >= right)
    return;
  
  while(i < j) {
    while(i < j && strcmp(key->name, p[j]->name) <= 0)
      j--;
    p[i] = p[j];
    while(i < j && strcmp(key->name, p[i]->name) >= 0)
      i++;
    p[j] = p[i];
  }
  p[i] = key;
  quick_sort(p, left, i - 1);
  quick_sort(p, i + 1, right);
}

void show_version()
{
  printf("pstree 1.0\n");
  printf("Copyright (C) 2019 Chen Feiyang\n");
}

void show_usage()
{
  printf("Usage: pstree [-p] [-n]\n");
  printf("       pstree -V\n");
  printf("Display a tree of processes.\n\n");
  printf("  -p\tshow PIDs\n");
  printf("  -n\tsort output by PID\n");
  printf("  -V\tdisplay version information\n\n");
}