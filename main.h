#ifndef _MAIN_H
#define _MAIN_H_

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>


/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1
#define TOK_BUFSIZE 128
#define TOK_DELIM " \t\r\n\a"

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2


/* _LIST_T */
/**
 *  * struct liststr - singly linked list
 *   * @num: the number field
 *    * @str: a string
 *     * @next: points to the next node
 *      */
typedef struct liststr
{
	    int num;
	        char *str;
		    struct liststr *next;
} list_t;



/*environ*/
typedef struct env_var {
		char *name;
			char *value;
} env_var;

typedef struct env_list {
		struct env_var *var;
			struct env_list *next;
} env_list;


/*info_t*/
/**
 *  * struct passinfo - contains pseudo-arguements to pass into a function,
 *   * allowing uniform prototype for function pointer struct
 *    * @arg: a string generated from getline containing arguements
 *     * @argv:an array of strings generated from arg
 *      * @path: a string path for the current command
 *       * @argc: the argument count
 *        * @line_count: the error count
 *         * @err_num: the error code for exit()s
 *          * @linecount_flag: if on count this line of input
 *           * @fname: the program filename
 *            * @env: linked list local copy of environ
 *             * @environ: custom modified copy of environ from LL env
 *              * @history: the history node
 *               * @alias: the alias node
 *                * @env_changed: on if environ was changed
 *                 * @status: the return status of the last exec'd command
 *                  * @cmd_buf: address of pointer to cmd_buf, on if chaining
 *                   * @cmd_buf_type: CMD_type ||, &&, ;
 *                    * @readfd: the fd from which to read line input
 *                     * @histcount: the history line number count
 *                      */
typedef struct info_t
{
	    char *arg;
	        char **argv;
		    char *path;
		        int argc;
			    unsigned int line_count;
			        int err_num;
				    int linecount_flag;
				        char *fname;
					    list_t *env;
					        list_t *history;
						    list_t *alias;
						        char **environ;
							    int env_changed;
							        int status;

								    char **cmd_buf; /* pointer to cmd ; chain buffer, for memory management */
								        int cmd_buf_type; /* CMD_type ||, &&, ; */
									    int readfd;
									        int histcount;
} info_t;


extern void _eputs(char *);
extern int read_history(info_t *);
extern int hsh(info_t *, char **av);
extern int populate_env_list(info_t *info);

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;

int *read_link(info_t *, char *i_eof);

/* Function prototype */
ssize_t my_getline(int fd, char **line, size_t *len, ssize_t bytes_read);
void process_char(char current_char);


/* Function declarations*/
 char *no_comment(char *in);
 void shell_lop(info_t *info);

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
			0, 0, 0}

		/* main.h */


/* Function prototypes */
int _myenv(info_t *info);
int _myhelp(info_t *info);



		/* get_builtin */
int (*get_builtin(char *cmd))(info_t *info);
/**
 *  * struct builtin - contains a builtin string and related function
 *   * @type: the builtin command flag
 *    * @func: the function
 *     */
typedef struct builtin
{
		char *type;
			int (*func)(info_t *);
} builtin_table;


/* toem_shloop.c */
int hsh(info_t *, char **av);
int find_builtin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);

/* toem_parser.c */
int is_cmd(info_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* shell_loop.c */
char *no_comment(char *in);
void shell_lop(info_t *info);

/* toem_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* Function to print a string to the standard error */
void _eputs(char *str) {
	    fprintf(stderr, "%s", str);
}

/* Function to print a character to the standard error */
int _eputchar(char c) {
	    return fputc(c, stderr);
}


/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_atoi.c */
int interactive(info_t *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* toem_errors1.c */
int _erratoi(char *);
void print_error(info_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int _myexit(info_t *);
int _mycd(info_t *);
int _myhelp(info_t *);

/* toem_builtin1.c */
int _myhistory(info_t *);
int _myalias(info_t *);

/*toem_getline.c */
ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfo.c */
void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* toem_environ.c */
char *_getenv(info_t *, const char *);
int _myenv(info_t *info);
int _mysetenv(info_t *);
int _myunsetenv(info_t *);
int populate_env_list(info_t *);

/* toem_getenv.c */
char **get_environ(info_t *);
int _unsetenv(info_t *info, char *);
int _setenv(info_t *info, char *, char *);

/* toem_history.c */
char *get_history_file(info_t *info);
int write_history(void);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/* toem_lists.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* toem_lists1.c */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* toem_vars.c */
int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);

#endif
