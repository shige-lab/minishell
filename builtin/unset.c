#include "minishell.h"

void	unset_(char **args)
{
	size_t	i;
	char	*equal_at;

	if (args[0] == NULL)
		return ;
	i = 0;
	while (args[i] != NULL)
	{
		if (args[0][0] == '-') // is_option
		{
			error_option("unset", args[i]);
			print_command_usage("unset", "unset [name[=value] ...]");
			i++;
			continue ;
		}
		equal_at = ft_strchr(args[i], '=');
		if (!ft_isalpha(args[i][0]) || equal_at != NULL)
		{
			error_ident("unset", args[i]);
			i++;
			continue ;
		}
		else
		{
			msh_unset(&(g_mshell->envlist), args[i]);
			i++;
			continue ;
		}
	}
}

void	msh_unset(t_envvar **envs, char *key)
{
	t_envvar	*head;
	t_envvar	*cur;
	t_envvar	*prev;
	size_t		key_len;

	head = *envs;
	prev = NULL;
	key_len = ft_strlen(key);
	while (*envs != NULL)
	{
		if (ft_strcmp((*envs)->key, key) == 0)
		{
			delete_envvar_contents((*envs)->key, (*envs)->val);
			cur = *envs;
			(*envs) = cur->next;
			if (prev != NULL)
				prev->next = cur->next;
			else
				head = cur->next;
			free(cur);
			break ;
		}
		prev = *envs;
		*envs = (*envs)->next;
	}
	*envs = head;
}
