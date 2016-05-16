#! /Users/user/Documents/CMC/prac4/server/server/mjs
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	/*recursive functions*/
	function fact(n)
	{
		if (n == 1 || n == 0)
			return 1;
		return n * fact(n-1);
	}
	write(fact(10), "\n");
}
</SCRIPT>