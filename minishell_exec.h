/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 17:10:15 by aboumata          #+#    #+#             */
/*   Updated: 2025/06/19 17:10:16 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_EXEC_H
#define MINISHELL_EXEC_H

void	free_path_directories(char **directories);
char	**get_path_directories(void);

#endif