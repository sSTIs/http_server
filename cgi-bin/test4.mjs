#! /Users/user/Documents/CMC/prac4/server/server/mjs
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	/*while do break continue*/
	var i = 1, j = 1;
	var k;
	while ( i < 10)
	{
		++i;
		if (i == 5)
			continue;
		while (j < 10)
		{

			if (j == 4)
				break;
			
			++j;
			if (j == 2)
				continue;
			write("j = ", j , " ");
		}
		write(i, " ");
		if (i == 7)
			break;
	}
	write("\n");
	for (i = 1; i < 10; ++i)
	{
		if (i == 5)
			continue;
		if (i == 7)
			break;
		write(i, " ");
	}
}
</SCRIPT>
