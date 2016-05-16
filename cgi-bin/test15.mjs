#! /Users/user/Documents/CMC/prac4/server/server/mjs
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	/*hard*/
	function z(x)
	{
		return 3 * x * x - 1;
	}
	function y(x)
	{
		if ( x > 0)
			return 2 * x + 3;
		else if (x < 0)
			return z(x);
		else return 0;
	}
	var sum = 0, i;
	for (i = -1000; i < 1000; ++i)
	{
		sum = sum + y(i);
	}
	write(sum, "\n");
}
</SCRIPT>