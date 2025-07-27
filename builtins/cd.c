/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                      +:+      +:+     :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:33:15 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/14 14:33:16 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_current_directory(void)
{
	char	buffer[4096];

	if (getcwd(buffer, sizeof(buffer)) != NULL)
		return (ft_strdup(buffer));
	return (NULL);
}

static void	update_pwd_variables(t_envs **env, char *old_pwd)
{
	char	*new_pwd;

	if (old_pwd)
	{
		remove_env(env, "OLDPWD");
		*env = add_env(*env, "OLDPWD", old_pwd);
	}
	new_pwd = get_current_directory();
	if (new_pwd)
	{
		remove_env(env, "PWD");
		*env = add_env(*env, "PWD", new_pwd);
		free(new_pwd);
	}
}

static char	*get_target_path(char *path)
{
	char	*home;

	if (!path)
	{
		home = getenv("HOME");
		if (!home)
		{
			printf("cd: HOME not set\n");
			return (NULL);
		}
		return (ft_strdup(home));
	}
	return (ft_strdup(path));
}

static int	check_cd_args(char **args)
{
	int	count = 0;

	while (args[count])
		count++;
	if (count > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int	builtin_cd(char **args)
{
	char	*target_path;
	char	*old_pwd;

	if (check_cd_args(args))
		return (1);
	old_pwd = get_current_directory();
	target_path = get_target_path(args[1]);
	if (!target_path)
	{
		free(old_pwd);
		return (1);
	}
	if (chdir(target_path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(target_path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(target_path);
		free(old_pwd);
		return (1);
	}
	update_pwd_variables(&g_env, old_pwd);
	free(target_path);
	free(old_pwd);
	return (0);
}
