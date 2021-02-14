/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 12:18:27 by sgertrud          #+#    #+#             */
/*   Updated: 2021/02/14 15:46:42 by sgertrud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long int	ft_atol(const char *str)
{
	long long int	out;
	char			negative;

	out = 0;
	negative = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-')
	{
		negative = 1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		if (out * 10 / 10 != out)
			return (negative ? 0 : -1);
		out = out * 10 - (*str++ - 48);
	}
	return (negative ? out : out * (-1));
}

int			ft_atoi(const char *str)
{
	long long int	out;
	char			negative;

	out = 0;
	negative = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-')
	{
		negative = 1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		if (out * 10 / 10 != out)
			return (negative ? 0 : -1);
		out = out * 10 - (*str++ - 48);
	}
	return (negative ? out : out * (-1));
}
