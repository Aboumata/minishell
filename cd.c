/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:33:15 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/14 14:33:16 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **split_by_spaces (char *line)
{
    return (NULL);
}

void builtin_cd (char *path)
{
    /*
    * args: array of string arguments, already P A R S E D.
    * e.g., for "cd ../dir" => args[0] = "cd", args[1] = "../dir", args[2] = NULL
    * Parsing must be done before calling this function.
    */


}
