/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   just_atoi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduman <iduman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 21:06:10 by iduman            #+#    #+#             */
/*   Updated: 2025/09/04 21:06:10 by iduman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	int			result;
	int			i;
	signed char	a;

	result = 0;
	i = 0;
	a = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			a = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * a);
} // ft_usleep yapılabilir zaman algısı kayıyo
