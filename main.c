/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:20:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/04/26 16:20:02 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minshell.h"

int	main(int argc, char **argv, char **envp)
{
	t_envs	*env_list;
	t_envs	*found;

	(void)argc;
	(void)argv;
	env_list = init_env(envp);
	printf("🌟 Original environment:\n");
	print_env(env_list);
	found = get_env(env_list, "PATH");
	if (found)
		printf("\n🔍 Found: %s = %s\n", found->names, found->values);
	else
		printf("\n🔍 PATH not found.\n");
	printf("\n✏️ Updating PATH...\n");
	update_env(env_list, "PATH", "/usr/new_path");
	print_env(env_list);
	printf("\n🗑 Removing PATH...\n");
	remove_env(&env_list, "PATH");
	print_env(env_list);
	printf("\n🧹 Freeing the environment list...\n");
	free_env(env_list);
	return (0);
}
