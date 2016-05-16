#! /Users/user/Documents/CMC/prac4/server/server/mjs
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	/*while do*/
	var x = 1;
	while (!(x == 10))
	{
		write(x++, "\n");
	}
	write(" ",x, "\n");
	write("qwe", "\n");
	do
	{
		x = x + x;
		write(x);
	}while (x != 80);
	write("\n");
}
</SCRIPT>
