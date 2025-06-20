/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:03:27 by aboumata          #+#    #+#             */
/*   Updated: 2025/06/20 19:03:32 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_exec.h"

void cleanup_execution(char *path, char **argv)
{

}

int handle_external_command(char **tokens, char **envp)
{
    char *path;
    char **argv;
    int status;

    if (!tokens || !tokens[0])
        return (-1);

    path = find_executable(tokens[0]);
    if (!path)
    {
        perror("command not found");
        return (127);
    }
    argv = create_argv(tokens);
    if (!argv) {
        free(path);
        return (-1);
    }
    status = execute_command(path, argv, envp);
    cleanup_execution(path, argv);
    return (status);
}
