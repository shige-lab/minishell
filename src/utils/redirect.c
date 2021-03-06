#include "minishell.h"

int	handle_in_redir(t_redirect_list *redirect)
{
	int	fd;

	if (redirect == NULL)
		return (0);
	if (redirect->is_ambiguous == true)
	{
		error_message(redirect->word, "ambiguous redirect" );
		return (-1);
	}
	if (ft_strcmp(redirect->redirect, ">>") == 0)
		fd = open(redirect->word, O_RDWR | O_APPEND | O_CREAT, 0664);
	else
		fd = open(redirect->word, O_RDWR | O_CREAT | O_TRUNC, 0664);
	if (fd == -1)
	{
		error_message(redirect->word, strerror(errno));
		return (-1);
	}
	ft_dup2(fd, 1);
	close(fd);
	if (redirect->next != NULL)
		handle_in_redir(redirect->next);
	return (0);
}

int	handle_out_redir(t_redirect_list *redirect)
{
	int	fd;

	if (redirect == NULL)
		return (0);
	if (redirect->is_ambiguous == true)
	{
		error_message(redirect->word, "ambiguous redirect" );
		return (-1);
	}
	if (ft_strcmp(redirect->redirect, "<<"))
	{
		fd = open(redirect->word, O_RDONLY);
		if (fd == -1)
		{
			error_message(redirect->word, strerror(errno));
			return (-1);
		}
		ft_dup2(fd, 0);
		close(fd);
	}
	if (redirect->next != NULL)
		handle_out_redir(redirect->next);
	return (0);
}

t_redirect_list	*_redir_lstlast(t_redirect_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
