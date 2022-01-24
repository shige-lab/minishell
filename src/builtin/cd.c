#include "minishell.h"

static void	_update_pwd_helper(char *path, char *key)
{
	char	*val;

	val = my_getenv(g_mshell->envlist, key);
	if (val == NULL)
		return ;
	if (path != NULL)
		msh_export(&(g_mshell->envlist), key, path);
}

static void	_update_pwd_in_envlist(char *path)
{
	_update_pwd_helper(path, "PWD");
}

static void	_update_oldpwd_in_envlist(char *path)
{
	_update_pwd_helper(path, "OLDPWD");
}

void	cd_(char **cmds)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (chdir(cmds[1]) == -1)
	{
		free(path);
		error_with_errno("cd", cmds[1]);
		return (add_exit_status_to_env(1));
	}
	_update_oldpwd_in_envlist(path);
	free(path);
	add_exit_status_to_env(0);
	path = getcwd(NULL, 0);
	_update_pwd_in_envlist(path);
	free(path);
}
