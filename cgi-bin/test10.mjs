#! /Users/user/Documents/CMC/prac4/server/server/mjs
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	function fact(n)
	{
		if (n == 1 || n == 0)
			return 1;
		return n * fact(n-1);
	}
	write(fact(5), "\n");
}
</SCRIPT>