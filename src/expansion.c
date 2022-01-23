#include "minishell.h"

void	join_token_and_token_prev(t_token **token)
{
	t_token	*tmp;
	printf("join_token_and_token_perv called\n");
	(*token)->prev->str = ft_strjoin_with_free(&(*token)->prev->str, &(*token)->str);
	if ((*token)->prev->str == NULL)
		error("expansion.c 7: malloc error");
	(*token)->prev->len = ft_strlen((*token)->prev->str);
	(*token)->prev->next = (*token)->next;
	(*token)->next->prev = (*token)->prev;
	printf("join_token_and_token_perv 12: str == %s\n", (*token)->prev->str);
	tmp = *token;
//	*token =
	*token = (*token)->prev;
	free(tmp);
}

char	*strdup_before_valiable(char *tmp, char *variable_name)
{
	size_t	i;
	size_t	dif;

	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '$')
			dif = ft_strncmp(tmp + i + 1, variable_name, ft_strlen(variable_name));
		if (dif == 0)
			return (ft_substr(tmp, 0, i));
		i++;
	}
	return (NULL);
}

char	*expand_str(char *str)
{
	char	*expanded_value;
	char	*t_str;
	char	*variable_name;
	size_t	i;

	i = 0;
	t_str = ft_strdup(str);
	// printf("strdup : t_str = %s\n", t_str);
	while (str[i])
	{
		// printf("handle_token_in_quotes 52: i = %zu,   str[i] = %c\n", i, str[i]);
		if (str[i] == '$' && (ft_isalnum((unsigned char)(str[i + 1])) || str[i + 1] == '?'))
		{
			variable_name = ft_substr(str + i, 1, get_valiable_name_len(str + i + 1));
			// fprintf(stderr, "++++++ variable_name = %s\n", variable_name);
			// fprintf(stderr, "++++++ ? = %s\n", my_getenv(g_mshell->envlist, variable_name));
			expanded_value = ft_strdup(my_getenv(g_mshell->envlist, variable_name));
			// printf("handle_token_in_quotes 56: variable_name = %s, str[i] = %c\n",  variable_name, str[i]);
			if (expanded_value == NULL && errno == 0)
				expanded_value = ft_strdup("");
			// printf("handle_token_in_quotes 40: expanded_value = %s\n", expanded_value);
			expanded_value = ft_strjoin_with_free_no_null(strdup_before_valiable(t_str, variable_name), expanded_value);
			// printf("handle_token_in_quotes 42: expanded_value = %s\n", expanded_value);
			i += ft_strlen(variable_name);
			free(variable_name);
			free(t_str);
			t_str = ft_strjoin_with_free_no_null(expanded_value, ft_substr(str+ i + 1, 0, ft_strlen(str+ i + 1)));
			if (t_str == NULL)
				error("handle_token_in_quotes.c 84: malloc error");
			// printf("handle_token_in_quotes 54: t_str = %s, i = %zu\n", t_str, i);
//			printf("handle_token_in_quotes 54: t_str = %s, i = %d, t_str[i] = %c\n", t_str, i, t_str[i]);
//			continue;
		}
		i++;
	}
//	free(str);
//	str = t_str;
	return (t_str);
}

static t_token	*new_token(t_token_kind kind, char *p, size_t len)
{
	t_token	*token;

	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		error("malloc error\n");
	token->kind = kind;
	token->str = p;
	token->len = len;
	return (token);
}

static size_t	join_valiable(char **p, t_token **tok)
{
	t_token	*cur;
	t_token	head;
	size_t	count;

	count = 0;
	head.next = NULL;
	cur = &head;
	while (p[count])
	{
		cur->next = new_token(TK_WORD, p[count], ft_strlen(p[count]));
		cur->next->prev = cur;
		cur = cur->next;
		count++;
	}
	free(p);
	cur->next = (*tok)->next;
	cur->next->prev = cur;
	(*tok)->prev->next = head.next;
	head.next->prev = (*tok)->prev;
	free((*tok)->str);
	free(*tok);
	*tok = cur;
	return (count);
}

size_t	replace_token(t_token **token, char *str)
{
 	size_t	increase_tok_num;
	char	**strs;
	t_token	*tmp;

 	if (*str == '\0')
 	{
 		free(str);
		fprintf(stderr,"126: p = %s\n", (*token)->str);
		if (is_redirect_kind((*token)->prev->kind))
			return (1);
		 tmp = (*token)->prev;
		 if ((*token)->next->is_join_prev == true)
			 (*token)->next->is_join_prev = false;
 		(*token)->prev->next = (*token)->next;
 		(*token)->next->prev = (*token)->prev;
		free((*token)->str);
		free((*token));
		 *token = tmp;
 		return (0);
 	}
	 strs = ft_split(str, ' ');
 	if (strs == NULL)
		 error("expansion.c 153: malloc error");
	free(str);
 	increase_tok_num = join_valiable(strs, token);
 	debug_token((*token), 1);
 	// free((*token));

 	return (increase_tok_num);
 }

 size_t	expand_token(t_token **token, int op_kind)
 {
 	char	*expanded_value;

 	fprintf(stderr,"expand_token called\n");
 	if (op_kind == TK_OP_DOLLAR)
 	{
 		expanded_value = expand_str((*token)->str);
		// fprintf(stderr,"expand_token 173:  %s\n", expanded_value);
		// fprintf(stderr,"expand_token 174:  %s\n", (*token)->next->str);
 		if (errno)
 			error(strerror(errno));
 		return (replace_token(token, expanded_value));
 	}
 	return (0);
 }

void	expand_node(t_node *node) {
	int op_kind;
	size_t cur_index;
	size_t added_token_size;
	t_token *head;

	head = node->word_list->prev;
	cur_index = 0;
	fprintf(stderr, "word_list_size : %zu cur_index : %zu\n", node->word_list_size, cur_index);
	while (cur_index++ < node->word_list_size) {
		op_kind = node->word_list->kind;
		if (op_kind == TK_WORD_IN_SINGLE_Q || op_kind == TK_WORD_IN_DOUBLE_Q)
			handle_token_in_quotes(node->word_list);
		else if (op_kind == TK_OP_DOLLAR)
		{
			added_token_size = expand_token(&(node->word_list), op_kind);
			printf("==============token_size %zu\n", added_token_size);
//			if (added_token_size == 0 && node->word_list->kind != TK_WORD && node->word_list->kind != TK_EOF)
//				error(">|");
			cur_index += added_token_size - 1;
			node->word_list_size += added_token_size - 1;
			fprintf(stderr,"177: p = %p\n", node->word_list);
		}
		if (node->word_list->is_join_prev == true)
		{
			join_token_and_token_prev(&(node->word_list));
			node->word_list_size -= 1;
			cur_index--;
		}
//		fprintf(stderr, "226         word_list_size : %zu i : %zu,  node->word_list->str = %s \n", node->word_list_size, cur_index, node->word_list->str);
		node->word_list = node->word_list->next;
		}
		node->word_list = head->next;
		create_t_command(node);
	}

void	expansion(t_node *node)
{
	while (node->kind == ND_PIPE)
	{
		expand_node(node->right);
		node = node->left;
	}
	expand_node(node);
}
