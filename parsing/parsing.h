/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:55:14 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/15 17:55:15 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"

typedef struct s_exp_ctx
{
	const char	*input;
	t_envs		*env;
	int			last_status;
	char		*out;
	int			*j;
	int			idx;
	int			*i;
}				t_exp_ctx;

char			*get_var_value(const char *var, t_envs *env, int last_status);
int				expand_question(t_exp_ctx *ctx);
int				expand_name(t_exp_ctx *ctx);
int				replace_variable(t_exp_ctx *ctx);
int				is_var_char(char c, int first);
int				write_value(const char *val, char *out, int *j);
char			*expand_variables(const char *input, t_envs *env,
					int last_status);
void			process_dollar(t_exp_ctx *ctx);
void			copy_literal_content(const char *input, int *i, char *out,
					int *j, char quote_char);
void			expand_within_quotes(const char *input, int *i, char *out,
					int *j, t_exp_ctx *ctx);
void			skip_quote_delimiter(const char *input, int *i,
					char quote_char);
void			setup_expansion_context(t_exp_ctx *ctx, const char *input,
					t_envs *env, int last_status);
void			process_expansion_char(const char *input, int *i, char *out,
					int *j, t_exp_ctx *ctx);
bool			is_whitespace(char c);
int				is_special(const char *s, int i);
void			skip_spaces(const char *s, int *i);

#endif