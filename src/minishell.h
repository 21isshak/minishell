/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:39:24 by iskaraag          #+#    #+#             */
/*   Updated: 2025/01/05 19:58:24 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_data
{
	// handle_input && execution
	char	*input;
	char	*command_path;
	pid_t	pid;
	int		status;
	// find_command_in_path
	char	*path;
	char	*path_copy;
	char	*token;
}			t_data;

typedef struct s_pipe_data
{
	int		fd[2];
	pid_t	pid;
	int		command_count;
	int		in_fd;
	char	***commands;
	int		i;
}			t_pipe_data;

typedef struct s_drct_data
{
	int		input_redirect;
	int		output_redirect;
	int		append_mode;
	char	*input_file;
	char	*output_file;
	int		fd;
}			t_drct_data;

extern char	**environ;
# define MAX_ARGS 1024

// ft_string_utils.c
size_t		ft_strlen(const char *str);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strdup(const char *src);

// path_utils.c
void		build_path(char *full_path, size_t size, const char *token,
				const char *command);
char		*find_command_in_path(const char *command);
char		*ft_strchr(const char *s, int c);
char		*ft_strtok(char *str, const char *delim);

// parse_utils.c
int			contains_pipe(char **args);
int			parse_piped_commands(char **args, char ***commands);
void		parse_command(char *input, char **args);

// builtin_handlers.c
void		handle_cd(char *args[]);
void		handle_env(void);
void		handle_unset(char *args[]);
void		handle_export(char *args[]);
void		handle_clear(void);

// builtin_execution.c
int			execute_builtin(char *args[]);

// signal_handling.c
void		handle_signal(int the_signal);

// input_handling.c
int			handle_input(t_data *data, char *args[]);

// execution.c
void		free_commands(char ***commands, int count);
void		execute_single_command(t_pipe_data *pdata, int i);
void		execute_piped_commands(char **args);
void		execution(t_data *data, char *args[]);

#endif